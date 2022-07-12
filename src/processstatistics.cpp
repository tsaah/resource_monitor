#include "processstatistics.h"

ProcessStatistics::ProcessStatistics(QObject *parent)
    : StatisticsBase{ new ProcessStatisticsCollector, defaultIntervalMs_, parent }
{
}

quint64 ProcessStatistics::processCount() const {
    return data_.processCount_;
}

quint64 ProcessStatistics::threadCount() const {
    return data_.threadCount_;
}

void ProcessStatistics::onCollectingFinished() {
    data_ = static_cast<ProcessStatisticsCollector*>(collector_)->data();
    emit statisticsChanged();
}
