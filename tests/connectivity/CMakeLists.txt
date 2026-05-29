set(CMAKE_AUTOMOC ON)

if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp")
    file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
endif()

qt_add_executable(tst_connectivity
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
)

qt_ifcodegen_extend_target(tst_connectivity
    IDL_FILES ${CMAKE_SOURCE_DIR}/idl/connectivity.qface
    TEMPLATE test
    IDL_IMPORT_PATH ${CMAKE_SOURCE_DIR}/idl
)

target_link_libraries(tst_connectivity PRIVATE
    covesavss_common
    covesavss_connectivity_frontend
    Qt::Test
)

add_test(NAME tst_connectivity COMMAND tst_connectivity)
