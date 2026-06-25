#include "exteriorbackends.h"
#include "../kuksaclient.h"

#include <common.h>
#include "../enumvalidation.h"

// ===========================================================================
// EnvironmentSensorsKuksaBackend
// ===========================================================================

EnvironmentSensorsKuksaBackend::EnvironmentSensorsKuksaBackend(KuksaClient *client, QObject *parent)
    : EnvironmentSensorsBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Exterior.EnvironmentSensors"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

EnvironmentSensorsKuksaBackend::~EnvironmentSensorsKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Exterior.EnvironmentSensors"));
}

void EnvironmentSensorsKuksaBackend::initialize()
{
    emit initializationDone();
}

void EnvironmentSensorsKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("ambientTemperature"))
        emit ambientTemperatureChanged(value.toReal());
    else if (property == QLatin1String("humidity"))
        emit humidityChanged(value.toReal());
    else if (property == QLatin1String("atmosphericPressure"))
        emit atmosphericPressureChanged(value.toReal());
    else if (property == QLatin1String("visibility"))
        emit visibilityChanged(value.toReal());
    else if (property == QLatin1String("lightIntensity"))
        emit lightIntensityChanged(value.toReal());
    else if (property == QLatin1String("roadCondition"))
        emit roadConditionChanged(validatedEnum<Common::RoadSurfaceCondition>(value, 4));
    else if (property == QLatin1String("windSpeed"))
        emit windSpeedChanged(value.toReal());
    else if (property == QLatin1String("windDirection"))
        emit windDirectionChanged(value.toReal());
    else if (property == QLatin1String("isRaining"))
        emit isRainingChanged(value.toBool());
}
