#pragma once

#include "istatisticscollector.h"

#include <QDir>
#include <QMutex>
#include <QObject>
#include <QThread>

class DiskStatisticsCollector final: public IStatisticsCollector {
    Q_OBJECT
public:
    struct Data {
        QVariantMap infos_;
        QStringList order_;
    };

    Data data() const;
    void collect() override;

private:
    void initialize();
    void collectStats(Data& data);

    Data data_;
    mutable QMutex mutex_;

    QDir procDir_;
    QStringList lastFound_;
};
