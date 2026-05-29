set(CMAKE_AUTOMOC ON)

if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp")
    file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
endif()

qt_add_executable(tst_cabin
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
)

qt_ifcodegen_extend_target(tst_cabin
    IDL_FILES ${CMAKE_SOURCE_DIR}/idl/cabin.qface
    TEMPLATE test
    IDL_IMPORT_PATH ${CMAKE_SOURCE_DIR}/idl
)

target_link_libraries(tst_cabin PRIVATE
    covesavss_common
    covesavss_cabin_frontend
    Qt::Test
)

add_test(NAME tst_cabin COMMAND tst_cabin)
