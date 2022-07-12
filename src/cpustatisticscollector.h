#pragma once

#include "istatisticscollector.h"

#include <QDir>
#include <QMutex>
#include <QObject>
#include <QThread>

class CpuStatisticsCollector final: public IStatisticsCollector {
    Q_OBJECT
public:
    struct Data {
        quint32 coreCount_{ 0 };
        qreal load_{ 0 };
        QList<qreal> coreLoad_;
    };

    Data data() const;
    void collect() override;

private:
    void initialize();
    void collectStats(Data& data);

    Data data_;
    mutable QMutex mutex_;

    QDir procDir_;
    qint64 oldCpuTotals_{ 0 };
    qint64 oldCpuIdles_{ 0 };
    QVector<qint64> oldCoreTotals_;
    QVector<qint64> oldCoreIdles_;
};
