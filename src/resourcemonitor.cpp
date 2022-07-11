#include "resourcemonitor.h"
#include "statisticsreader.h"

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

    startTimer(100);
}

ResourceMonitor::~ResourceMonitor() {
    workerThread_.quit();
    workerThread_.wait();
}

Statistics ResourceMonitor::statistics() const {
    return statisticsReader_->currentStatistics();
}

void ResourceMonitor::timerEvent(QTimerEvent *event) {
    emit startStatisticsUpdate();
}
