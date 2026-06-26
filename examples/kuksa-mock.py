#!/usr/bin/env python3
"""Minimal KUKSA Databroker mock that serves animated VSS signals.

Runs a gRPC server implementing just enough of the kuksa.val.v2.VAL service
for the COVESA VSS dashboard's KUKSA backend plugin to connect directly —
no real KUKSA Databroker or Docker required.

The plugin calls:
  1. ListMetadata  — returns metadata for the signals we animate
  2. Subscribe     — streams back animated values at ~10 Hz

Animated signals:
  - Vehicle.Speed                              (float, 0–100 km/h sine wave)
  - Vehicle.Powertrain.CombustionEngine.Speed  (int32, 800–5000 RPM sine wave)
  - Vehicle.Powertrain.CombustionEngine.ECT    (float, coolant temp ~85–95 C)
  - Vehicle.Powertrain.FuelSystem.Range        (float, 420→380 km slow decline)
  - Vehicle.Powertrain.FuelSystem.Level        (float, fuel level 0–100%)
  - Vehicle.Powertrain.Transmission.CurrentGear (int32, gear from speed)
  - Vehicle.Powertrain.Transmission.IsParkLockEngaged (bool)

Requirements:
    pip install grpcio grpcio-tools

Usage:
    python examples/kuksa-mock.py [--host HOST] [--port PORT]
"""

import argparse
import math
import os
import sys
import tempfile
import time
from concurrent import futures

# ---------------------------------------------------------------------------
# Dependency check
# ---------------------------------------------------------------------------
try:
    import grpc
    from grpc_tools import protoc
except ImportError:
    print(
        "Missing dependencies. Install them with:\n"
        "    pip install grpcio grpcio-tools",
        file=sys.stderr,
    )
    sys.exit(1)


def compile_protos():
    """Compile vendored KUKSA proto files and return the temp directory."""
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    proto_dir = os.path.join(repo_root, "src", "plugins", "kuksa", "proto",
                             "kuksa", "val", "v2")

    if not os.path.isdir(proto_dir):
        print(f"Proto directory not found: {proto_dir}", file=sys.stderr)
        sys.exit(1)

    out_dir = tempfile.mkdtemp(prefix="kuksa_proto_")

    import grpc_tools as _gt
    well_known = os.path.join(os.path.dirname(_gt.__file__), "_proto")

    for proto_file in ("types.proto", "val.proto"):
        result = protoc.main([
            "grpc_tools.protoc",
            f"--proto_path={proto_dir}",
            f"--proto_path={well_known}",
            f"--python_out={out_dir}",
            f"--grpc_python_out={out_dir}",
            os.path.join(proto_dir, proto_file),
        ])
        if result != 0:
            print(f"protoc failed for {proto_file} (exit {result})",
                  file=sys.stderr)
            sys.exit(1)

    return out_dir


# ---------------------------------------------------------------------------
# Signal definitions
# ---------------------------------------------------------------------------
SIGNALS = {
    "Vehicle.Speed": {
        "data_type": 11,   # DATA_TYPE_FLOAT
        "entry_type": 2,   # ENTRY_TYPE_SENSOR
        "description": "Vehicle speed.",
        "unit": "km/h",
    },
    "Vehicle.Powertrain.CombustionEngine.Speed": {
        "data_type": 8,    # DATA_TYPE_UINT16 (mapped to uint32 in proto)
        "entry_type": 2,   # ENTRY_TYPE_SENSOR
        "description": "Engine speed measured as rotations per minute.",
        "unit": "rpm",
    },
    "Vehicle.Powertrain.CombustionEngine.ECT": {
        "data_type": 11,   # DATA_TYPE_FLOAT
        "entry_type": 2,   # ENTRY_TYPE_SENSOR
        "description": "Engine coolant temperature.",
        "unit": "celsius",
    },
    "Vehicle.Powertrain.FuelSystem.Level": {
        "data_type": 11,   # DATA_TYPE_FLOAT
        "entry_type": 2,   # ENTRY_TYPE_SENSOR
        "description": "Fuel level as a percent of capacity. 0 = empty. 100 = full.",
        "unit": "percent",
    },
    "Vehicle.Powertrain.FuelSystem.Range": {
        "data_type": 11,   # DATA_TYPE_FLOAT
        "entry_type": 2,   # ENTRY_TYPE_SENSOR
        "description": "Remaining range with current fuel level.",
        "unit": "km",
    },
    "Vehicle.Powertrain.Transmission.CurrentGear": {
        "data_type": 5,    # DATA_TYPE_INT32
        "entry_type": 2,   # ENTRY_TYPE_SENSOR
        "description": "Current gear. 0=Neutral, -1=Reverse, 1/2/...=Forward.",
        "unit": "",
    },
    "Vehicle.Powertrain.Transmission.IsParkLockEngaged": {
        "data_type": 1,    # DATA_TYPE_BOOL
        "entry_type": 2,   # ENTRY_TYPE_SENSOR
        "description": "Is the park lock engaged.",
        "unit": "",
    },
}


