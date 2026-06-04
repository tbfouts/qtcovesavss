#include "bodybackends.h"
#include "../kuksaclient.h"
#include "../vsspathmapping.h"

#include <common.h>

// ===========================================================================
// BodyControlKuksaBackend
// ===========================================================================

BodyControlKuksaBackend::BodyControlKuksaBackend(KuksaClient *client, QObject *parent)
    : BodyControlBackendInterface(parent)
    , m_client(client)
{
    m_client->registerBackend(QStringLiteral("COVESA.VSS.Body.BodyControl"),
        [this](const QString &property, const QString &zone, const QVariant &value) {
            onVssValue(property, zone, value);
        });
}

void BodyControlKuksaBackend::initialize()
{
    emit initializationDone();
}

void BodyControlKuksaBackend::setIsTrunkLocked(bool isTrunkLocked)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyControl"),
        QStringLiteral("isTrunkLocked"));
    m_client->actuate(path, isTrunkLocked);
}

void BodyControlKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("rearMainSpoilerPosition"))
        emit rearMainSpoilerPositionChanged(value.toInt());
    else if (property == QLatin1String("turnSignalState"))
        emit turnSignalStateChanged(static_cast<Common::TurnSignalState>(value.toInt()));
    else if (property == QLatin1String("isHoodOpen"))
        emit isHoodOpenChanged(value.toBool());
    else if (property == QLatin1String("isTrunkOpen"))
        emit isTrunkOpenChanged(value.toBool());
    else if (property == QLatin1String("isTrunkLocked"))
        emit isTrunkLockedChanged(value.toBool());
    else if (property == QLatin1String("rainIntensity"))
        emit rainIntensityChanged(value.toReal());
}

// ===========================================================================
// BodyLightsKuksaBackend
// ===========================================================================

BodyLightsKuksaBackend::BodyLightsKuksaBackend(KuksaClient *client, QObject *parent)
    : BodyLightsBackendInterface(parent)
    , m_client(client)
{
    m_client->registerBackend(QStringLiteral("COVESA.VSS.Body.BodyLights"),
        [this](const QString &property, const QString &zone, const QVariant &value) {
            onVssValue(property, zone, value);
        });
}

void BodyLightsKuksaBackend::initialize()
{
    emit initializationDone();
}

void BodyLightsKuksaBackend::setLightSwitch(Common::LightSwitch lightSwitch)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyLights"),
        QStringLiteral("lightSwitch"));
    m_client->actuate(path, static_cast<int>(lightSwitch));
}

void BodyLightsKuksaBackend::setIsHighBeamOn(bool isHighBeamOn)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyLights"),
        QStringLiteral("isHighBeamOn"));
    m_client->actuate(path, isHighBeamOn);
}

void BodyLightsKuksaBackend::setIsLowBeamOn(bool isLowBeamOn)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyLights"),
        QStringLiteral("isLowBeamOn"));
    m_client->actuate(path, isLowBeamOn);
}

void BodyLightsKuksaBackend::setIsFogFrontOn(bool isFogFrontOn)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyLights"),
        QStringLiteral("isFogFrontOn"));
    m_client->actuate(path, isFogFrontOn);
}

void BodyLightsKuksaBackend::setIsFogRearOn(bool isFogRearOn)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyLights"),
        QStringLiteral("isFogRearOn"));
    m_client->actuate(path, isFogRearOn);
}

void BodyLightsKuksaBackend::setIsParkingOn(bool isParkingOn)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyLights"),
        QStringLiteral("isParkingOn"));
    m_client->actuate(path, isParkingOn);
}

void BodyLightsKuksaBackend::setIsHazardOn(bool isHazardOn)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyLights"),
        QStringLiteral("isHazardOn"));
    m_client->actuate(path, isHazardOn);
}

void BodyLightsKuksaBackend::setInteriorBrightness(int interiorBrightness)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyLights"),
        QStringLiteral("interiorBrightness"));
    m_client->actuate(path, interiorBrightness);
}

void BodyLightsKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("lightSwitch"))
        emit lightSwitchChanged(static_cast<Common::LightSwitch>(value.toInt()));
    else if (property == QLatin1String("isHighBeamOn"))
        emit isHighBeamOnChanged(value.toBool());
    else if (property == QLatin1String("isLowBeamOn"))
        emit isLowBeamOnChanged(value.toBool());
    else if (property == QLatin1String("isFogFrontOn"))
        emit isFogFrontOnChanged(value.toBool());
    else if (property == QLatin1String("isFogRearOn"))
        emit isFogRearOnChanged(value.toBool());
    else if (property == QLatin1String("isRunningOn"))
        emit isRunningOnChanged(value.toBool());
    else if (property == QLatin1String("isParkingOn"))
        emit isParkingOnChanged(value.toBool());
    else if (property == QLatin1String("isBrakeOn"))
        emit isBrakeOnChanged(value.toBool());
    else if (property == QLatin1String("isHazardOn"))
        emit isHazardOnChanged(value.toBool());
    else if (property == QLatin1String("isBackupOn"))
        emit isBackupOnChanged(value.toBool());
    else if (property == QLatin1String("interiorBrightness"))
        emit interiorBrightnessChanged(value.toInt());
}

