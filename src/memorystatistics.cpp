#include "memorystatistics.h"

MemoryStatistics::MemoryStatistics(QObject *parent)
    : StatisticsBase{ new MemoryStatisticsCollector, defaultIntervalMs_, parent }
{
}

quint64 MemoryStatistics::memoryTotal() const {
    return data_.memoryTotal_;
}

quint64 MemoryStatistics::memoryFree() const {
    return data_.memoryFree_;
}

quint64 MemoryStatistics::memoryUsed() const {
    return data_.memoryUsed_;
}

quint64 MemoryStatistics::memoryAvailable() const {
    return data_.memoryAvailable_;
}

quint64 MemoryStatistics::memoryCached() const {
    return data_.memoryCached_;
}

quint64 MemoryStatistics::swapTotal() const {
    return data_.swapTotal_;
}

quint64 MemoryStatistics::swapFree() const {
    return data_.swapFree_;
}

quint64 MemoryStatistics::swapUsed() const {
    return data_.swapUsed_;
}

void MemoryStatistics::onCollectingFinished() {
    data_ = static_cast<MemoryStatisticsCollector*>(collector_)->data();
    emit statisticsChanged();
}
