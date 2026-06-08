#include "chassisbackends.h"
#include "../kuksaclient.h"
#include "../vsspathmapping.h"

#include <common.h>

// ===========================================================================
// ChassisAxleKuksaBackend (zoned: Front, Rear)
// ===========================================================================

ChassisAxleKuksaBackend::ChassisAxleKuksaBackend(KuksaClient *client, QObject *parent)
    : ChassisAxleBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Chassis.ChassisAxle"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

ChassisAxleKuksaBackend::~ChassisAxleKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Chassis.ChassisAxle"));
}

void ChassisAxleKuksaBackend::initialize()
{
    emit initializationDone();
}

QStringList ChassisAxleKuksaBackend::availableZones() const
{
    static const QString iid = QStringLiteral("COVESA.VSS.Chassis.ChassisAxle");
    return VssPathMapping::zones(iid);
}

void ChassisAxleKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    if (property == QLatin1String("wheelCount"))
        emit wheelCountChanged(value.toInt(), zone);
    else if (property == QLatin1String("wheelDiameter"))
        emit wheelDiameterChanged(value.toReal(), zone);
    else if (property == QLatin1String("wheelWidth"))
        emit wheelWidthChanged(value.toReal(), zone);
    else if (property == QLatin1String("tireDiameter"))
        emit tireDiameterChanged(value.toReal(), zone);
    else if (property == QLatin1String("tireWidth"))
        emit tireWidthChanged(value.toReal(), zone);
    else if (property == QLatin1String("tireAspectRatio"))
        emit tireAspectRatioChanged(value.toInt(), zone);
}

// ===========================================================================
// ChassisWheelKuksaBackend (zoned: FrontLeft, FrontRight, RearLeft, RearRight)
// ===========================================================================

ChassisWheelKuksaBackend::ChassisWheelKuksaBackend(KuksaClient *client, QObject *parent)
    : ChassisWheelBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Chassis.ChassisWheel"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

ChassisWheelKuksaBackend::~ChassisWheelKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Chassis.ChassisWheel"));
}

void ChassisWheelKuksaBackend::initialize()
{
    emit initializationDone();
}

QStringList ChassisWheelKuksaBackend::availableZones() const
{
    static const QString iid = QStringLiteral("COVESA.VSS.Chassis.ChassisWheel");
    return VssPathMapping::zones(iid);
}

void ChassisWheelKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    if (property == QLatin1String("tirePressure"))
        emit tirePressureChanged(value.toReal(), zone);
    else if (property == QLatin1String("isPressureLow"))
        emit isPressureLowChanged(value.toBool(), zone);
    else if (property == QLatin1String("tireTemperature"))
        emit tireTemperatureChanged(value.toReal(), zone);
    else if (property == QLatin1String("brakeWear"))
        emit brakeWearChanged(value.toReal(), zone);
    else if (property == QLatin1String("isBrakeFluidLevelLow"))
        emit isBrakeFluidLevelLowChanged(value.toBool(), zone);
    else if (property == QLatin1String("speed"))
        emit speedChanged(value.toReal(), zone);
}

// ===========================================================================
// ChassisSteeringKuksaBackend
// ===========================================================================

ChassisSteeringKuksaBackend::ChassisSteeringKuksaBackend(KuksaClient *client, QObject *parent)
    : ChassisSteeringBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Chassis.ChassisSteering"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

ChassisSteeringKuksaBackend::~ChassisSteeringKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Chassis.ChassisSteering"));
}

void ChassisSteeringKuksaBackend::initialize()
{
    emit initializationDone();
}

void ChassisSteeringKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("steeringWheelAngle"))
        emit steeringWheelAngleChanged(value.toReal());
    else if (property == QLatin1String("steeringWheelTilt"))
        emit steeringWheelTiltChanged(value.toInt());
    else if (property == QLatin1String("steeringWheelExtension"))
        emit steeringWheelExtensionChanged(value.toInt());
    else if (property == QLatin1String("position"))
        emit positionChanged(static_cast<Common::SteeringWheelPosition>(value.toInt()));
}

// ===========================================================================
// ChassisBrakeKuksaBackend
// ===========================================================================

ChassisBrakeKuksaBackend::ChassisBrakeKuksaBackend(KuksaClient *client, QObject *parent)
    : ChassisBrakeBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Chassis.ChassisBrake"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

ChassisBrakeKuksaBackend::~ChassisBrakeKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Chassis.ChassisBrake"));
}

void ChassisBrakeKuksaBackend::initialize()
{
    emit initializationDone();
}

void ChassisBrakeKuksaBackend::setIsParkingBrakeEngaged(bool isParkingBrakeEngaged)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Chassis.ChassisBrake");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isParkingBrakeEngaged")), isParkingBrakeEngaged);
}

void ChassisBrakeKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("pedalPosition"))
        emit pedalPositionChanged(value.toInt());
    else if (property == QLatin1String("isDriverBraking"))
        emit isDriverBrakingChanged(value.toBool());
    else if (property == QLatin1String("absStatus"))
        emit absStatusChanged(static_cast<Common::ABSStatus>(value.toInt()));
    else if (property == QLatin1String("fluidLevelWarning"))
        emit fluidLevelWarningChanged(static_cast<Common::FluidLevelWarning>(value.toInt()));
    else if (property == QLatin1String("isParkingBrakeEngaged"))
        emit isParkingBrakeEngagedChanged(value.toBool());
    else if (property == QLatin1String("parkingBrakeType"))
        emit parkingBrakeTypeChanged(static_cast<Common::ParkingBrakeType>(value.toInt()));
}
