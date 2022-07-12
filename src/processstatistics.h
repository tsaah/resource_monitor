#pragma once

#include "processstatisticscollector.h"
#include "statisticsbase.h"

#include <QtQml/qqmlregistration.h>

class ProcessStatistics final: public StatisticsBase {
    Q_OBJECT
    Q_PROPERTY(quint64 processCount READ processCount NOTIFY statisticsChanged)
    Q_PROPERTY(quint64 threadCount READ threadCount NOTIFY statisticsChanged)
    QML_NAMED_ELEMENT(Process)
    QML_SINGLETON
public:
    explicit ProcessStatistics(QObject* parent = nullptr);

    quint64 processCount() const;
    quint64 threadCount() const;

private slots:
    void onCollectingFinished();

private:
    static constexpr int defaultIntervalMs_{ 1000 };
    ProcessStatisticsCollector::Data data_;
};
