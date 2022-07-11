#pragma once

#include <QObject>
#include <QDir>
#include "statistics.h"

class StaticsticsReader: public QObject {
    Q_OBJECT
public:
    Statistics currentStatistics() const;

public slots:
    void collect();

signals:
    void collectFinished();

private:
    void initialize();
    void collectCpuLoad(CpuStats &cpu);
    void collectNumberOfThreadsAndProcesses(ProcessStats &process);
    void collectAvailableMemoryAndMemoryUsed(MemoryStats& memory);
    void collectDiskInfoTotalAvailableUsed(DiskStats& disks);

    bool initialized_{ false };
    bool busy_{ false };
    QDir procDir_;
    Statistics currentStatistics_;
    qint64 oldTotals_;
    qint64 oldIdles_;
    QVector<qint64> coreOldTotals_;
    QVector<qint64> coreOldIdles_;
    QVector<QString> lastFound_;
};
