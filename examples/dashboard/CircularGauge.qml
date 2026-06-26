import QtQuick

Item {
    id: gaugeRoot

    // ── Configurable properties ─────────────────────────────────
    property real value: 0
    property real maxValue: 120
    property real majorStep: 20        // bold tick every N units
    property real mediumStep: 10       // medium tick every N units
    property real minorStep: 2         // fine tick every N units
    property int  labelDivisor: 1      // divide label text (e.g. 1000 → "3" for 3000 rpm)
    property string unitText: "mph"
    property string rangeText: "450 mi"
    property bool showReadout: true
    property real readoutDivisor: 1    // 1 → "60", 1000 → "3.5"
    property real redlineValue: -1     // if >= 0, draw red arc from this value to maxValue
    property color redlineColor: "#E8531E"

    // ── Internal geometry ───────────────────────────────────────
    readonly property real startAngle: -150   // 0-value at 7-o'clock (degrees from 12-o'clock)
    readonly property real sweepAngle: 300    // total arc span in degrees
    readonly property real cx: width / 2
    readonly property real cy: height / 2
    readonly property real radius: width / 2

    function valueToAngle(v: real): real {
        return startAngle + (v / maxValue) * sweepAngle;
    }

    // Rebuild tick marks when any step/range property changes
    onMaxValueChanged:  tickCanvas.requestPaint()
    onMajorStepChanged: tickCanvas.requestPaint()
    onMediumStepChanged: tickCanvas.requestPaint()
    onMinorStepChanged: tickCanvas.requestPaint()

    // ── Drop shadow ─────────────────────────────────────────────
    Rectangle {
        width: gaugeRoot.width + 6
        height: gaugeRoot.height + 6
        radius: width / 2
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: 2
        anchors.verticalCenterOffset: 2
        color: "#b0b0b0"
    }

    // ── Gauge face (radial gradient via Canvas) ─────────────────
    Canvas {
        id: faceCanvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d");
            var cx = width / 2;
            var cy = height / 2;
            var r  = width / 2;

            var grad = ctx.createRadialGradient(cx, cy * 0.88, 0, cx, cy, r);
            grad.addColorStop(0.00, "#ece8e3");
            grad.addColorStop(0.70, "#e4e0db");
            grad.addColorStop(0.90, "#d6d2cd");
            grad.addColorStop(1.00, "#cac6c1");

            ctx.beginPath();
            ctx.arc(cx, cy, r - 1, 0, Math.PI * 2);
            ctx.fillStyle = grad;
            ctx.fill();

            // Subtle rim
            ctx.strokeStyle = "#b0aca7";
            ctx.lineWidth = 1;
            ctx.stroke();
        }
    }

    // ── Tick marks ──────────────────────────────────────────────
    Canvas {
        id: tickCanvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            var cx     = width / 2;
            var cy     = height / 2;
            var outerR = width / 2 - 8;

            var tickCount = Math.round(gaugeRoot.maxValue / gaugeRoot.minorStep);
            for (var i = 0; i <= tickCount; i++) {
                var v        = i * gaugeRoot.minorStep;
                var angleDeg = gaugeRoot.valueToAngle(v);
                var rad      = (angleDeg - 90) * Math.PI / 180;

                var len, w;
                if (v % gaugeRoot.majorStep === 0) {            // major
                    len = 22; w = 3.0;
                } else if (v % gaugeRoot.mediumStep === 0) {    // medium
                    len = 16; w = 2.0;
                } else {                                         // minor
                    len = 10; w = 1.5;
                }

                var ox = cx + outerR * Math.cos(rad);
                var oy = cy + outerR * Math.sin(rad);
                var ix = cx + (outerR - len) * Math.cos(rad);
                var iy = cy + (outerR - len) * Math.sin(rad);

                ctx.beginPath();
                ctx.moveTo(ox, oy);
                ctx.lineTo(ix, iy);
                ctx.strokeStyle = "#222222";
                ctx.lineWidth   = w;
                ctx.lineCap     = "butt";
                ctx.stroke();
            }
        }
    }

    // ── Numeric labels at major ticks ───────────────────────────
    Repeater {
        id: labelRepeater
        model: {
            var labels = [];
            for (var v = 0; v <= gaugeRoot.maxValue; v += gaugeRoot.majorStep)
                labels.push(v);
            return labels;
        }

        delegate: Text {
            required property var modelData

            readonly property real angleDeg: gaugeRoot.valueToAngle(modelData)
            readonly property real rad: (angleDeg - 90) * Math.PI / 180
            readonly property real labelR: gaugeRoot.radius * 0.77

            x: gaugeRoot.cx + labelR * Math.cos(rad) - width / 2
            y: gaugeRoot.cy + labelR * Math.sin(rad) - height / 2

            text: (modelData / gaugeRoot.labelDivisor).toString()
            font.pixelSize: gaugeRoot.width * 0.055
            font.weight: Font.Bold
            color: "#2a2a2a"
        }
    }

    // ── Arc: redline zone or decorative bottom arc ──────────────
    Canvas {
        id: arcCanvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            var cx   = width / 2;
            var cy   = height / 2;
            var arcR = (width / 2) - 12;

            var startRad, endRad, color;
            if (gaugeRoot.redlineValue >= 0) {
                // Redline arc from redlineValue to maxValue
                startRad = (gaugeRoot.valueToAngle(gaugeRoot.redlineValue) - 90) * Math.PI / 180;
                endRad   = (gaugeRoot.valueToAngle(gaugeRoot.maxValue) - 90) * Math.PI / 180;
                color = gaugeRoot.redlineColor;
            } else {
                // Decorative arc at bottom of dial
                startRad = (160 - 90) * Math.PI / 180;
                endRad   = (200 - 90) * Math.PI / 180;
                color = "#E8531E";
            }

            ctx.beginPath();
            ctx.arc(cx, cy, arcR, startRad, endRad);
            ctx.strokeStyle = color;
            ctx.lineWidth   = 5;
            ctx.lineCap     = "round";
            ctx.stroke();
        }
    }

    onRedlineValueChanged: arcCanvas.requestPaint()
    onRedlineColorChanged: arcCanvas.requestPaint()

    // ── Needle ──────────────────────────────────────────────────
    Item {
        id: needle

        readonly property real needleLength: gaugeRoot.radius - 32
        width: 6
        height: needleLength
        x: gaugeRoot.cx - width / 2
        y: gaugeRoot.cy - needleLength
        transformOrigin: Item.Bottom
        rotation: gaugeRoot.valueToAngle(gaugeRoot.value)

        Behavior on rotation {
            NumberAnimation {
                duration: 600
                easing.type: Easing.OutCubic
            }
        }

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "#E8531E"
        }
    }

    // ── Center hub ──────────────────────────────────────────────
    Rectangle {
        width: 12
        height: 12
        radius: 6
        color: "#E8531E"
        anchors.centerIn: parent
    }

    // ── Unit label (e.g. "mph", "×1000 rpm") ────────────────────
    Text {
        text: gaugeRoot.unitText
        font.pixelSize: gaugeRoot.width * 0.048
        color: "#888888"
        anchors.horizontalCenter: parent.horizontalCenter
        y: gaugeRoot.cy + gaugeRoot.radius * 0.17 - 30
    }

    // ── Large value readout ─────────────────────────────────────
    Text {
        visible: gaugeRoot.showReadout
        text: gaugeRoot.readoutDivisor > 1
              ? (gaugeRoot.value / gaugeRoot.readoutDivisor).toFixed(1)
              : Math.round(gaugeRoot.value).toString()
        font.pixelSize: gaugeRoot.width * 0.17
        font.weight: Font.Black
        color: "#1a1a1a"
        anchors.horizontalCenter: parent.horizontalCenter
        y: gaugeRoot.cy + gaugeRoot.radius * 0.32 - 30
    }

    // ── Range display (optional — hidden when empty) ────────────
    Text {
        visible: gaugeRoot.rangeText.length > 0
        text: gaugeRoot.rangeText
        font.pixelSize: gaugeRoot.width * 0.038
        color: "#999999"
        font.weight: Font.DemiBold
        anchors.horizontalCenter: parent.horizontalCenter
        y: gaugeRoot.cy + gaugeRoot.radius * 0.75
    }

}
