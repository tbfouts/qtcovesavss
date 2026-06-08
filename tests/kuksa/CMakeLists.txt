set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Test Protobuf ProtobufWellKnownTypes Grpc)

set(KUKSA_SRC ${CMAKE_SOURCE_DIR}/src/backends/kuksa)

qt_add_executable(tst_kuksa
    tst_kuksabackends.cpp
    ${KUKSA_SRC}/kuksaclient.cpp
    ${KUKSA_SRC}/vsspathmapping.cpp
    ${KUKSA_SRC}/backends/vehiclebackends.cpp
    ${KUKSA_SRC}/backends/powertrainbackends.cpp
    ${KUKSA_SRC}/backends/bodybackends.cpp
    ${KUKSA_SRC}/backends/cabinbackends.cpp
    ${KUKSA_SRC}/backends/adasbackends.cpp
    ${KUKSA_SRC}/backends/chassisbackends.cpp
    ${KUKSA_SRC}/backends/driverbackends.cpp
    ${KUKSA_SRC}/backends/safetybackends.cpp
    ${KUKSA_SRC}/backends/exteriorbackends.cpp
    ${KUKSA_SRC}/backends/servicebackends.cpp
    ${KUKSA_SRC}/backends/connectivitybackends.cpp
    ${KUKSA_SRC}/backends/motionmanagementbackends.cpp
)

set(PROTO_DIR ${KUKSA_SRC}/proto/kuksa/val/v2)

qt_add_protobuf(tst_kuksa
    PROTO_FILES
        ${PROTO_DIR}/types.proto
        ${PROTO_DIR}/val.proto
)

qt_add_grpc(tst_kuksa CLIENT
    PROTO_FILES
        ${PROTO_DIR}/val.proto
)

target_include_directories(tst_kuksa PRIVATE
    ${KUKSA_SRC}
    ${KUKSA_SRC}/backends
)

target_link_libraries(tst_kuksa PRIVATE
    Qt::Core
    Qt::Test
    Qt::InterfaceFramework
    Qt::Protobuf
    Qt::ProtobufWellKnownTypes
    Qt::Grpc
    covesavss_vehicle_frontend
    covesavss_powertrain_frontend
    covesavss_body_frontend
    covesavss_cabin_frontend
    covesavss_adas_frontend
    covesavss_chassis_frontend
    covesavss_driver_frontend
    covesavss_safety_frontend
    covesavss_exterior_frontend
    covesavss_service_frontend
    covesavss_connectivity_frontend
    covesavss_motionmanagement_frontend
    covesavss_common
)

add_test(NAME tst_kuksa COMMAND tst_kuksa)
