qt_add_executable(resource_monitor
    main.cpp
)

qt_add_resources(resource_monitor "qmlresources"
    PREFIX "/qml"
    FILES main.qml
)

qt_add_qml_module(resource_monitor
    URI my.rimac.task
    VERSION 1.0
    SOURCES
        istatisticscollector.h
        statisticsbase.h statisticsbase.cpp
        cpustatistics.h cpustatistics.cpp
        cpustatisticscollector.h cpustatisticscollector.cpp
        processstatistics.h processstatistics.cpp
        processstatisticscollector.h processstatisticscollector.cpp
        memorystatistics.h memorystatistics.cpp
        memorystatisticscollector.h memorystatisticscollector.cpp
        diskstatistics.h diskstatistics.cpp
        diskstatisticscollector.h diskstatisticscollector.cpp
    QML_FILES
        MyTaskQml/CpuView.qml
        MyTaskQml/CpuBar.qml
        MyTaskQml/ProcessView.qml
        MyTaskQml/MemoryView.qml
        MyTaskQml/MemoryBar.qml
        MyTaskQml/DisksView.qml
        MyTaskQml/DiskView.qml
        MyTaskQml/Common.js
)

target_link_libraries(resource_monitor
    PRIVATE Qt6::Gui Qt6::Quick
)
