#include "diskstatistics.h"

DiskStatistics::DiskStatistics(QObject *parent)
    : StatisticsBase{ new DiskStatisticsCollector, defaultIntervalMs_, parent }
{
}

QVariantMap DiskStatistics::infos() const {
    return data_.infos_;
}

QStringList DiskStatistics::order() const {
    return data_.order_;
}

void DiskStatistics::onCollectingFinished() {
    data_ = static_cast<DiskStatisticsCollector*>(collector_)->data();
    emit statisticsChanged();
}
