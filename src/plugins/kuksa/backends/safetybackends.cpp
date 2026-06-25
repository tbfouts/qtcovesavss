#include "safetybackends.h"
#include "../kuksaclient.h"
#include "../vsspathmapping.h"

#include <common.h>
#include "../enumvalidation.h"

// ===========================================================================
// CrashDetectionKuksaBackend
// ===========================================================================

CrashDetectionKuksaBackend::CrashDetectionKuksaBackend(KuksaClient *client, QObject *parent)
    : CrashDetectionBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Safety.CrashDetection"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

CrashDetectionKuksaBackend::~CrashDetectionKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Safety.CrashDetection"));
}

void CrashDetectionKuksaBackend::initialize()
{
    emit initializationDone();
}

void CrashDetectionKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("crashStatus"))
        emit crashStatusChanged(validatedEnum<Common::CrashStatus>(value, 6));
    else if (property == QLatin1String("crashSeverity"))
        emit crashSeverityChanged(value.toReal());
    else if (property == QLatin1String("isCrashDetected"))
        emit isCrashDetectedChanged(value.toBool());
    else if (property == QLatin1String("crashTimestamp"))
        emit crashTimestampChanged(value.toString());
    else if (property == QLatin1String("impactForce"))
        emit impactForceChanged(value.toReal());
    else if (property == QLatin1String("impactAngle"))
        emit impactAngleChanged(value.toReal());
}

// ===========================================================================
// AirbagSystemKuksaBackend
// ===========================================================================

AirbagSystemKuksaBackend::AirbagSystemKuksaBackend(KuksaClient *client, QObject *parent)
    : AirbagSystemBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Safety.AirbagSystem"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

AirbagSystemKuksaBackend::~AirbagSystemKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Safety.AirbagSystem"));
}

void AirbagSystemKuksaBackend::initialize()
{
    emit initializationDone();
}

QStringList AirbagSystemKuksaBackend::availableZones() const
{
    static const QString iid = QStringLiteral("COVESA.VSS.Safety.AirbagSystem");
    return VssPathMapping::zones(iid);
}

void AirbagSystemKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    if (property == QLatin1String("status"))
        emit statusChanged(validatedEnum<Common::AirbagStatus>(value, 2), zone);
    else if (property == QLatin1String("isDeployed"))
        emit isDeployedChanged(value.toBool(), zone);
}

// ===========================================================================
// BeltSystemKuksaBackend
// ===========================================================================

BeltSystemKuksaBackend::BeltSystemKuksaBackend(KuksaClient *client, QObject *parent)
    : BeltSystemBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Safety.BeltSystem"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

BeltSystemKuksaBackend::~BeltSystemKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Safety.BeltSystem"));
}

void BeltSystemKuksaBackend::initialize()
{
    emit initializationDone();
}

QStringList BeltSystemKuksaBackend::availableZones() const
{
    static const QString iid = QStringLiteral("COVESA.VSS.Safety.BeltSystem");
    return VssPathMapping::zones(iid);
}

void BeltSystemKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    if (property == QLatin1String("isFastened"))
        emit isFastenedChanged(value.toBool(), zone);
    else if (property == QLatin1String("isWarning"))
        emit isWarningChanged(value.toBool(), zone);
}
