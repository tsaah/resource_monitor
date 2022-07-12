#pragma once

#include "memorystatisticscollector.h"
#include "statisticsbase.h"

#include <QtQml/qqmlregistration.h>

class MemoryStatistics final: public StatisticsBase {
    Q_OBJECT
    Q_PROPERTY(qint64 memoryTotal READ memoryTotal NOTIFY statisticsChanged)
    Q_PROPERTY(qint64 memoryFree READ memoryFree NOTIFY statisticsChanged)
    Q_PROPERTY(qint64 memoryAvailable READ memoryAvailable NOTIFY statisticsChanged)
    Q_PROPERTY(qint64 memoryUsed READ memoryUsed NOTIFY statisticsChanged)
    Q_PROPERTY(qint64 memoryCached READ memoryCached NOTIFY statisticsChanged)
    Q_PROPERTY(qint64 swapTotal READ swapTotal NOTIFY statisticsChanged)
    Q_PROPERTY(qint64 swapFree READ swapFree NOTIFY statisticsChanged)
    Q_PROPERTY(qint64 swapUsed READ swapUsed NOTIFY statisticsChanged)
    QML_NAMED_ELEMENT(Memory)
    QML_SINGLETON
public:
    explicit MemoryStatistics(QObject* parent = nullptr);

    quint64 memoryTotal() const;
    quint64 memoryFree() const;
    quint64 memoryUsed() const;
    quint64 memoryAvailable() const;
    quint64 memoryCached() const;
    quint64 swapTotal() const;
    quint64 swapFree() const;
    quint64 swapUsed() const;

private slots:
    void onCollectingFinished();

private:
    static constexpr int defaultIntervalMs_{ 200 };
    MemoryStatisticsCollector::Data data_;
};
