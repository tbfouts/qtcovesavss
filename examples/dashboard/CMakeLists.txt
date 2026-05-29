qt_add_executable(vss_dashboard WIN32
    main.cpp
)

qt_add_qml_module(vss_dashboard
    URI Dashboard
    QML_FILES
        Main.qml
)

target_link_libraries(vss_dashboard PRIVATE
    covesavss_vehicle_frontend
    covesavss_powertrain_frontend
    covesavss_exterior_frontend
    Qt::Quick
)

# Tell qmlls / Qt Creator where the generated QML modules live
set(QML_IMPORT_PATH
    ${CMAKE_BINARY_DIR}/src/vehicle
    ${CMAKE_BINARY_DIR}/src/powertrain
    ${CMAKE_BINARY_DIR}/src/exterior
    CACHE STRING "" FORCE
)
