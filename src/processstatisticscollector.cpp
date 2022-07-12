#include "processstatisticscollector.h"

namespace {

const int THREADS_INDEX = 19;

} // namespace

ProcessStatisticsCollector::Data ProcessStatisticsCollector::data() const {
    QMutexLocker locker(&mutex_);
    return data_;
}

void ProcessStatisticsCollector::collect() {
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

void ProcessStatisticsCollector::initialize() {
    procDir_ = QDir(QStringLiteral("/proc"));
    if (Q_UNLIKELY(!procDir_.exists() || !procDir_.isReadable())) {
        qFatal("no /proc filesystem or it is not readable");
    }
}

void ProcessStatisticsCollector::collectStats(Data &data) {
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
    data.processCount_ = processCount;
    data.threadCount_ = threadCount;
}
