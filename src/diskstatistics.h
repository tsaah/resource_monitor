#pragma once

#include "diskstatisticscollector.h"
#include "statisticsbase.h"

#include <QtQml/qqmlregistration.h>

class DiskStatistics final: public StatisticsBase {
    Q_OBJECT
    Q_PROPERTY(QVariantMap infos READ infos NOTIFY statisticsChanged)
    Q_PROPERTY(QStringList order READ order NOTIFY statisticsChanged)
    QML_NAMED_ELEMENT(Disks)
    QML_SINGLETON
public:
    explicit DiskStatistics(QObject* parent = nullptr);

    QVariantMap infos() const;
    QStringList order() const;

private slots:
    void onCollectingFinished();

private:
    static constexpr int defaultIntervalMs_{ 100 };
    DiskStatisticsCollector::Data data_;
};
