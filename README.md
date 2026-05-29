# COVESA VSS Qt Library

A Qt library wrapping the COVESA Vehicle Signal Specification (VSS) v6.0 as native Qt/QML types. Built with Qt Interface Framework and ifcodegen for automatic code generation from QFace IDL files.

## Overview

This library provides 600+ vehicle data signals organized into 12 QML modules, each mapping to a VSS domain. Every signal is exposed as a `Q_PROPERTY` on a QObject-derived class, usable directly from both C++ and QML. This is a frontend-only library — backend plugins that provide actual data (from KUKSA Databroker, CAN bus, SOME/IP, etc.) must be provided separately.

## Requirements

- **Qt 6.10+** with the following modules:
  - Qt Core
  - Qt Interface Framework (requires Qt for Device Creation license or building Qt IF from source)
  - Qt Qml
  - Qt Quick
- CMake 3.16+
- C++17 compiler

> **Note:** Qt Interface Framework is not included in the default open-source Qt installation. It is available with a Qt for Device Creation license, or can be built from source from the [qt/qtinterfaceframework](https://code.qt.io/cgit/qt/qtinterfaceframework.git/) repository.

## Getting Started

### Clone

```sh
git clone <repository-url> covesavss
cd covesavss
```

### Build

Use `qt-cmake` from your Qt installation to configure and build:

```sh
# macOS
/path/to/Qt/6.10.2/macos/bin/qt-cmake -S . -B build

# Linux
/path/to/Qt/6.10.2/gcc_64/bin/qt-cmake -S . -B build

# Windows (from a Qt command prompt)
C:\Qt\6.10.2\msvc2022_64\bin\qt-cmake -S . -B build
```

Then build:

```sh
cmake --build build
```

### Run Tests

```sh
qt-cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
cd build && ctest --output-on-failure
```

## Using in Your Qt Application

### CMake Integration

The recommended way to consume this library is via `add_subdirectory`. In your project's `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)
project(myapp LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick InterfaceFramework)
qt_standard_project_setup(REQUIRES 6.10)

# Add the COVESA VSS library (adjust path to where you cloned it)
add_subdirectory(third_party/covesavss)

qt_add_executable(myapp main.cpp)
qt_add_qml_module(myapp URI MyApp QML_FILES Main.qml)

# Link the frontend libraries for the modules you need
target_link_libraries(myapp PRIVATE
    covesavss_vehicle_frontend
    covesavss_powertrain_frontend
    covesavss_cabin_frontend
    Qt::Quick
)
```

Each domain module produces a frontend target named `covesavss_<module>_frontend`:

| Module | CMake Target |
|---|---|
| Vehicle | `covesavss_vehicle_frontend` |
| Powertrain | `covesavss_powertrain_frontend` |
| Body | `covesavss_body_frontend` |
| Cabin | `covesavss_cabin_frontend` |
| ADAS | `covesavss_adas_frontend` |
| Chassis | `covesavss_chassis_frontend` |
| Driver | `covesavss_driver_frontend` |
| Safety | `covesavss_safety_frontend` |
| Exterior | `covesavss_exterior_frontend` |
| Service | `covesavss_service_frontend` |
| Connectivity | `covesavss_connectivity_frontend` |
| MotionManagement | `covesavss_motionmanagement_frontend` |

The shared `covesavss_common` library (enums) is linked automatically via each frontend's `PUBLIC` dependency.

### Backend Plugins

This library provides only frontend types (QObject classes and QML modules). At runtime, Qt Interface Framework discovers backend plugins by scanning for an `interfaceframework/` directory adjacent to the application binary.

You must provide a backend plugin that implements the Qt IF backend interface for the modules you use. Options include:

- **[Eclipse KUKSA Databroker](https://github.com/eclipse-kuksa/kuksa-databroker)** — A gRPC-based VSS server with simulation tools like [kuksa-mock-provider](https://github.com/eclipse-kuksa/kuksa-mock-provider) and [kuksa-csv-provider](https://github.com/eclipse-kuksa/kuksa-csv-provider). Write a Qt IF backend plugin that connects to KUKSA's gRPC API.
- **Custom backend** — Implement a Qt IF backend plugin that connects to your vehicle's data bus (CAN, SOME/IP, etc.).

Place your backend plugin `.so`/`.dylib`/`.dll` files in an `interfaceframework/` directory next to your application binary.

### QML Usage

Import the modules you need and instantiate interface types:

```qml
import QtQuick
import QtQuick.Controls
import COVESA.VSS.Vehicle
import COVESA.VSS.Powertrain

ApplicationWindow {
    VehicleDynamics { id: dynamics }
    TractionBattery { id: battery }

    Text {
        text: "Speed: " + dynamics.speed.toFixed(1) + " km/h"
    }
    Text {
        text: "Battery: " + battery.stateOfCharge.toFixed(0) + "%"
    }
}
```

Zoned interfaces support Qt Interface Framework's zone model for multi-instance VSS branches (seats, wheels, doors):

```qml
import COVESA.VSS.Cabin

CabinHVAC {
    id: hvac
    zone: "FrontLeft"
}
Text { text: "Driver temp: " + hvac.targetTemperature }
```

### C++ Usage

The generated headers follow the pattern `<lowercaseinterfacename>.h`. Include them and use the types directly:

```cpp
#include <vehicledynamics.h>
#include <tractionbattery.h>

auto *dynamics = new VehicleDynamics(this);
qDebug() << "Speed:" << dynamics->speed();

auto *battery = new TractionBattery(this);
qDebug() << "SoC:" << battery->stateOfCharge();
```

Common enums are on the generated `Common` class:

```cpp
#include <common.h>

// Enum values use type-specific prefixes to avoid collisions
Common::PowertrainElectric  // PowertrainType enum
Common::FuelGasoline        // FuelType enum
Common::ChargeActive        // ChargingStatus enum
```

## QML Modules

| QML Import URI | VSS Domain | Interfaces |
|---|---|---|
| `COVESA.VSS.Vehicle` | Top-level vehicle signals | VehicleIdentification, VehicleDynamics, CurrentLocation, LowVoltageBattery |
| `COVESA.VSS.Powertrain` | Powertrain subsystems | PowertrainStatus, CombustionEngine, Transmission, ElectricMotor (zoned), TractionBattery, FuelSystem |
| `COVESA.VSS.Body` | Body, lights, mirrors | BodyControl, BodyLights, Windshield (zoned), BodyMirrors (zoned) |
| `COVESA.VSS.Cabin` | Seats, HVAC, infotainment | CabinSeat (zoned), CabinHVAC (zoned), CabinInfotainment, CabinDoor (zoned), CabinLights |
| `COVESA.VSS.ADAS` | Driver assist systems | ADASControl, CruiseControl, ObstacleDetection (zoned) |
| `COVESA.VSS.Chassis` | Steering, brakes, wheels | ChassisAxle (zoned), ChassisWheel (zoned), ChassisSteering, ChassisBrake |
| `COVESA.VSS.Driver` | Driver monitoring | DriverMonitoring |
| `COVESA.VSS.Safety` | Crash detection | CrashDetection, AirbagSystem (zoned), BeltSystem (zoned) |
| `COVESA.VSS.Exterior` | Environment sensors | EnvironmentSensors |
| `COVESA.VSS.Service` | Service status | ServiceStatus |
| `COVESA.VSS.Connectivity` | Connectivity status | ConnectivityControl |
| `COVESA.VSS.MotionManagement` | Brake/steering/suspension control | BrakeControl, SteeringControl, SuspensionControl (zoned) |

Shared enums are in the `Common` module, imported automatically by all domain modules.

## Architecture

Each module consists of:

- **QFace IDL file** (`idl/<module>.qface`) defining interfaces, properties, and type references
- **Frontend library** (auto-generated C++ classes + QML module via `qt_ifcodegen_add_qml_module`)

The `Common` module (`idl/common.qface`) defines shared enums used across all domains. Enum values use type-specific prefixes (e.g., `PowertrainElectric`, `FuelGasoline`) to avoid collisions within the generated QObject class.

## VSS-to-QFace Mapping

| VSS Concept | QFace Representation |
|---|---|
| `sensor` (read-only data) | `readonly` property |
| `actuator` (read-write control) | Property without `readonly` |
| `attribute` (static metadata) | `readonly` property |
| `allowed` values | `Common.<EnumType>` property type |
| `instances` (Row1/Row2, Left/Right) | `@config: { zoned: true }` interface annotation |

## Project Structure

```
idl/           QFace IDL files (source of truth)
src/common/    Shared enum library (covesavss_common)
src/<module>/  Per-module frontend library
tests/         Auto-generated test suite per module (12 suites)
examples/      Dashboard example app (requires a backend plugin to run)
```

## Known Limitations

- **No install target:** The project does not provide `cmake --install` support. Use `add_subdirectory` for integration.
- **Frontend only:** No backend plugins are included. You must provide a backend (e.g., KUKSA Databroker) for the dashboard example or any consuming application to receive data at runtime.

## License

This project is provided as a reference implementation for the COVESA VSS specification with Qt Interface Framework.
