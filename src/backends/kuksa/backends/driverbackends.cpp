#include "driverbackends.h"
#include "../kuksaclient.h"

#include <common.h>

// ===========================================================================
// DriverMonitoringKuksaBackend
// ===========================================================================

DriverMonitoringKuksaBackend::DriverMonitoringKuksaBackend(KuksaClient *client, QObject *parent)
    : DriverMonitoringBackendInterface(parent)
    , m_client(client)
{
    m_client->registerBackend(QStringLiteral("COVESA.VSS.Driver.DriverMonitoring"),
        [this](const QString &property, const QString &zone, const QVariant &value) {
            onVssValue(property, zone, value);
        });
}

void DriverMonitoringKuksaBackend::initialize()
{
    emit initializationDone();
}

void DriverMonitoringKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("heartRate"))
        emit heartRateChanged(value.toReal());
    else if (property == QLatin1String("isAttentive"))
        emit isAttentiveChanged(value.toBool());
    else if (property == QLatin1String("attentivenessLevel"))
        emit attentivenessLevelChanged(value.toReal());
    else if (property == QLatin1String("isFatigued"))
        emit isFatiguedChanged(value.toBool());
    else if (property == QLatin1String("fatigueLevel"))
        emit fatigueLevelChanged(value.toReal());
    else if (property == QLatin1String("isEyesOnRoad"))
        emit isEyesOnRoadChanged(value.toBool());
    else if (property == QLatin1String("isHandsOnWheel"))
        emit isHandsOnWheelChanged(value.toBool());
    else if (property == QLatin1String("distractionLevel"))
        emit distractionLevelChanged(value.toInt());
    else if (property == QLatin1String("identifierSubject"))
        emit identifierSubjectChanged(value.toString());
    else if (property == QLatin1String("identifierIssuer"))
        emit identifierIssuerChanged(value.toString());
}
