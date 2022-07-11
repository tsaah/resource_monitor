#pragma once

#include <QObject>
#include <QMap>
#include <QDir>

struct CpuStats {
    Q_GADGET
    Q_PROPERTY(qint32 coreCount READ coreCount)
    Q_PROPERTY(qreal load READ load)
    Q_PROPERTY(QVector<qreal> coreLoad READ coreLoad)
public:
    qint32 coreCount_{ 0 };
    qreal load_{ 0 };
    qint32 coreCount() const { return coreCount_; }
    qreal load() const { return load_; }
    QVector<qreal> coreLoad_;
    QVector<qreal> coreLoad() const { return coreLoad_; }
};

struct MemoryStats {
    Q_GADGET
    Q_PROPERTY(qint64 memoryTotal READ memoryTotal)
    Q_PROPERTY(qint64 memoryFree READ memoryFree)
    Q_PROPERTY(qint64 memoryAvailable READ memoryAvailable)
    Q_PROPERTY(qint64 memoryUsed READ memoryUsed)
    Q_PROPERTY(qint64 memoryCached READ memoryCached)
    Q_PROPERTY(qint64 swapTotal READ swapTotal)
    Q_PROPERTY(qint64 swapFree READ swapFree)
    Q_PROPERTY(qint64 swapUsed READ swapUsed)
public:
    qint64 memoryTotal_{ 0 };
    qint64 memoryTotal() const { return memoryTotal_; }
    qint64 memoryFree_{ 0 };
    qint64 memoryFree() const { return memoryFree_; }
    qint64 memoryUsed_{ 0 };
    qint64 memoryUsed() const { return memoryUsed_; }
    qint64 memoryAvailable_{ 0 };
    qint64 memoryAvailable() const { return memoryAvailable_; }
    qint64 memoryCached_{ 0 };
    qint64 memoryCached() const { return memoryCached_; }
    qint64 swapTotal_{ 0 };
    qint64 swapTotal() const { return swapTotal_; }
    qint64 swapFree_{ 0 };
    qint64 swapFree() const { return swapFree_; }
    qint64 swapUsed_{ 0 };
    qint64 swapUsed() const { return swapUsed_; }
};

struct ProcessStats {
    Q_GADGET
    Q_PROPERTY(quint64 processCount READ processCount)
    Q_PROPERTY(quint64 threadCount READ threadCount)
public:
    quint64 processCount_{ 0 };
    quint64 processCount() const { return processCount_; }
    quint64 threadCount_{ 0 };
    quint64 threadCount() const { return threadCount_; }
};

struct DiskInfo {
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(quint64 total READ total)
    Q_PROPERTY(quint64 used READ used)
    Q_PROPERTY(quint64 available READ available)
    Q_PROPERTY(quint64 free READ free)
public:
    QString name_;
    QString name() const { return name_; }
    quint64 total_{ 0 };
    quint64 total() const { return total_; }
    quint64 used_{ 0 };
    quint64 used() const { return used_; }
    quint64 available_{ 0 };
    quint64 available() const { return available_; }
    quint64 free_{ 0 };
    quint64 free() const { return free_; }
};

struct DiskStats {
    Q_GADGET
    Q_PROPERTY(QVariantMap infos READ infos)
    Q_PROPERTY(QVector<QString> disksOrder READ disksOrder)
public:
    QVariantMap infos_;
    QVariantMap infos() const { return infos_; }
    QVector<QString> disksOrder_;
    QVector<QString> disksOrder() const { return disksOrder_; }
};

struct Statistics {
    Q_GADGET
    Q_PROPERTY(CpuStats cpu READ cpu)
    Q_PROPERTY(MemoryStats memory READ memory)
    Q_PROPERTY(ProcessStats process READ process)
    Q_PROPERTY(DiskStats disk READ disk)
public:
    CpuStats cpu() const { return cpu_; }
    MemoryStats memory() const { return memory_; }
    ProcessStats process() const { return process_; }
    DiskStats disk() const { return disk_; }
    CpuStats cpu_;
    MemoryStats memory_;
    ProcessStats process_;
    DiskStats disk_;
};
