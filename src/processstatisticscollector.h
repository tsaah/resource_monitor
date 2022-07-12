#pragma once

#include "istatisticscollector.h"

#include <QDir>
#include <QMutex>
#include <QObject>
#include <QThread>

class ProcessStatisticsCollector final: public IStatisticsCollector {
    Q_OBJECT
public:
    struct Data {
        quint64 processCount_{ 0 };
        quint64 threadCount_{ 0 };
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
