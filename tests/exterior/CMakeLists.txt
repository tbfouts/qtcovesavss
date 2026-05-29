set(CMAKE_AUTOMOC ON)

if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp")
    file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
endif()

qt_add_executable(tst_exterior
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
)

qt_ifcodegen_extend_target(tst_exterior
    IDL_FILES ${CMAKE_SOURCE_DIR}/idl/exterior.qface
    TEMPLATE test
    IDL_IMPORT_PATH ${CMAKE_SOURCE_DIR}/idl
)

target_link_libraries(tst_exterior PRIVATE
    covesavss_common
    covesavss_exterior_frontend
    Qt::Test
)

add_test(NAME tst_exterior COMMAND tst_exterior)