def compute_values(t):
    """Return current animated values for time t (seconds)."""
    period = 10.0
    phase = (t % period) / period * 2 * math.pi

    speed = 50.0 + 50.0 * math.sin(phase)
    rpm = 2900 + 2100 * math.sin(phase)

    # Coolant temp: oscillates gently around 90 C (normal operating range)
    coolant_temp = 90.0 + 5.0 * math.sin(phase * 0.3)

    # Fuel level: slow decline 75% → 45% over ~200s, then wraps
    fuel_phase = (t % 200.0) / 200.0
    fuel_level = 75.0 - 30.0 * fuel_phase

    # Slow decline: 420 → 380 over ~100s, then wraps
    range_phase = (t % 100.0) / 100.0
    range_val = 420.0 - 40.0 * range_phase

    # Gear derived from speed — park when stopped, drive gears otherwise
    if speed < 2.0:
        gear = 0        # Neutral (park-like at standstill)
        is_park = True
    elif speed < 20.0:
        gear = 1
        is_park = False
    elif speed < 45.0:
        gear = 2
        is_park = False
    elif speed < 70.0:
        gear = 3
        is_park = False
    elif speed < 85.0:
        gear = 4
        is_park = False
    else:
        gear = 5
        is_park = False

    return {
        "Vehicle.Speed": speed,
        "Vehicle.Powertrain.CombustionEngine.Speed": rpm,
        "Vehicle.Powertrain.CombustionEngine.ECT": coolant_temp,
        "Vehicle.Powertrain.FuelSystem.Level": fuel_level,
        "Vehicle.Powertrain.FuelSystem.Range": range_val,
        "Vehicle.Powertrain.Transmission.CurrentGear": gear,
        "Vehicle.Powertrain.Transmission.IsParkLockEngaged": is_park,
    }


def main():
    parser = argparse.ArgumentParser(
        description="Mock KUKSA Databroker serving animated VSS signals")
    parser.add_argument("--host", default="0.0.0.0",
                        help="Listen address (default: 0.0.0.0)")
    parser.add_argument("--port", default=55555, type=int,
                        help="Listen port (default: 55555)")
    args = parser.parse_args()

    # Compile protos and import generated modules
    out_dir = compile_protos()
    sys.path.insert(0, out_dir)

    import types_pb2 as types
    import val_pb2 as val
    import val_pb2_grpc as val_grpc

    # ------------------------------------------------------------------
    # VAL service implementation
    # ------------------------------------------------------------------
    class VALServicer(val_grpc.VALServicer):

        def ListMetadata(self, request, context):
            """Return metadata for our animated signals."""
            metadata_list = []
            for i, (path, info) in enumerate(SIGNALS.items()):
                meta = types.Metadata(
                    path=path,
                    id=i + 1,
                    data_type=info["data_type"],
                    entry_type=info["entry_type"],
                    description=info["description"],
                    unit=info["unit"],
                )
                metadata_list.append(meta)

            return val.ListMetadataResponse(metadata=metadata_list)

        def Subscribe(self, request, context):
            """Stream animated values for subscribed paths at ~10 Hz."""
            paths = list(request.signal_paths)
            # Filter to paths we know about
            active = [p for p in paths if p in SIGNALS]

            if not active:
                context.set_code(grpc.StatusCode.NOT_FOUND)
                context.set_details("None of the requested paths are available")
                return

            print(f"Client subscribed to: {', '.join(active)}")

            while context.is_active():
                t = time.monotonic()
                values = compute_values(t)

                entries = {}
                for path in active:
                    v = values[path]
                    if path == "Vehicle.Powertrain.CombustionEngine.Speed":
                        value = types.Value(uint32=int(v))
                    elif path == "Vehicle.Powertrain.Transmission.CurrentGear":
                        value = types.Value(int32=int(v))
                    elif path == "Vehicle.Powertrain.Transmission.IsParkLockEngaged":
                        value = types.Value(bool=v)
                    else:
                        value = types.Value(float=v)
                    entries[path] = types.Datapoint(value=value)

                yield val.SubscribeResponse(entries=entries)

                speed = values["Vehicle.Speed"]
                rpm = values["Vehicle.Powertrain.CombustionEngine.Speed"]
                ect = values["Vehicle.Powertrain.CombustionEngine.ECT"]
                flvl = values["Vehicle.Powertrain.FuelSystem.Level"]
                gear = values["Vehicle.Powertrain.Transmission.CurrentGear"]
                print(f"\rSpd:{speed:5.1f} | "
                      f"RPM:{int(rpm):5d} | "
                      f"ECT:{ect:4.1f} | "
                      f"Fuel:{flvl:4.1f}% | "
                      f"Gear:{gear}",
                      end="", flush=True)

                time.sleep(0.1)

            print("\nClient disconnected.")

        def GetValue(self, request, context):
            """Return current value for a single signal."""
            path = request.signal_id.path
            if path not in SIGNALS:
                context.set_code(grpc.StatusCode.NOT_FOUND)
                context.set_details(f"Signal not found: {path}")
                return val.GetValueResponse()

            values = compute_values(time.monotonic())
            v = values[path]
            if path == "Vehicle.Powertrain.CombustionEngine.Speed":
                value = types.Value(uint32=int(v))
            elif path == "Vehicle.Powertrain.Transmission.CurrentGear":
                value = types.Value(int32=int(v))
            elif path == "Vehicle.Powertrain.Transmission.IsParkLockEngaged":
                value = types.Value(bool=v)
            else:
                value = types.Value(float=v)

            return val.GetValueResponse(
                data_point=types.Datapoint(value=value))

        def GetServerInfo(self, request, context):
            return val.GetServerInfoResponse(
                name="kuksa-mock", version="0.1.0", commit_hash="mock")

    # ------------------------------------------------------------------
    # Start server
    # ------------------------------------------------------------------
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=4))
    val_grpc.add_VALServicer_to_server(VALServicer(), server)

    listen_addr = f"{args.host}:{args.port}"
    server.add_insecure_port(listen_addr)
    server.start()

    print(f"Mock KUKSA Databroker listening on {listen_addr}")
    print(f"Serving signals: {', '.join(SIGNALS.keys())}")
    print("Press Ctrl+C to stop.\n")

    try:
        server.wait_for_termination()
    except KeyboardInterrupt:
        print("\nShutting down.")
        server.stop(grace=1)


if __name__ == "__main__":
    main()
