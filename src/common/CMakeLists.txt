qt_add_library(covesavss_common)

qt_ifcodegen_extend_target(covesavss_common
    IDL_FILES ${CMAKE_SOURCE_DIR}/idl/common.qface
    TEMPLATE frontend
)
