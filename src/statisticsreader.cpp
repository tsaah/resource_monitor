#include "statisticsreader.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QVariant>

#include <filesystem>

#include <sys/statvfs.h>
#include <unistd.h>

namespace {

const int THREADS_INDEX = 19;

} // namespace

void StaticsticsReader::initialize() {
    if (Q_UNLIKELY(initialized_)) { return; }

    procDir_ = QDir(QStringLiteral("/proc"));
    if (Q_UNLIKELY(!procDir_.exists() || !procDir_.isReadable())) {
        qFatal("no /proc filesystem or it is not readable");
    }

    auto coreCount = sysconf(_SC_NPROCESSORS_ONLN);
    if (Q_UNLIKELY(coreCount < 1)) {
        coreCount = 1;
        qWarning() << "Could not determine number of cores, defaulting to 1.";
    }

    currentStatistics_.cpu_.coreCount_ = coreCount;
    currentStatistics_.cpu_.coreLoad_.resize(coreCount);
    coreOldTotals_.resize(coreCount);
    coreOldIdles_.resize(coreCount);

    initialized_ = true;
}

void StaticsticsReader::collect() {
    if (Q_UNLIKELY(!initialized_)) { initialize(); }
    static bool busy{ false };
    if (Q_LIKELY(!busy)) {
        busy = true;
        auto newStatistics = currentStatistics_;
        collectCpuLoad(newStatistics.cpu_);
        collectNumberOfThreadsAndProcesses(newStatistics.process_);
        collectAvailableMemoryAndMemoryUsed(newStatistics.memory_);
        collectDiskInfoTotalAvailableUsed(newStatistics.disk_);
        {
            QMutexLocker locker(&mutex_);
            currentStatistics_ = std::move(newStatistics);
        }
        busy = false;
        emit collectFinished();
    }
}

Statistics StaticsticsReader::currentStatistics() const {
    QMutexLocker locker(&mutex_);
    return currentStatistics_;
}

void StaticsticsReader::collectCpuLoad(CpuStats& cpu) {
    QFile stat(procDir_.absolutePath() + "/stat");
    if (Q_LIKELY(stat.open(QIODevice::ReadOnly | QIODevice::Text))) {
        QTextStream stream(&stat);
        for (int i = 0;; i++) {
            QString entry;
            stream >> entry;
            if (Q_UNLIKELY(entry.left(3) != "cpu")) { break; }
            stream.skipWhiteSpace();

            QVector<qint64> times;
            times.reserve(10);
            qint64 totalSum = 0;

            for (;;) {
                quint64 value = 0;
                stream >> value;
                if (Q_LIKELY(value > 0 || stream.status() == QTextStream::Ok)) {
                    times.append(value);
                    totalSum += value;
                } else {
                    stream.resetStatus();
                    break;
                }
            }

            if (Q_UNLIKELY(times.size() < 4)) {
                qFatal("Unexpected /proc/stat structure");
            }

            // Subtract fields 8-9 and any future unknown fields
            const qint64 totals = std::max(0ll, totalSum - (times.size() > 8 ? std::accumulate(times.begin() + 8, times.end(), 0) : 0));

            // Add iowait field if present
            const qint64 idles = std::max(0ll, times.at(3) + (times.size() > 4 ? times.at(4) : 0));

            // Calculate values for totals from first line of stat
            if (i == 0) {
                const qint64 calcTotals = std::max(1ll, totals - oldTotals_);
                const qint64 calcIdles = std::max(1ll, idles - oldIdles_);
                oldTotals_ = totals;
                oldIdles_ = idles;

                cpu.load_ = std::clamp(static_cast<qreal>(calcTotals - calcIdles) / static_cast<qreal>(calcTotals), 0.0, 1.0);
            } else { // Calculate cpu total for each core
                if (i > cpu.coreCount_) { break; }
                const qint64 calcTotals = std::max(0ll, totals - coreOldTotals_.at(i - 1));
                const qint64 calcIdles = std::max(0ll, idles - coreOldIdles_.at(i - 1));
                coreOldTotals_[i - 1] = totals;
                coreOldIdles_[i - 1] = idles;

                cpu.coreLoad_[i - 1] = std::clamp(static_cast<qreal>(calcTotals - calcIdles) / static_cast<qreal>(calcTotals), 0.0, 1.0);
            }
        }
        stat.close();
    }
}


