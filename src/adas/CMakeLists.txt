# Frontend - generates C++ classes + QML module
qt_ifcodegen_add_qml_module(covesavss_adas_frontend
    IDL_FILES ${CMAKE_SOURCE_DIR}/idl/adas.qface
    TEMPLATE frontend
    IDL_IMPORT_PATH ${CMAKE_SOURCE_DIR}/idl
)

target_link_libraries(covesavss_adas_frontend PUBLIC
    covesavss_common
)
