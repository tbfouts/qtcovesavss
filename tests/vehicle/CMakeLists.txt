set(CMAKE_AUTOMOC ON)

if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp")
    file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
endif()

qt_add_executable(tst_vehicle
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
)

qt_ifcodegen_extend_target(tst_vehicle
    IDL_FILES ${CMAKE_SOURCE_DIR}/idl/vehicle.qface
    TEMPLATE test
    IDL_IMPORT_PATH ${CMAKE_SOURCE_DIR}/idl
)

target_link_libraries(tst_vehicle PRIVATE
    covesavss_common
    covesavss_vehicle_frontend
    Qt::Test
)

add_test(NAME tst_vehicle COMMAND tst_vehicle)
