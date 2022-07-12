#pragma once

#include <QObject>
#include <QThread>
#include <QtQml/qqmlregistration.h>

#include "statistics.h"

class StaticsticsReader;

class ResourceMonitor: public QObject {
    Q_OBJECT
    Q_PROPERTY(int intervalMs READ intervalMs WRITE setIntervalMs NOTIFY intervalMsChanged)
    Q_PROPERTY(Statistics statistics READ statistics NOTIFY statisticsChanged)
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit ResourceMonitor(QObject* parent = nullptr);
    ~ResourceMonitor();

    Statistics statistics() const;
    int intervalMs() const;
    void setIntervalMs(int newIntervalMs);

protected:
    void timerEvent(QTimerEvent*);

signals:
    void startStatisticsUpdate();
    void statisticsChanged();
    void intervalMsChanged();

private slots:
    void onIntervalChanged();

private:
    static const int DEFAULT_INTERVAL_MS;
    QThread workerThread_;
    StaticsticsReader* statisticsReader_{ nullptr };
    int intervalMs_{ 0 };
    int timerId_{ 0 };
};
