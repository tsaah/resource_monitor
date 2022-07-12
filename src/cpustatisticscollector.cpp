#include "cpustatisticscollector.h"

#include <QDebug>
#include <QFile>

#include <unistd.h>

CpuStatisticsCollector::Data CpuStatisticsCollector::data() const {
    QMutexLocker locker(&mutex_);
    return data_;
}

void CpuStatisticsCollector::collect() {
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

void CpuStatisticsCollector::initialize() {
    procDir_ = QDir(QStringLiteral("/proc"));
    if (Q_UNLIKELY(!procDir_.exists() || !procDir_.isReadable())) {
        qFatal("no /proc filesystem or it is not readable");
    }

    auto coreCount = sysconf(_SC_NPROCESSORS_ONLN);
    if (Q_UNLIKELY(coreCount < 1)) {
        coreCount = 1;
        qWarning() << "Could not determine number of cores, defaulting to 1.";
    }

    data_.coreCount_ = coreCount;
    data_.coreLoad_.resize(coreCount);

    oldCoreTotals_.resize(coreCount);
    oldCoreIdles_.resize(coreCount);
}

void CpuStatisticsCollector::collectStats(Data &data) {
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
                const qint64 calcTotals = std::max(1ll, totals - oldCpuTotals_);
                const qint64 calcIdles = std::max(1ll, idles - oldCpuIdles_);
                oldCpuTotals_ = totals;
                oldCpuIdles_ = idles;

                data.load_ = std::clamp(static_cast<qreal>(calcTotals - calcIdles) / static_cast<qreal>(calcTotals), 0.0, 1.0);
            } else { // Calculate cpu total for each core
                if (i > data.coreCount_) { break; }
                const qint64 calcTotals = std::max(0ll, totals - oldCoreTotals_.at(i - 1));
                const qint64 calcIdles = std::max(0ll, idles - oldCoreIdles_.at(i - 1));
                oldCoreTotals_[i - 1] = totals;
                oldCoreIdles_[i - 1] = idles;

                data.coreLoad_[i - 1] = std::clamp(static_cast<qreal>(calcTotals - calcIdles) / static_cast<qreal>(calcTotals), 0.0, 1.0);
            }
        }
        stat.close();
    }
}
