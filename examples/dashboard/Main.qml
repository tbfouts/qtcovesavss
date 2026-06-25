import QtQuick
import COVESA.VSS.Vehicle
import COVESA.VSS.Powertrain

Window {
    id: root
    width: 1920
    height: 720
    visible: true
    title: "COVESA VSS Dashboard"
    color: "#dedede"

    readonly property real gaugeSize: height * 0.94

    // ── VSS data sources ────────────────────────────────────────
    VehicleDynamics { id: dynamics }
    CombustionEngine { id: engine }
    FuelSystem { id: fuel }

    // ── Speedometer (left) ──────────────────────────────────────
    CircularGauge {
        id: speedGauge
        width: root.gaugeSize
        height: root.gaugeSize
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 30

        value: dynamics.speed
        maxValue: 240
        majorStep: 20
        mediumStep: 10
        minorStep: 5
        labelDivisor: 1
        unitText: "km/h"
        rangeText: fuel.range > 0 ? fuel.range.toFixed(0) + " km" : ""
    }

    // ── Tachometer (right) ──────────────────────────────────────
    CircularGauge {
        id: rpmGauge
        width: root.gaugeSize
        height: root.gaugeSize
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 30

        value: engine.speed
        maxValue: 8000
        majorStep: 1000
        mediumStep: 500
        minorStep: 200
        labelDivisor: 1000
        unitText: "×1000 rpm"
        rangeText: ""
        readoutDivisor: 1000
    }
}