// ===========================================================================
// WindshieldKuksaBackend (zoned: Front, Rear)
// ===========================================================================

WindshieldKuksaBackend::WindshieldKuksaBackend(KuksaClient *client, QObject *parent)
    : WindshieldBackendInterface(parent)
    , m_client(client)
{
    m_client->registerBackend(QStringLiteral("COVESA.VSS.Body.Windshield"),
        [this](const QString &property, const QString &zone, const QVariant &value) {
            onVssValue(property, zone, value);
        });
}

void WindshieldKuksaBackend::initialize()
{
    emit initializationDone();
}

QStringList WindshieldKuksaBackend::availableZones() const
{
    return VssPathMapping::zones(QStringLiteral("COVESA.VSS.Body.Windshield"));
}

void WindshieldKuksaBackend::setWiperMode(Common::WiperMode wiperMode, const QString &zone)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.Windshield"),
        QStringLiteral("wiperMode"), zone);
    m_client->actuate(path, static_cast<int>(wiperMode));
}

void WindshieldKuksaBackend::setIsHeatingOn(bool isHeatingOn, const QString &zone)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.Windshield"),
        QStringLiteral("isHeatingOn"), zone);
    m_client->actuate(path, isHeatingOn);
}

void WindshieldKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    if (property == QLatin1String("wiperMode"))
        emit wiperModeChanged(static_cast<Common::WiperMode>(value.toInt()), zone);
    else if (property == QLatin1String("wiperWearLevel"))
        emit wiperWearLevelChanged(value.toInt(), zone);
    else if (property == QLatin1String("isWiperFluidLevelLow"))
        emit isWiperFluidLevelLowChanged(value.toBool(), zone);
    else if (property == QLatin1String("isHeatingOn"))
        emit isHeatingOnChanged(value.toBool(), zone);
    else if (property == QLatin1String("isWindshieldHeatingOn"))
        emit isWindshieldHeatingOnChanged(value.toBool(), zone);
}

// ===========================================================================
// BodyMirrorsKuksaBackend (zoned: Left, Right)
// ===========================================================================

BodyMirrorsKuksaBackend::BodyMirrorsKuksaBackend(KuksaClient *client, QObject *parent)
    : BodyMirrorsBackendInterface(parent)
    , m_client(client)
{
    m_client->registerBackend(QStringLiteral("COVESA.VSS.Body.BodyMirrors"),
        [this](const QString &property, const QString &zone, const QVariant &value) {
            onVssValue(property, zone, value);
        });
}

void BodyMirrorsKuksaBackend::initialize()
{
    emit initializationDone();
}

QStringList BodyMirrorsKuksaBackend::availableZones() const
{
    return VssPathMapping::zones(QStringLiteral("COVESA.VSS.Body.BodyMirrors"));
}

void BodyMirrorsKuksaBackend::setTilt(int tilt, const QString &zone)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyMirrors"),
        QStringLiteral("tilt"), zone);
    m_client->actuate(path, tilt);
}

void BodyMirrorsKuksaBackend::setPan(int pan, const QString &zone)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyMirrors"),
        QStringLiteral("pan"), zone);
    m_client->actuate(path, pan);
}

void BodyMirrorsKuksaBackend::setIsHeatingOn(bool isHeatingOn, const QString &zone)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyMirrors"),
        QStringLiteral("isHeatingOn"), zone);
    m_client->actuate(path, isHeatingOn);
}

void BodyMirrorsKuksaBackend::setIsFolded(bool isFolded, const QString &zone)
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Body.BodyMirrors"),
        QStringLiteral("isFolded"), zone);
    m_client->actuate(path, isFolded);
}

void BodyMirrorsKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    if (property == QLatin1String("tilt"))
        emit tiltChanged(value.toInt(), zone);
    else if (property == QLatin1String("pan"))
        emit panChanged(value.toInt(), zone);
    else if (property == QLatin1String("isHeatingOn"))
        emit isHeatingOnChanged(value.toBool(), zone);
    else if (property == QLatin1String("isFolded"))
        emit isFoldedChanged(value.toBool(), zone);
}
