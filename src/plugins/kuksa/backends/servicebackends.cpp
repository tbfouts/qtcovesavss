#include "servicebackends.h"
#include "../kuksaclient.h"

#include <common.h>

// ===========================================================================
// ServiceStatusKuksaBackend
// ===========================================================================

ServiceStatusKuksaBackend::ServiceStatusKuksaBackend(KuksaClient *client, QObject *parent)
    : ServiceStatusBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Service.ServiceStatus"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

ServiceStatusKuksaBackend::~ServiceStatusKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Service.ServiceStatus"));
}

void ServiceStatusKuksaBackend::initialize()
{
    emit initializationDone();
}

void ServiceStatusKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("overallStatus"))
        emit overallStatusChanged(static_cast<Common::ServiceStatus>(value.toInt()));
    else if (property == QLatin1String("distanceToService"))
        emit distanceToServiceChanged(value.toInt());
    else if (property == QLatin1String("timeToService"))
        emit timeToServiceChanged(value.toInt());
    else if (property == QLatin1String("isServiceDue"))
        emit isServiceDueChanged(value.toBool());
    else if (property == QLatin1String("lastServiceDate"))
        emit lastServiceDateChanged(value.toString());
    else if (property == QLatin1String("odometerAtLastService"))
        emit odometerAtLastServiceChanged(value.toInt());
    else if (property == QLatin1String("isOilChangeRequired"))
        emit isOilChangeRequiredChanged(value.toBool());
    else if (property == QLatin1String("isBrakeInspectionRequired"))
        emit isBrakeInspectionRequiredChanged(value.toBool());
    else if (property == QLatin1String("isTireRotationRequired"))
        emit isTireRotationRequiredChanged(value.toBool());
    else if (property == QLatin1String("engineOilLifeRemaining"))
        emit engineOilLifeRemainingChanged(value.toInt());
}
