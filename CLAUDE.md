# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
# Configure (tested with Qt 6.10.2; requires InterfaceFramework module)
/Users/taylorfouts/Qt/6.10.2/macos/bin/qt-cmake -S . -B build -DBUILD_TESTING=ON

# Build everything
cmake --build build

# Run all tests (13 test suites: 12 frontend + 1 backend)
cd build && ctest --output-on-failure

# Run a single test
cd build && ./tests/vehicle/tst_vehicle
```

## Architecture

This library wraps the COVESA Vehicle Signal Specification (VSS) v6.0 as native Qt/QML types using Qt Interface Framework's `ifcodegen` code generator.

### Code Generation Pipeline

QFace IDL files (`idl/*.qface`) are processed by `ifcodegen` at CMake configure time to produce:
- **Frontend libraries**: C++ QObject classes with Q_PROPERTY for each signal, plus QML module registration

No hand-written C++ exists for the signal classes. All domain logic comes from the IDL files.

A KUKSA Databroker backend plugin (`src/backends/kuksa/`) connects to Eclipse KUKSA via gRPC to provide live VSS data. It is conditionally built when Qt Protobuf and Grpc modules are available. Backend plugins live under `src/backends/` to separate them from the frontend modules.

### Module Dependency Chain

```
Common (idl/common.qface)          -- shared enums only, no interfaces
  |
  v
12 Domain Modules (vehicle, powertrain, body, cabin, adas, chassis,
                   driver, safety, exterior, service, connectivity,
                   motionmanagement)
  |
  +-- Frontend lib (covesavss_<module>_frontend) links PUBLIC covesavss_common
```

### KUKSA Backend Plugin

Located in `src/backends/kuksa/`. A single Qt IF backend plugin that connects all 36 interfaces to a KUKSA Databroker via gRPC.

Key classes:
- **KuksaPlugin** (`kuksaplugin.h/cpp`): `QIfServiceInterface` implementation, creates all 36 backend objects and the gRPC client. Accepts `host`/`port` via `updateServiceSettings()`.
- **KuksaClient** (`kuksaclient.h/cpp`): Owns the gRPC channel (`QGrpcHttp2Channel`), subscribes to VSS paths, handles actuation requests. Reconnects with exponential backoff.
- **VssPathMapping** (`vsspathmapping.h/cpp`): Static bidirectional lookup table mapping `(IID, property, zone)` to VSS paths and back. ~200 entries.
- **Backend classes** (`backends/*.h/cpp`): 36 classes (24 non-zoned + 12 zoned) inheriting from generated `*BackendInterface` classes. Each connects to `KuksaClient::valueChanged`, filters by IID, and emits typed signals.

Proto files are vendored from `eclipse-kuksa/kuksa-databroker` in `src/backends/kuksa/proto/kuksa/val/v2/`. The import in `val.proto` was changed to `import "types.proto"` (relative) because Qt's `qt_add_protobuf` auto-adds the proto file directory as a protoc include path.

### CMake Target Patterns

Common module uses `qt_ifcodegen_extend_target` on a plain library (no QML module, just enums).

Domain modules use `qt_ifcodegen_add_qml_module` for the frontend (reads URI from qface `module` declaration). This requires `IDL_IMPORT_PATH ${CMAKE_SOURCE_DIR}/idl` so `import Common 1.0` resolves.

Tests use `qt_ifcodegen_extend_target` with `TEMPLATE test` on a dummy executable.

## QFace IDL Conventions

### Reserved Words

`model`, `map`, `signal`, `module`, `interface`, `struct`, `enum`, `flag`, `list`, `var`, `const`, `extends` are QFace reserved words. Properties cannot use these names. Use alternatives like `vehicleModel` instead of `model`, `manifoldAbsolutePressure` instead of `map`.

### Interface Naming

An interface name must not match the last segment of its module name. For example, module `COVESA.VSS.Powertrain` cannot have an interface named `Powertrain` because the generated header `powertrain.h` would include itself. Use `PowertrainStatus` instead.

### Common Module Import

The common module is declared as `module Common 1.0` (short name, not `COVESA.VSS.Common`) because the import prefix in QFace must exactly match the module name for type references like `Common.PowertrainType` to resolve.

### Enum Value Uniqueness

All enums in a QFace module are generated into a single C++ QObject class as plain `enum` (not `enum class`). Values must be unique across all enums in the module. Use type-specific prefixes (e.g., `PowertrainUnknown`, `FuelUnknown`, `GearUnknown` instead of `Unknown` repeated).

## Architectural and Naming Decisions

### Single Common module for all shared enums

All 30+ enums live in one `Common` QFace module rather than being split per domain (e.g., a separate `PowertrainEnums` module, `BodyEnums` module, etc.).

**Why**: QFace cross-module imports add a separate library dependency per import. Splitting enums across many modules would create a web of fine-grained library dependencies and multiply the number of CMake targets. A single common module keeps the dependency graph simple: every domain module depends on exactly one shared library.

**Ramification**: All enums share a single generated C++ QObject class, so enum value names must be globally unique within that class. This forced the use of type-specific prefixes on every value (`PowertrainUnknown`, `FuelUnknown`, `GearUnknown` rather than just `Unknown`). Adding a new enum requires checking all existing value names for collisions. If the enum count grows significantly, splitting into multiple common modules (e.g., `CommonPowertrain`, `CommonBody`) could reduce the collision surface at the cost of more library targets.

### Common module named `Common 1.0` instead of `COVESA.VSS.Common 1.0`

The common module uses the short name `Common` while all domain modules use the full `COVESA.VSS.<Name>` pattern.

**Why**: QFace type references must use the exact module name as a prefix. With `module COVESA.VSS.Common 1.0`, every property would need `COVESA.VSS.Common.PowertrainType` instead of `Common.PowertrainType`. The short name keeps property declarations readable.

**Ramification**: The common module does not get its own QML import URI (it uses `qt_ifcodegen_extend_target` on a plain library, not `qt_ifcodegen_add_qml_module`). Enums are not directly accessible from QML by import path. They are accessible through the domain module's generated code since the frontend links publicly against common. If direct QML access to common enums is needed in the future, a separate QML registration mechanism would be required.

### Interface named `PowertrainStatus` instead of `Powertrain`

The top-level interface in the Powertrain module is `PowertrainStatus`, not `Powertrain`.

**Why**: ifcodegen generates a header file named after the interface (`powertrain.h`), and a separate module-level header is also named after the module's last segment (`powertrain.h`). When both exist, the interface header includes the module header via `#include "powertrain.h"`, which resolves to itself instead, causing an infinite self-include. Renaming the interface breaks the collision.

**Ramification**: The QML type name is `PowertrainStatus` rather than the more natural `Powertrain`. This applies to any module where the "obvious" top-level interface name would match the module name. The same pattern would apply if someone added a `Body` interface to `COVESA.VSS.Body` or a `Chassis` interface to `COVESA.VSS.Chassis`. Those modules avoid this by not having a top-level summary interface with the module's name.

### One QFace file per VSS domain, not one giant file

Each VSS domain (Vehicle, Powertrain, Body, etc.) is a separate QFace module producing a separate QML import and separate shared library.

**Why**: A single monolithic QFace file would generate one massive library and one QML module. Separate modules let applications import only what they need (`import COVESA.VSS.Vehicle` without pulling in Cabin or ADAS), reducing binary size and load time. It also parallelizes the build since each module compiles independently.

**Ramification**: Applications that use types from multiple domains must link against multiple frontend libraries. Cross-domain type references are limited to what the Common module provides -- a domain module cannot directly reference an interface from another domain module.

### Interface granularity: 2-6 interfaces per module

Each module has multiple interfaces (e.g., Powertrain has PowertrainStatus, CombustionEngine, Transmission, ElectricMotor, TractionBattery, FuelSystem) rather than one monolithic interface per module.

**Why**: Each QFace interface becomes one QObject-derived C++ class and one QML type. A single interface with 100+ properties would be unwieldy. Splitting by VSS sub-branch keeps each class focused and lets QML code instantiate only the subsystem it needs.

**Ramification**: Each interface is an independent QML singleton requiring its own instantiation. An app showing engine RPM and battery SoC needs both `CombustionEngine { }` and `TractionBattery { }` rather than a single `Powertrain { }` object. This trades convenience for granularity.

### Zoned interfaces for VSS instances

VSS `instances` (e.g., Seat Row1Left/Row1Right, Wheel FrontLeft/FrontRight) are mapped to Qt Interface Framework's zone system via `@config: { zoned: true }` rather than creating separate interfaces per instance.

**Why**: Qt IF's zone system is purpose-built for this. It provides a single interface class with zone-aware property access, automatic backend routing per zone, and a consistent QML API (`CabinSeat { zone: "Row1Left" }`). Creating separate interfaces (e.g., `SeatRow1Left`, `SeatRow1Right`) would multiply the number of generated classes and lose the ability to iterate over zones programmatically.

### VSS sensor/actuator mapped to readonly/writable

VSS `sensor` and `attribute` types map to `readonly` QFace properties. VSS `actuator` types map to writable (no `readonly`) properties.

**Why**: This directly reflects VSS semantics. A speed sensor is read-only data from the vehicle; a charge limit is a value the user can set. Qt IF generates appropriate C++ accessors (getter-only vs getter+setter) and QML bindings based on this.

**Ramification**: The readonly/writable distinction is baked into the generated code at configure time. Changing a property from sensor to actuator (or vice versa) requires modifying the QFace file and reconfiguring CMake. There is no runtime mechanism to change property writability.
