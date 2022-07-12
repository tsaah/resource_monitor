#pragma once

#include "cpustatisticscollector.h"
#include "statisticsbase.h"

#include <QtQml/qqmlregistration.h>

class CpuStatistics final: public StatisticsBase {
    Q_OBJECT
    Q_PROPERTY(qint32 coreCount READ coreCount NOTIFY statisticsChanged)
    Q_PROPERTY(qreal load READ load NOTIFY statisticsChanged)
    Q_PROPERTY(QList<qreal> coreLoad READ coreLoad NOTIFY statisticsChanged)
    QML_NAMED_ELEMENT(Cpu)
    QML_SINGLETON
public:
    explicit CpuStatistics(QObject* parent = nullptr);

    qint32 coreCount() const;
    qreal load() const;
    QList<qreal> coreLoad() const;

private slots:
    void onCollectingFinished();

private:
    static constexpr int defaultIntervalMs_{ 100 };
    CpuStatisticsCollector::Data data_;
};
