#include "statisticsbase.h"

StatisticsBase::StatisticsBase(IStatisticsCollector *collector, int intervalMs, QObject *parent)
    : QObject{ parent }
    , collector_{ collector }
    , intervalMs_{ intervalMs }
{
    assert(collector_);
    collector_->moveToThread(&workerThread_);
    connect(&workerThread_, &QThread::finished, collector_, &QObject::deleteLater);
    connect(&workerThread_, &QThread::started, collector_, &IStatisticsCollector::collect);
    connect(this, &StatisticsBase::startStatisticsUpdate, collector_, &IStatisticsCollector::collect);
    connect(collector_, &IStatisticsCollector::collectingFinished, this, &StatisticsBase::onCollectingFinished);
    workerThread_.start();

    connect(this, &StatisticsBase::intervalMsChanged, this, &StatisticsBase::onIntervalChanged, Qt::DirectConnection);

    timerId_ = startTimer(intervalMs_);
}

StatisticsBase::~StatisticsBase() {
    workerThread_.quit();
    workerThread_.wait();
}

int StatisticsBase::intervalMs() const {
    return intervalMs_;
}

void StatisticsBase::setIntervalMs(int newIntervalMs) {
    if (intervalMs_ == newIntervalMs) { return; }
    intervalMs_ = newIntervalMs;
    emit intervalMsChanged();
}

void StatisticsBase::onIntervalChanged() {
    if (Q_LIKELY(timerId_ != 0)) {
        killTimer(timerId_);
    }
    timerId_ = startTimer(intervalMs_);
}

void StatisticsBase::timerEvent(QTimerEvent *) {
    emit startStatisticsUpdate();
}
