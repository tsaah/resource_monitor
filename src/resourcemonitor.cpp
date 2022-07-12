#include "resourcemonitor.h"
#include "statisticsreader.h"

const int ResourceMonitor::DEFAULT_INTERVAL_MS = 100;

ResourceMonitor::ResourceMonitor(QObject* parent)
    : QObject{ parent }
{
    statisticsReader_ = new StaticsticsReader;
    statisticsReader_->moveToThread(&workerThread_);
    connect(&workerThread_, &QThread::finished, statisticsReader_, &QObject::deleteLater);
    connect(&workerThread_, &QThread::started, statisticsReader_, &StaticsticsReader::collect);
    connect(this, &ResourceMonitor::startStatisticsUpdate, statisticsReader_, &StaticsticsReader::collect);
    connect(statisticsReader_, &StaticsticsReader::collectFinished, this, &ResourceMonitor::statisticsChanged);
    workerThread_.start();

    connect(this, &ResourceMonitor::intervalMsChanged, this, &ResourceMonitor::onIntervalChanged, Qt::DirectConnection);
    setIntervalMs(DEFAULT_INTERVAL_MS);
}

ResourceMonitor::~ResourceMonitor() {
    workerThread_.quit();
    workerThread_.wait();
}

Statistics ResourceMonitor::statistics() const {
    return statisticsReader_->currentStatistics();
}

void ResourceMonitor::timerEvent(QTimerEvent*) {
    emit startStatisticsUpdate();
}

void ResourceMonitor::onIntervalChanged() {
    if (Q_LIKELY(timerId_ != 0)) {
        killTimer(timerId_);
    }
    timerId_ = startTimer(intervalMs_);
}

int ResourceMonitor::intervalMs() const {
    return intervalMs_;
}

void ResourceMonitor::setIntervalMs(int newIntervalMs) {
    if (intervalMs_ == newIntervalMs) { return; }
    intervalMs_ = newIntervalMs;
    emit intervalMsChanged();
}
