#pragma once

#include <QObject>

class IStatisticsCollector: public QObject {
    Q_OBJECT
public:
    virtual ~IStatisticsCollector() = default;

    virtual void collect() = 0;

signals:
    void collectingFinished();
};
