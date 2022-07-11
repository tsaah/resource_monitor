#pragma once

#include <QObject>
#include <QThread>
#include <QtQml/qqmlregistration.h>

#include "statistics.h"

class StaticsticsReader;

class ResourceMonitor: public QObject {
    Q_OBJECT
    Q_PROPERTY(Statistics statistics READ statistics NOTIFY statisticsChanged)
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit ResourceMonitor(QObject* parent = nullptr);
    ~ResourceMonitor();

    Statistics statistics() const;

protected:
    void timerEvent(QTimerEvent* event);

signals:
    void startStatisticsUpdate();
    void statisticsChanged();

private:
    QThread workerThread_;
    StaticsticsReader* statisticsReader_{ nullptr };
};
