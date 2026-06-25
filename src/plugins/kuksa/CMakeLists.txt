find_package(Qt6 REQUIRED COMPONENTS Protobuf ProtobufWellKnownTypes Grpc)

set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto/kuksa/val/v2)

# Backend plugin
qt_add_plugin(covesavss_kuksa_backend
    PLUGIN_TYPE interfaceframework
    CLASS_NAME KuksaPlugin
    kuksaplugin.h
    kuksaplugin.cpp
    kuksaclient.h
    kuksaclient.cpp
    vsspathmapping.h
    vsspathmapping.cpp
    backends/vehiclebackends.h
    backends/vehiclebackends.cpp
    backends/powertrainbackends.h
    backends/powertrainbackends.cpp
    backends/bodybackends.h
    backends/bodybackends.cpp
    backends/cabinbackends.h
    backends/cabinbackends.cpp
    backends/adasbackends.h
    backends/adasbackends.cpp
    backends/chassisbackends.h
    backends/chassisbackends.cpp
    backends/driverbackends.h
    backends/driverbackends.cpp
    backends/safetybackends.h
    backends/safetybackends.cpp
    backends/exteriorbackends.h
    backends/exteriorbackends.cpp
    backends/servicebackends.h
    backends/servicebackends.cpp
    backends/connectivitybackends.h
    backends/connectivitybackends.cpp
    backends/motionmanagementbackends.h
    backends/motionmanagementbackends.cpp
)

# Generate protobuf messages into the plugin target
qt_add_protobuf(covesavss_kuksa_backend
    PROTO_FILES
        ${PROTO_DIR}/types.proto
        ${PROTO_DIR}/val.proto
)

# Generate gRPC client stubs into the plugin target
qt_add_grpc(covesavss_kuksa_backend CLIENT
    PROTO_FILES
        ${PROTO_DIR}/val.proto
)

# Place the plugin where Qt IF can discover it during development builds:
#   <build>/plugins/interfaceframework/<plugin-lib>
set_target_properties(covesavss_kuksa_backend PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/interfaceframework"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/interfaceframework"
)

target_link_libraries(covesavss_kuksa_backend PRIVATE
    Qt::Core
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
