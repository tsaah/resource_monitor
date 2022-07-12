#include "cpustatistics.h"

CpuStatistics::CpuStatistics(QObject *parent)
    : StatisticsBase{ new CpuStatisticsCollector, defaultIntervalMs_, parent }
{
}

qint32 CpuStatistics::coreCount() const {
    return data_.coreCount_;
}

qreal CpuStatistics::load() const {
    return data_.load_;
}

QList<qreal> CpuStatistics::coreLoad() const {
    return data_.coreLoad_;
}

void CpuStatistics::onCollectingFinished() {
    data_ = static_cast<CpuStatisticsCollector*>(collector_)->data();
    emit statisticsChanged();
}
