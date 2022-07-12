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
    PRIVATE Qt6::Gui Qt6::Quick Qt6::Charts Qt6::Qml
)
