#pragma once

#include "istatisticscollector.h"

#include <QObject>
#include <QThread>

class StatisticsBase: public QObject {
    Q_OBJECT
    Q_PROPERTY(int intervalMs READ intervalMs WRITE setIntervalMs NOTIFY intervalMsChanged)
public:
    explicit StatisticsBase(IStatisticsCollector* collector, int intervalMs, QObject* parent = nullptr);
    virtual ~StatisticsBase();

    int intervalMs() const;
    void setIntervalMs(int newIntervalMs);

signals:
    void startStatisticsUpdate();
    void statisticsChanged();
    void intervalMsChanged();

protected:
    IStatisticsCollector* collector_{ nullptr };

protected slots:
    virtual void onCollectingFinished() = 0;

private slots:
    void onIntervalChanged();

private:
    void timerEvent(QTimerEvent*);

    QThread workerThread_;
    int intervalMs_{ 0 };
    int timerId_{ 0 };
};

