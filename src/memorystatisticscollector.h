#pragma once

#include "istatisticscollector.h"

#include <QDir>
#include <QMutex>
#include <QObject>
#include <QThread>

class MemoryStatisticsCollector final: public IStatisticsCollector {
    Q_OBJECT
public:
    struct Data {
        quint64 memoryTotal_{ 0 };
        quint64 memoryFree_{ 0 };
        quint64 memoryUsed_{ 0 };
        quint64 memoryAvailable_{ 0 };
        quint64 memoryCached_{ 0 };
        quint64 swapTotal_{ 0 };
        quint64 swapFree_{ 0 };
        quint64 swapUsed_{ 0 };
    };

    Data data() const;
    void collect() override;

private:
    void initialize();
    void collectStats(Data& data);

    Data data_;
    mutable QMutex mutex_;

    QDir procDir_;
};
