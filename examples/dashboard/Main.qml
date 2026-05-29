import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import COVESA.VSS.Vehicle
import COVESA.VSS.Powertrain
import COVESA.VSS.Exterior

ApplicationWindow {
    id: root
    width: 1024
    height: 768
    visible: true
    title: "COVESA VSS Dashboard"

    // --- Vehicle module ---
    VehicleIdentification { id: vid }
    VehicleDynamics { id: dynamics }
    CurrentLocation { id: location }
    LowVoltageBattery { id: lvBattery }

    // --- Powertrain module ---
    PowertrainStatus { id: ptStatus }
    CombustionEngine { id: engine }
    Transmission { id: transmission }
    TractionBattery { id: hvBattery }
    FuelSystem { id: fuel }

    // --- Exterior module ---
    EnvironmentSensors { id: env }

    header: ToolBar {
        Label {
            text: "COVESA VSS Dashboard"
            font.pixelSize: 18
            font.bold: true
            anchors.centerIn: parent
        }
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            spacing: 12

            // ── Vehicle Identification ──────────────────────────
            GroupBox {
                title: "Vehicle Identification"
                Layout.fillWidth: true
                Layout.margins: 12
                GridLayout {
                    columns: 4
                    columnSpacing: 24
                    width: parent.width
                    Label { text: "VIN:"; font.bold: true }
                    Label { text: vid.vin || "—"; Layout.fillWidth: true }
                    Label { text: "Brand:"; font.bold: true }
                    Label { text: vid.brand || "—" }
                    Label { text: "Model:"; font.bold: true }
                    Label { text: vid.vehicleModel || "—" }
                    Label { text: "Year:"; font.bold: true }
                    Label { text: vid.year || "—" }
                    Label { text: "Body Type:"; font.bold: true }
                    Label { text: vid.bodyType || "—" }
                    Label { text: "Registered:"; font.bold: true }
                    Label { text: vid.dateVehicleFirstRegistered || "—" }
                    Label { text: "Emission Std:"; font.bold: true }
                    Label { text: vid.meetsEmissionStandard || "—" }
                    Label { text: "Special Usage:"; font.bold: true }
                    Label { text: vid.vehicleSpecialUsage || "—" }
                    Label { text: "Interior Color:"; font.bold: true }
                    Label { text: vid.vehicleInteriorColor || "—" }
                    Label { text: "Interior Type:"; font.bold: true }
                    Label { text: vid.vehicleInteriorType || "—" }
                }
            }

            // ── Vehicle Dynamics ────────────────────────────────
            GroupBox {
                title: "Vehicle Dynamics"
                Layout.fillWidth: true
                Layout.margins: 12
                GridLayout {
                    columns: 4
                    columnSpacing: 24
                    width: parent.width
                    Label { text: "Speed:"; font.bold: true }
                    Label { text: dynamics.speed.toFixed(1) + " km/h" }
                    Label { text: "Average Speed:"; font.bold: true }
                    Label { text: dynamics.averageSpeed.toFixed(1) + " km/h" }
                    Label { text: "Moving:"; font.bold: true }
                    Label { text: dynamics.isMoving ? "Yes" : "No" }
                    Label { text: "Weight:"; font.bold: true }
                    Label { text: dynamics.currentOverallWeight + " kg" }
                    Label { text: "Odometer:"; font.bold: true }
                    Label { text: dynamics.traveledDistance.toFixed(1) + " km" }
                    Label { text: "Trip:"; font.bold: true }
                    Label { text: dynamics.traveledDistanceSinceStart.toFixed(1) + " km" }
                    Label { text: "Trip Meter:"; font.bold: true }
                    Label { text: dynamics.tripMeterReading.toFixed(1) + " km" }
                    Item {} Item {}
                }
            }

            // ── Current Location ────────────────────────────────
            GroupBox {
                title: "Current Location"
                Layout.fillWidth: true
                Layout.margins: 12
                GridLayout {
                    columns: 4
                    columnSpacing: 24
                    width: parent.width
                    Label { text: "Latitude:"; font.bold: true }
                    Label { text: location.latitude.toFixed(6) + "°" }
                    Label { text: "Longitude:"; font.bold: true }
                    Label { text: location.longitude.toFixed(6) + "°" }
                    Label { text: "Altitude:"; font.bold: true }
                    Label { text: location.altitude.toFixed(1) + " m" }
                    Label { text: "Heading:"; font.bold: true }
                    Label { text: location.heading.toFixed(1) + "°" }
                    Label { text: "H. Accuracy:"; font.bold: true }
                    Label { text: location.horizontalAccuracy.toFixed(1) + " m" }
                    Label { text: "V. Accuracy:"; font.bold: true }
                    Label { text: location.verticalAccuracy.toFixed(1) + " m" }
                    Label { text: "GNSS Fix:"; font.bold: true }
                    Label { text: location.gnssFixType }
                    Label { text: "Timestamp:"; font.bold: true }
                    Label { text: location.timestamp || "—" }
                }
            }

            // ── 12V Battery ─────────────────────────────────────
            GroupBox {
                title: "Low Voltage Battery (12V)"
                Layout.fillWidth: true
                Layout.margins: 12
                GridLayout {
                    columns: 4
                    columnSpacing: 24
                    width: parent.width
                    Label { text: "Voltage:"; font.bold: true }
                    Label { text: lvBattery.currentVoltage.toFixed(2) + " V" }
                    Label { text: "Current:"; font.bold: true }
                    Label { text: lvBattery.currentCurrent.toFixed(2) + " A" }
                    Label { text: "Nominal Voltage:"; font.bold: true }
                    Label { text: lvBattery.nominalVoltage.toFixed(1) + " V" }
                    Label { text: "Capacity:"; font.bold: true }
                    Label { text: lvBattery.nominalCapacity + " Ah" }
                }
            }

            // ── Powertrain Status ───────────────────────────────
            GroupBox {
                title: "Powertrain Status"
                Layout.fillWidth: true
                Layout.margins: 12
                GridLayout {
                    columns: 4
                    columnSpacing: 24
                    width: parent.width
                    Label { text: "Type:"; font.bold: true }
                    Label { text: ptStatus.type }
                    Label { text: "Range:"; font.bold: true }
                    Label { text: ptStatus.range.toFixed(1) + " km" }
                    Label { text: "Regen Energy:"; font.bold: true }
                    Label { text: ptStatus.accumulatedBrakingEnergy.toFixed(1) + " kWh" }
                    Item {} Item {}
                }
            }

            // ── Combustion Engine ───────────────────────────────
            GroupBox {
                title: "Combustion Engine"
                Layout.fillWidth: true
                Layout.margins: 12
                GridLayout {
                    columns: 4
                    columnSpacing: 24
                    width: parent.width
                    Label { text: "RPM:"; font.bold: true }
                    Label { text: engine.speed }
                    Label { text: "Running:"; font.bold: true }
                    Label { text: engine.isRunning ? "Yes" : "No" }
                    Label { text: "Power:"; font.bold: true }
                    Label { text: engine.power.toFixed(1) + " kW" }
                    Label { text: "Torque:"; font.bold: true }
                    Label { text: engine.torque.toFixed(1) + " Nm" }
                    Label { text: "Coolant Temp:"; font.bold: true }
                    Label { text: engine.engineCoolantTemperature.toFixed(1) + " °C" }
                    Label { text: "Oil Temp:"; font.bold: true }
                    Label { text: engine.engineOilTemperature.toFixed(1) + " °C" }
                    Label { text: "Oil Pressure:"; font.bold: true }
                    Label { text: engine.engineOilPressure.toFixed(1) + " kPa" }
                    Label { text: "Oil Level:"; font.bold: true }
                    Label { text: engine.engineOilLevel + " %" }
                    Label { text: "Throttle:"; font.bold: true }
                    Label { text: engine.tps.toFixed(1) + " %" }
                    Label { text: "MAF:"; font.bold: true }
                    Label { text: engine.maf.toFixed(2) + " g/s" }
                    Label { text: "MAP:"; font.bold: true }
                    Label { text: engine.manifoldAbsolutePressure + " kPa" }
                    Label { text: "Engine Hours:"; font.bold: true }
                    Label { text: engine.engineHours + " h" }
                    Label { text: "Idle Hours:"; font.bold: true }
                    Label { text: engine.idleHours + " h" }
                    Label { text: "Displacement:"; font.bold: true }
                    Label { text: engine.displacement + " cc" }
                    Label { text: "Cylinders:"; font.bold: true }
                    Label { text: engine.numberOfCylinders }
                    Label { text: "Configuration:"; font.bold: true }
                    Label { text: engine.configuration || "—" }
                    Item {} Item {}
                }
            }

            // ── Transmission ────────────────────────────────────
            GroupBox {
                title: "Transmission"
                Layout.fillWidth: true
                Layout.margins: 12
                GridLayout {
                    columns: 4
                    columnSpacing: 24
                    width: parent.width
                    Label { text: "Current Gear:"; font.bold: true }
                    Label { text: transmission.currentGear }
                    Label { text: "Selected Gear:"; font.bold: true }
                    Label { text: transmission.selectedGear }
                    Label { text: "Gear Count:"; font.bold: true }
                    Label { text: transmission.gearCount }
                    Label { text: "Park Lock:"; font.bold: true }
                    Label { text: transmission.isParkLockEngaged ? "Engaged" : "Released" }
                    Label { text: "Temperature:"; font.bold: true }
                    Label { text: transmission.temperature.toFixed(1) + " °C" }
                    Label { text: "Clutch Wear:"; font.bold: true }
                    Label { text: transmission.clutchWear.toFixed(1) + " %" }
                    Label { text: "Distance:"; font.bold: true }
                    Label { text: transmission.travelledDistance.toFixed(1) + " km" }
                    Item {} Item {}
                }
            }

            // ── Traction Battery (HV) ──────────────────────────
            GroupBox {
                title: "Traction Battery (High Voltage)"
                Layout.fillWidth: true
                Layout.margins: 12
                GridLayout {
                    columns: 4
                    columnSpacing: 24
                    width: parent.width
                    Label { text: "State of Charge:"; font.bold: true }
                    Label { text: hvBattery.stateOfCharge.toFixed(1) + " %" }
                    Label { text: "State of Health:"; font.bold: true }
                    Label { text: hvBattery.stateOfHealth.toFixed(1) + " %" }
                    Label { text: "Voltage:"; font.bold: true }
                    Label { text: hvBattery.currentVoltage.toFixed(1) + " V" }
                    Label { text: "Current:"; font.bold: true }
                    Label { text: hvBattery.currentCurrent.toFixed(1) + " A" }
                    Label { text: "Power:"; font.bold: true }
                    Label { text: hvBattery.currentPower.toFixed(1) + " kW" }
                    Label { text: "Temperature:"; font.bold: true }
                    Label { text: hvBattery.temperature.toFixed(1) + " °C" }
                    Label { text: "Charging:"; font.bold: true }
                    Label { text: hvBattery.isCharging ? "Yes" : "No" }
                    Label { text: "Cable Connected:"; font.bold: true }
                    Label { text: hvBattery.isChargingCableConnected ? "Yes" : "No" }
                    Label { text: "Charge Rate:"; font.bold: true }
                    Label { text: hvBattery.chargingRate.toFixed(1) + " kW" }
                    Label { text: "Charge Limit:"; font.bold: true }
                    Label { text: hvBattery.chargeLimit.toFixed(0) + " %" }
                    Label { text: "Time to Complete:"; font.bold: true }
                    Label { text: hvBattery.timeToComplete.toFixed(0) + " min" }
                    Label { text: "Nominal Voltage:"; font.bold: true }
                    Label { text: hvBattery.nominalVoltage + " V" }
                    Label { text: "Max Voltage:"; font.bold: true }
                    Label { text: hvBattery.maxVoltage + " V" }
                    Label { text: "Gross Capacity:"; font.bold: true }
                    Label { text: hvBattery.grossCapacity + " kWh" }
                    Label { text: "Net Capacity:"; font.bold: true }
                    Label { text: hvBattery.netCapacity + " kWh" }
                    Label { text: "Charged Energy:"; font.bold: true }
                    Label { text: hvBattery.accumulatedChargedEnergy.toFixed(1) + " kWh" }
                    Label { text: "Consumed Energy:"; font.bold: true }
                    Label { text: hvBattery.accumulatedConsumedEnergy.toFixed(1) + " kWh" }
                }
            }

            // ── Fuel System ─────────────────────────────────────
            GroupBox {
                title: "Fuel System"
                Layout.fillWidth: true
                Layout.margins: 12
                GridLayout {
                    columns: 4
                    columnSpacing: 24
                    width: parent.width
                    Label { text: "Fuel Level:"; font.bold: true }
                    Label { text: fuel.fuelLevel.toFixed(1) + " %" }
                    Label { text: "Range:"; font.bold: true }
                    Label { text: fuel.range.toFixed(1) + " km" }
                    Label { text: "Instant Consumption:"; font.bold: true }
                    Label { text: fuel.instantConsumption.toFixed(2) + " L/100km" }
                    Label { text: "Avg Consumption:"; font.bold: true }
                    Label { text: fuel.averageConsumption.toFixed(2) + " L/100km" }
                    Label { text: "Since Start:"; font.bold: true }
                    Label { text: fuel.consumptionSinceStart.toFixed(2) + " L" }
                    Label { text: "Tank Capacity:"; font.bold: true }
                    Label { text: fuel.tankCapacity + " L" }
                    Label { text: "Stop/Start:"; font.bold: true }
                    Label { text: fuel.isEngineStopStartEnabled ? "Enabled" : "Disabled" }
                    Label { text: "Low Fuel:"; font.bold: true }
                    Label { text: fuel.isFuelLevelLow ? "YES" : "No" }
                }
            }

            // ── Environment ─────────────────────────────────────
            GroupBox {
                title: "Environment"
                Layout.fillWidth: true
                Layout.margins: 12
                GridLayout {
                    columns: 4
                    columnSpacing: 24
                    width: parent.width
                    Label { text: "Temperature:"; font.bold: true }
                    Label { text: env.ambientTemperature.toFixed(1) + " °C" }
                    Label { text: "Humidity:"; font.bold: true }
                    Label { text: env.humidity.toFixed(0) + " %" }
                    Label { text: "Pressure:"; font.bold: true }
                    Label { text: env.atmosphericPressure.toFixed(1) + " hPa" }
                    Label { text: "Visibility:"; font.bold: true }
                    Label { text: env.visibility.toFixed(0) + " m" }
                    Label { text: "Light Intensity:"; font.bold: true }
                    Label { text: env.lightIntensity.toFixed(0) + " lux" }
                    Label { text: "Wind Speed:"; font.bold: true }
                    Label { text: env.windSpeed.toFixed(1) + " m/s" }
                    Label { text: "Wind Direction:"; font.bold: true }
                    Label { text: env.windDirection.toFixed(0) + "°" }
                    Label { text: "Raining:"; font.bold: true }
                    Label { text: env.isRaining ? "Yes" : "No" }
                }
            }

            // bottom spacer
            Item { Layout.preferredHeight: 12 }
        }
    }
}
