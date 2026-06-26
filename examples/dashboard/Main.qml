import QtQuick
import COVESA.VSS.Vehicle
import COVESA.VSS.Powertrain

Window {
    id: root
    width: 1920
    height: 720
    visible: true
    title: "COVESA VSS Dashboard"
    color: "#1a1a1a"

    readonly property real mainGaugeSize: height * 0.94
    readonly property real tachSize: height * 0.7
    readonly property real sideGaugeSize: height * 0.52

    // ── VSS data sources ────────────────────────────────────────
    VehicleDynamics { id: dynamics }
    CombustionEngine { id: engine }
    FuelSystem { id: fuel }
    Transmission { id: transmission }

    // ── Gear label helper ───────────────────────────────────────
    readonly property int activeGearIndex: {
        var gear = transmission.currentGear;
        if (gear === 0 && transmission.isParkLockEngaged)
            return 0;  // P
        if (gear < 0)
            return 1;  // R
        if (gear === 0)
            return 2;  // N
        return 3;       // D
    }

    // ── Main horizontal layout ──────────────────────────────────
    Row {
        anchors.centerIn: parent
        spacing: 40

        // ── Left: Small tachometer ──────────────────────────────
        CircularGauge {
            id: rpmGauge
            width: root.tachSize
            height: root.tachSize
            anchors.verticalCenter: parent.verticalCenter

            value: engine.speed
            maxValue: 8000
            majorStep: 1000
            mediumStep: 500
            minorStep: 200
            labelDivisor: 1000
            unitText: "\u00d71000 rpm"
            rangeText: ""
            readoutDivisor: 1000
            redlineValue: 6500
        }

        // ── Center: Large speedometer ───────────────────────────
        CircularGauge {
            id: speedGauge
            width: root.mainGaugeSize
            height: root.mainGaugeSize
            anchors.verticalCenter: parent.verticalCenter

            value: dynamics.speed
            maxValue: 240
            majorStep: 20
            mediumStep: 10
            minorStep: 5
            labelDivisor: 1
            unitText: "km/h"
            rangeText: fuel.range > 0 ? Math.round(fuel.range) + " km" : ""
        }

        // ── Right: Fuel, Temp, PRNDL ────────────────────────────
        Column {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 24

            // Fuel gauge (partial arc)
            ArcGauge {
                id: fuelGauge
                width: root.sideGaugeSize * 0.8
                height: root.sideGaugeSize * 0.75

                value: fuel.fuelLevel
                minValue: 0
                maxValue: 100
                startLabel: "E"
                endLabel: "F"
                unitText: "FUEL"
            }

            // Coolant temp gauge (partial arc)
            ArcGauge {
                id: tempGauge
                width: root.sideGaugeSize * 0.8
                height: root.sideGaugeSize * 0.75

                value: engine.engineCoolantTemperature
                minValue: 50
                maxValue: 130
                startLabel: "C"
                endLabel: "H"
                unitText: "TEMP"
            }

            // PRNDL indicator
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 8

                Repeater {
                    model: ["P", "R", "N", "D"]

                    delegate: Rectangle {
                        required property int index
                        required property string modelData

                        width: 44
                        height: 44
                        radius: 6
                        color: index === root.activeGearIndex
                               ? "#E8531E" : "#333333"

                        Text {
                            anchors.centerIn: parent
                            text: modelData
                            font.pixelSize: 20
                            font.weight: Font.Bold
                            color: index === root.activeGearIndex
                                   ? "#ffffff" : "#777777"
                        }
                    }
                }
            }
        }
    }
}
