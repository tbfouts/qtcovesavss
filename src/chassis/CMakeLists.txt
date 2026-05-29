# Frontend - generates C++ classes + QML module
qt_ifcodegen_add_qml_module(covesavss_chassis_frontend
    IDL_FILES ${CMAKE_SOURCE_DIR}/idl/chassis.qface
    TEMPLATE frontend
    IDL_IMPORT_PATH ${CMAKE_SOURCE_DIR}/idl
)

target_link_libraries(covesavss_chassis_frontend PUBLIC
    covesavss_common
)
