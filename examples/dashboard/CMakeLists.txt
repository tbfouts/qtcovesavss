qt_add_executable(vss_dashboard WIN32
    main.cpp
)

qt_add_qml_module(vss_dashboard
    URI Dashboard
    QML_FILES
        Main.qml
        CircularGauge.qml
        ArcGauge.qml
)

target_link_libraries(vss_dashboard PRIVATE
    covesavss_vehicle_frontend
    covesavss_powertrain_frontend
    Qt::Quick
)

# Let the app discover the KUKSA backend plugin from the build tree
target_compile_definitions(vss_dashboard PRIVATE
    BUILD_PLUGIN_PATH="${CMAKE_BINARY_DIR}/plugins"
)

# Tell qmlls / Qt Creator where the generated QML modules live
set(QML_IMPORT_PATH
    ${CMAKE_BINARY_DIR}/src/vehicle
    ${CMAKE_BINARY_DIR}/src/powertrain
    CACHE STRING "" FORCE
)
