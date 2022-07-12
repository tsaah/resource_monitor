#include "memorystatisticscollector.h"

#include <QDebug>
#include <QFile>

MemoryStatisticsCollector::Data MemoryStatisticsCollector::data() const {
    QMutexLocker locker(&mutex_);
    return data_;
}

void MemoryStatisticsCollector::collect() {
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

void MemoryStatisticsCollector::initialize() {
    procDir_ = QDir(QStringLiteral("/proc"));
    if (Q_UNLIKELY(!procDir_.exists() || !procDir_.isReadable())) {
        qFatal("no /proc filesystem or it is not readable");
    }
}

void MemoryStatisticsCollector::collectStats(Data &data) {
    data.swapTotal_ = 0;

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
                stream >> data.memoryTotal_;
                data.memoryTotal_ <<= 10;
            } else if (label == "MemFree:") {
                stream >> data.memoryFree_;
                data.memoryFree_ <<= 10;
            } else if (label == "MemAvailable:") {
                stream >> data.memoryAvailable_;
                data.memoryAvailable_ <<= 10;
                hasAvailableMemory = true;
            } else if (label == "Cached:") {
                stream >> data.memoryCached_;
                data.memoryCached_ <<= 10;
            } else if (label == "SwapTotal:") {
                stream >> data.swapTotal_;
                data.swapTotal_ <<= 10;
            } else if (label == "SwapFree:") {
                stream >> data.swapFree_;
                data.swapFree_ <<= 10;
                break;
            }
            stream.readLine();
        }
        if (Q_UNLIKELY(!hasAvailableMemory)) {
            data.memoryAvailable_ = data.memoryFree_ + data.memoryCached_;
        }
        data.memoryUsed_ = data.memoryTotal_ - data.memoryAvailable_;
        if (Q_LIKELY(data.swapTotal_ > 0)) {
            data.swapUsed_ = data.swapTotal_ - data.swapFree_;
        }
        meminfo.close();
    } else {
        qFatal("Failed to read /proc/meminfo");
    }
}
