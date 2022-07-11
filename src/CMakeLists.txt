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
        resourcemonitor.h resourcemonitor.cpp
        statisticsreader.h statisticsreader.cpp
        statistics.h
    QML_FILES
        CpuWidget.qml
        ProcessWidget.qml
        MemoryWidget.qml
        DiskWidget.qml
)

target_link_libraries(resource_monitor
    PRIVATE Qt6::Gui Qt6::Quick Qt6::Charts Qt6::Qml
)
