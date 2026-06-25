#include "motionmanagementbackends.h"
#include "../kuksaclient.h"
#include "../vsspathmapping.h"

#include <common.h>
#include "../enumvalidation.h"

// ===========================================================================
// BrakeControlKuksaBackend
// ===========================================================================

BrakeControlKuksaBackend::BrakeControlKuksaBackend(KuksaClient *client, QObject *parent)
    : BrakeControlBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.MotionManagement.BrakeControl"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

BrakeControlKuksaBackend::~BrakeControlKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.MotionManagement.BrakeControl"));
}

void BrakeControlKuksaBackend::initialize()
{
    emit initializationDone();
}

void BrakeControlKuksaBackend::setPedalPositionTarget(int pedalPositionTarget)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.MotionManagement.BrakeControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("pedalPositionTarget")), pedalPositionTarget);
}

void BrakeControlKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("pedalPositionTarget"))
        emit pedalPositionTargetChanged(value.toInt());
    else if (property == QLatin1String("pedalPositionActual"))
        emit pedalPositionActualChanged(value.toInt());
    else if (property == QLatin1String("isBrakeActive"))
        emit isBrakeActiveChanged(value.toBool());
    else if (property == QLatin1String("brakeTorque"))
        emit brakeTorqueChanged(value.toReal());
}

// ===========================================================================
// SteeringControlKuksaBackend
// ===========================================================================

SteeringControlKuksaBackend::SteeringControlKuksaBackend(KuksaClient *client, QObject *parent)
    : SteeringControlBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.MotionManagement.SteeringControl"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

SteeringControlKuksaBackend::~SteeringControlKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.MotionManagement.SteeringControl"));
}

void SteeringControlKuksaBackend::initialize()
{
    emit initializationDone();
}

void SteeringControlKuksaBackend::setSteeringAngleTarget(qreal steeringAngleTarget)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.MotionManagement.SteeringControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("steeringAngleTarget")), steeringAngleTarget);
}

void SteeringControlKuksaBackend::setIsSteerByWireEnabled(bool isSteerByWireEnabled)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.MotionManagement.SteeringControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isSteerByWireEnabled")), isSteerByWireEnabled);
}

void SteeringControlKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("steeringAngleTarget"))
        emit steeringAngleTargetChanged(value.toReal());
    else if (property == QLatin1String("steeringAngleActual"))
        emit steeringAngleActualChanged(value.toReal());
    else if (property == QLatin1String("steeringTorque"))
        emit steeringTorqueChanged(value.toReal());
    else if (property == QLatin1String("isSteerByWireEnabled"))
        emit isSteerByWireEnabledChanged(value.toBool());
}

// ===========================================================================
// SuspensionControlKuksaBackend
// ===========================================================================

SuspensionControlKuksaBackend::SuspensionControlKuksaBackend(KuksaClient *client, QObject *parent)
    : SuspensionControlBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.MotionManagement.SuspensionControl"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

SuspensionControlKuksaBackend::~SuspensionControlKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.MotionManagement.SuspensionControl"));
}

void SuspensionControlKuksaBackend::initialize()
{
    emit initializationDone();
}

QStringList SuspensionControlKuksaBackend::availableZones() const
{
    static const QString iid = QStringLiteral("COVESA.VSS.MotionManagement.SuspensionControl");
    return VssPathMapping::zones(iid);
}

void SuspensionControlKuksaBackend::setHeightTarget(int heightTarget, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.MotionManagement.SuspensionControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("heightTarget"), zone), heightTarget);
}

void SuspensionControlKuksaBackend::setMode(Common::SuspensionMode mode, const QString &zone)
{
    if (!m_client) return;
    Q_UNUSED(zone)
    static const QString iid = QStringLiteral("COVESA.VSS.MotionManagement.SuspensionControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("mode")), static_cast<int>(mode));
}

void SuspensionControlKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    if (property == QLatin1String("heightTarget"))
        emit heightTargetChanged(value.toInt(), zone);
    else if (property == QLatin1String("heightActual"))
        emit heightActualChanged(value.toInt(), zone);
    else if (property == QLatin1String("mode"))
        emit modeChanged(validatedEnum<Common::SuspensionMode>(value, 3), zone);
    else if (property == QLatin1String("damperForce"))
        emit damperForceChanged(value.toReal(), zone);
}
