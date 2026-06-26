import QtQuick

Item {
    id: gauge

    property real value: 0
    property real minValue: 0
    property real maxValue: 100
    property string startLabel: ""     // e.g. "E" or "C"
    property string endLabel: ""       // e.g. "F" or "H"
    property string unitText: ""       // e.g. "FUEL"

    // Arc geometry — 150-degree sweep, open at the bottom
    readonly property real startAngle: -210    // 7-o'clock (degrees from 12-o'clock)
    readonly property real sweepAngle: 150     // to about 10-o'clock
    readonly property real cx: width / 2
    readonly property real cy: height * 0.55
    readonly property real arcRadius: width * 0.38

    function valueToAngle(v: real): real {
        var clamped = Math.max(minValue, Math.min(maxValue, v));
        var frac = (clamped - minValue) / (maxValue - minValue);
        return startAngle + frac * sweepAngle;
    }

    // ── Background arc track ──────────────────────────────────
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            var startRad = (gauge.startAngle - 90) * Math.PI / 180;
            var endRad   = (gauge.startAngle + gauge.sweepAngle - 90) * Math.PI / 180;

            // Dark track
            ctx.beginPath();
            ctx.arc(gauge.cx, gauge.cy, gauge.arcRadius, startRad, endRad);
            ctx.strokeStyle = "#444444";
            ctx.lineWidth = 8;
            ctx.lineCap = "round";
            ctx.stroke();
        }
    }

    // ── Filled arc (value portion) ────────────────────────────
    Canvas {
        id: valueArc
        anchors.fill: parent

        property real displayValue: gauge.value
        Behavior on displayValue {
            NumberAnimation { duration: 600; easing.type: Easing.OutCubic }
        }

        onDisplayValueChanged: requestPaint()
        Component.onCompleted: requestPaint()

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            var startRad = (gauge.startAngle - 90) * Math.PI / 180;
            var valAngle = gauge.valueToAngle(displayValue);
            var endRad   = (valAngle - 90) * Math.PI / 180;

            if (Math.abs(endRad - startRad) < 0.01)
                return;

            ctx.beginPath();
            ctx.arc(gauge.cx, gauge.cy, gauge.arcRadius, startRad, endRad);
            ctx.strokeStyle = "#E8531E";
            ctx.lineWidth = 8;
            ctx.lineCap = "round";
            ctx.stroke();
        }
    }

    // ── Needle ────────────────────────────────────────────────
    Item {
        id: needle
        readonly property real needleLen: gauge.arcRadius + 12

        width: 4
        height: needleLen
        x: gauge.cx - width / 2
        y: gauge.cy - needleLen
        transformOrigin: Item.Bottom
        rotation: gauge.valueToAngle(gauge.value)

        Behavior on rotation {
            NumberAnimation { duration: 600; easing.type: Easing.OutCubic }
        }

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "#E8531E"
        }
    }

    // ── Center hub ────────────────────────────────────────────
    Rectangle {
        width: 8; height: 8; radius: 4
        color: "#E8531E"
        x: gauge.cx - 4
        y: gauge.cy - 4
    }

    // ── Start label (e.g. "E" or "C") ─────────────────────────
    Text {
        visible: gauge.startLabel.length > 0
        text: gauge.startLabel
        font.pixelSize: gauge.width * 0.10
        font.weight: Font.Bold
        color: "#888888"

        readonly property real angleDeg: gauge.startAngle
        readonly property real rad: (angleDeg - 90) * Math.PI / 180
        readonly property real labelR: gauge.arcRadius + 22

        x: gauge.cx + labelR * Math.cos(rad) - width / 2
        y: gauge.cy + labelR * Math.sin(rad) - height / 2
    }

    // ── End label (e.g. "F" or "H") ──────────────────────────
    Text {
        visible: gauge.endLabel.length > 0
        text: gauge.endLabel
        font.pixelSize: gauge.width * 0.10
        font.weight: Font.Bold
        color: "#888888"

        readonly property real angleDeg: gauge.startAngle + gauge.sweepAngle
        readonly property real rad: (angleDeg - 90) * Math.PI / 180
        readonly property real labelR: gauge.arcRadius + 22

        x: gauge.cx + labelR * Math.cos(rad) - width / 2
        y: gauge.cy + labelR * Math.sin(rad) - height / 2
    }

    // ── Unit label ────────────────────────────────────────────
    Text {
        visible: gauge.unitText.length > 0
        text: gauge.unitText
        font.pixelSize: gauge.width * 0.10
        font.weight: Font.DemiBold
        color: "#666666"
        anchors.horizontalCenter: parent.horizontalCenter
        y: gauge.cy + gauge.arcRadius * 0.45
    }
}
