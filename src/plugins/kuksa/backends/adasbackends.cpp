#include "adasbackends.h"
#include "../kuksaclient.h"
#include "../vsspathmapping.h"

#include <common.h>
#include "../enumvalidation.h"

// ===========================================================================
// ADASControlKuksaBackend
// ===========================================================================

ADASControlKuksaBackend::ADASControlKuksaBackend(KuksaClient *client, QObject *parent)
    : ADASControlBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.ADAS.ADASControl"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

ADASControlKuksaBackend::~ADASControlKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.ADAS.ADASControl"));
}

void ADASControlKuksaBackend::initialize()
{
    emit initializationDone();
}

void ADASControlKuksaBackend::setIsAEBEnabled(bool isAEBEnabled)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.ADAS.ADASControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isAEBEnabled")), isAEBEnabled);
}

void ADASControlKuksaBackend::setIsEBDEnabled(bool isEBDEnabled)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.ADAS.ADASControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isEBDEnabled")), isEBDEnabled);
}

void ADASControlKuksaBackend::setIsESCEnabled(bool isESCEnabled)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.ADAS.ADASControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isESCEnabled")), isESCEnabled);
}

void ADASControlKuksaBackend::setIsTCSEnabled(bool isTCSEnabled)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.ADAS.ADASControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isTCSEnabled")), isTCSEnabled);
}

void ADASControlKuksaBackend::setIsLaneDepartureEnabled(bool isLaneDepartureEnabled)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.ADAS.ADASControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isLaneDepartureEnabled")), isLaneDepartureEnabled);
}

void ADASControlKuksaBackend::setIsBlindSpotEnabled(bool isBlindSpotEnabled)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.ADAS.ADASControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isBlindSpotEnabled")), isBlindSpotEnabled);
}

void ADASControlKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("activeAutonomyLevel"))
        emit activeAutonomyLevelChanged(validatedEnum<Common::AutonomyLevel>(value, 5));
    else if (property == QLatin1String("supportedAutonomyLevel"))
        emit supportedAutonomyLevelChanged(validatedEnum<Common::AutonomyLevel>(value, 5));
    else if (property == QLatin1String("isAEBEnabled"))
        emit isAEBEnabledChanged(value.toBool());
    else if (property == QLatin1String("isAEBActive"))
        emit isAEBActiveChanged(value.toBool());
    else if (property == QLatin1String("isAEBError"))
        emit isAEBErrorChanged(value.toBool());
    else if (property == QLatin1String("isEBDEnabled"))
        emit isEBDEnabledChanged(value.toBool());
    else if (property == QLatin1String("isEBDActive"))
        emit isEBDActiveChanged(value.toBool());
    else if (property == QLatin1String("isEBDError"))
        emit isEBDErrorChanged(value.toBool());
    else if (property == QLatin1String("isESCEnabled"))
        emit isESCEnabledChanged(value.toBool());
    else if (property == QLatin1String("isESCActive"))
        emit isESCActiveChanged(value.toBool());
    else if (property == QLatin1String("isESCError"))
        emit isESCErrorChanged(value.toBool());
    else if (property == QLatin1String("isTCSEnabled"))
        emit isTCSEnabledChanged(value.toBool());
    else if (property == QLatin1String("isTCSActive"))
        emit isTCSActiveChanged(value.toBool());
    else if (property == QLatin1String("isTCSError"))
        emit isTCSErrorChanged(value.toBool());
    else if (property == QLatin1String("laneDepartureState"))
        emit laneDepartureStateChanged(validatedEnum<Common::LaneDepartureState>(value, 4));
    else if (property == QLatin1String("isLaneDepartureEnabled"))
        emit isLaneDepartureEnabledChanged(value.toBool());
    else if (property == QLatin1String("isLaneDepartureWarning"))
        emit isLaneDepartureWarningChanged(value.toBool());
    else if (property == QLatin1String("blindSpotState"))
        emit blindSpotStateChanged(validatedEnum<Common::BlindSpotState>(value, 3));
    else if (property == QLatin1String("isBlindSpotEnabled"))
        emit isBlindSpotEnabledChanged(value.toBool());
}

// ===========================================================================
// CruiseControlKuksaBackend
// ===========================================================================

CruiseControlKuksaBackend::CruiseControlKuksaBackend(KuksaClient *client, QObject *parent)
    : CruiseControlBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.ADAS.CruiseControl"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

CruiseControlKuksaBackend::~CruiseControlKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.ADAS.CruiseControl"));
}

void CruiseControlKuksaBackend::initialize()
{
    emit initializationDone();
}

void CruiseControlKuksaBackend::setIsEnabled(bool isEnabled)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.ADAS.CruiseControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isEnabled")), isEnabled);
}

void CruiseControlKuksaBackend::setSpeedSet(qreal speedSet)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.ADAS.CruiseControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("speedSet")), speedSet);
}

void CruiseControlKuksaBackend::setTargetGap(qreal targetGap)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.ADAS.CruiseControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("targetGap")), targetGap);
}

void CruiseControlKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("isEnabled"))
        emit isEnabledChanged(value.toBool());
    else if (property == QLatin1String("isActive"))
        emit isActiveChanged(value.toBool());
    else if (property == QLatin1String("isError"))
        emit isErrorChanged(value.toBool());
    else if (property == QLatin1String("speedSet"))
        emit speedSetChanged(value.toReal());
    else if (property == QLatin1String("mode"))
        emit modeChanged(validatedEnum<Common::CruiseControlMode>(value, 4));
    else if (property == QLatin1String("targetGap"))
        emit targetGapChanged(value.toReal());
    else if (property == QLatin1String("isAdaptive"))
        emit isAdaptiveChanged(value.toBool());
}

// ===========================================================================
// ObstacleDetectionKuksaBackend (zoned: Front, Rear, Left, Right)
// ===========================================================================

ObstacleDetectionKuksaBackend::ObstacleDetectionKuksaBackend(KuksaClient *client, QObject *parent)
    : ObstacleDetectionBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.ADAS.ObstacleDetection"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

ObstacleDetectionKuksaBackend::~ObstacleDetectionKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.ADAS.ObstacleDetection"));
}

void ObstacleDetectionKuksaBackend::initialize()
{
    emit initializationDone();
}

QStringList ObstacleDetectionKuksaBackend::availableZones() const
{
    static const QString iid = QStringLiteral("COVESA.VSS.ADAS.ObstacleDetection");
    return VssPathMapping::zones(iid);
}

void ObstacleDetectionKuksaBackend::setIsEnabled(bool isEnabled, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.ADAS.ObstacleDetection");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isEnabled"), zone), isEnabled);
}

void ObstacleDetectionKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    if (property == QLatin1String("distance"))
        emit distanceChanged(value.toReal(), zone);
    else if (property == QLatin1String("warningLevel"))
        emit warningLevelChanged(validatedEnum<Common::ObstacleWarningLevel>(value, 4), zone);
    else if (property == QLatin1String("isEnabled"))
        emit isEnabledChanged(value.toBool(), zone);
    else if (property == QLatin1String("isDetected"))
        emit isDetectedChanged(value.toBool(), zone);
}
