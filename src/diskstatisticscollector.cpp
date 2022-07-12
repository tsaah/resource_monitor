#include "diskstatisticscollector.h"

#include <QVariant>

#include <sys/statvfs.h>

DiskStatisticsCollector::Data DiskStatisticsCollector::data() const {
    QMutexLocker locker(&mutex_);
    return data_;
}

void DiskStatisticsCollector::collect() {
    static bool initialized{ false };
    if (Q_UNLIKELY(!initialized)) { initialize(); }

    static bool busy{ false };
    if (Q_LIKELY(!busy)) {
        busy = true;
        auto newData = data_;
        collectStats(newData);
        {
            QMutexLocker locker(&mutex_);
            data_ = std::move(newData);
        }
        busy = false;
        emit collectingFinished();
    }
}

void DiskStatisticsCollector::initialize() {
    procDir_ = QDir(QStringLiteral("/proc"));
    if (Q_UNLIKELY(!procDir_.exists() || !procDir_.isReadable())) {
        qFatal("no /proc filesystem or it is not readable");
    }
}

void DiskStatisticsCollector::collectStats(Data &data) {
    auto& diskInfos = data.infos_;

    // Get list of "real" filesystems from /proc/filesystems
    QSet<QString> skipSet = { "nodev", "squashfs", "nullfs" };
    QSet<QString> fstypes = { "zfs", "wslfs", "drvfs" };
    QFile filesystems(procDir_.absolutePath() + "/filesystems");
    if (Q_LIKELY(filesystems.open(QIODevice::ReadOnly | QIODevice::Text))) {
        const auto filesystemsList = filesystems.readAll().trimmed().split('\n');
        for (const auto& line: filesystemsList) {
            const auto fsInfo = line.trimmed().split('\t');
            QString fstype = fsInfo.last();
            if (!skipSet.contains(fstype) && !skipSet.contains(fsInfo.first())) {
                fstypes.insert(fstype);
            }
        }
        filesystems.close();
    } else {
        qFatal("Failed to read /proc/filesystems");
    }

    // Get mounts from /etc/mtab or /proc/self/mounts
    const QDir mtabPath("/etc/mtab");
    const QDir selfmountsPath(procDir_.absolutePath() + "/self/mounts");
    const QString mountsPath = mtabPath.exists() ? mtabPath.absolutePath() : selfmountsPath.absolutePath();
    QFile mounts(mountsPath);
    if (Q_LIKELY(mounts.open(QIODevice::ReadOnly | QIODevice::Text))) {
        const auto mountsContents = mounts.readAll();
        QStringList found;
        found.reserve(lastFound_.size());
        QByteArray mountpoint, fstype;

        const auto lines = mountsContents.trimmed().split('\n');
        for (const auto& line: lines) {
            const auto splittedLine = line.split(' ');
            mountpoint = splittedLine[1];
            fstype = splittedLine[2];

            if (found.contains(mountpoint)) {
                continue;
            }

            if (fstypes.contains(fstype)) {
                found.append(mountpoint);

                if (!data.infos_.contains(mountpoint)) {
                    QVariantMap info;
                    auto name = mountpoint.split('/').last();
                    if (name.isEmpty()) {
                        name = (mountpoint == "/" ? "root" : mountpoint);
                    }
                    info["name"] = name;
                    data.infos_[mountpoint] = info;
                }
            }
        }
        // remove diskInfos no longer mounted
        for (auto i = diskInfos.constBegin(); i != diskInfos.constEnd();) {
            if (!found.contains(i.key())) {
                i = diskInfos.erase(i);
            } else {
                ++i;
            }
        }
        lastFound_ = std::move(found);
        mounts.close();
    } else {
        qFatal("Failed to get mounts from /etc/mtab and /proc/self/mounts");
    }

    // read disk sizes
    for (auto i = diskInfos.begin(); i != diskInfos.end(); ++i) {
        const auto& mountpoint = i.key();
        QDir mountpointDir(mountpoint);
        if (Q_UNLIKELY(!mountpointDir.exists())) { continue; }

        struct statvfs64 vfs;
        if (Q_UNLIKELY(statvfs64(mountpoint.toStdString().c_str(), &vfs) < 0)) {
            qWarning() << "Failed to get disk/partition stats with statvfs() for: " <<  mountpoint;
            continue;
        }

        auto info = i.value().toMap();
        const auto total = static_cast<quint64>(vfs.f_blocks * vfs.f_frsize);
        const auto available = static_cast<quint64>(vfs.f_bavail * vfs.f_frsize);
        info["total"] = total;
        info["available"] = available;
        info["free"] = static_cast<quint64>(vfs.f_bfree * vfs.f_frsize);
        info["used"] = total - available;
        data.infos_[mountpoint] = info;
    }

    // set diskInfos order
    data.order_.clear();
    if (diskInfos.contains("/")) {
        data.order_.append("/");
    }
    for (const auto& name: lastFound_) {
        if (name != "/") {
            data.order_.append(name);
        }
    }
}