void StaticsticsReader::collectAvailableMemoryAndMemoryUsed(MemoryStats& memory) {
    memory.swapTotal_ = 0;

    // Read memory info from /proc/meminfo
    QFile meminfo(procDir_.absolutePath() + "/meminfo");
    if (Q_LIKELY(meminfo.open(QIODevice::ReadOnly | QIODevice::Text))) {
        QTextStream stream(&meminfo);
        bool hasAvailableMemory = false;
        for (;;) {
            QString label;
            stream >> label;
            if (label == "Dirty:") {
                break;
            } else if (label == "MemTotal:") {
                stream >> memory.memoryTotal_;
                memory.memoryTotal_ <<= 10;
            } else if (label == "MemFree:") {
                stream >> memory.memoryFree_;
                memory.memoryFree_ <<= 10;
            } else if (label == "MemAvailable:") {
                stream >> memory.memoryAvailable_;
                memory.memoryAvailable_ <<= 10;
                hasAvailableMemory = true;
            } else if (label == "Cached:") {
                stream >> memory.memoryCached_;
                memory.memoryCached_ <<= 10;
            } else if (label == "SwapTotal:") {
                stream >> memory.swapTotal_;
                memory.swapTotal_ <<= 10;
            } else if (label == "SwapFree:") {
                stream >> memory.swapFree_;
                memory.swapFree_ <<= 10;
                break;
            }
            stream.readLine();
        }
        if (Q_UNLIKELY(!hasAvailableMemory)) {
            memory.memoryAvailable_ = memory.memoryFree_ + memory.memoryCached_;
        }
        memory.memoryUsed_ = memory.memoryTotal_ - memory.memoryAvailable_;
        if (Q_LIKELY(memory.swapTotal_ > 0)) {
            memory.swapUsed_ = memory.swapTotal_ - memory.swapFree_;
        }
        meminfo.close();
    } else {
        qFatal("Failed to read /proc/meminfo");
    }
}

void StaticsticsReader::collectNumberOfThreadsAndProcesses(ProcessStats& process) {
    quint64 processCount = 0;
    quint64 threadCount = 0;
    const auto procSubdirs = procDir_.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Readable);
    for (const auto& subdir: procSubdirs) {
        const auto pidString = subdir.fileName();
        bool isANumber = false;
        pidString.toULongLong(&isANumber);
        ++processCount;
        QFile pidstat(subdir.absoluteFilePath() + "/stat");
        if (Q_LIKELY(pidstat.open(QIODevice::ReadOnly | QIODevice::Text))) {
            const auto threads = pidstat.readAll().split(' ')[THREADS_INDEX];
            threadCount += threads.toULongLong();
        }
    }
    process.processCount_ = processCount;
    process.threadCount_ = threadCount;
}

void StaticsticsReader::collectDiskInfoTotalAvailableUsed(DiskStats &disks) {
    auto& diskInfos = disks.infos_;

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
        QVector<QString> found;
        found.reserve(lastFound_.size());
        QByteArray mountpoint, fstype;

        const auto lines = mountsContents.trimmed().split('\n');
        for (const auto& line: lines) {
            const auto data = line.split(' ');
            mountpoint = data[1];
            fstype = data[2];

            if (found.contains(mountpoint)) {
                continue;
            }

            if (fstypes.contains(fstype)) {
                found.append(mountpoint);

                if (!disks.infos_.contains(mountpoint)) {
                    DiskInfo diskInfo{ mountpoint.split('/').last() };
                    if (diskInfo.name_.isEmpty()) {
                        diskInfo.name_ = (mountpoint == "/" ? "root" : mountpoint);
                    }
                    disks.infos_[mountpoint] = QVariant::fromValue(diskInfo);
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
        auto diskInfo = i.value().value<DiskInfo>();
        diskInfo.total_ = vfs.f_blocks * vfs.f_frsize;
        diskInfo.available_ = vfs.f_bavail * vfs.f_frsize;
        diskInfo.free_ = vfs.f_bfree * vfs.f_frsize;
        diskInfo.used_ = diskInfo.total_ - diskInfo.available_;
        disks.infos_[mountpoint] = QVariant::fromValue(diskInfo);
    }

    // set diskInfos order
    disks.disksOrder_.clear();
    if (diskInfos.contains("/")) {
        disks.disksOrder_.append("/");
    }
    for (const auto& name: lastFound_) {
        if (name != "/") {
            disks.disksOrder_.append(name);
        }
    }
}

