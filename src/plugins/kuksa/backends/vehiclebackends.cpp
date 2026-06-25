#include "vehiclebackends.h"
#include "../kuksaclient.h"

#include <common.h>
#include "../enumvalidation.h"

// ===========================================================================
// VehicleIdentificationKuksaBackend
// ===========================================================================

VehicleIdentificationKuksaBackend::VehicleIdentificationKuksaBackend(KuksaClient *client, QObject *parent)
    : VehicleIdentificationBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Vehicle.VehicleIdentification"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

VehicleIdentificationKuksaBackend::~VehicleIdentificationKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Vehicle.VehicleIdentification"));
}

void VehicleIdentificationKuksaBackend::initialize()
{
    emit initializationDone();
}

void VehicleIdentificationKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("vin"))
        emit vinChanged(value.toString());
    else if (property == QLatin1String("brand"))
        emit brandChanged(value.toString());
    else if (property == QLatin1String("vehicleModel"))
        emit vehicleModelChanged(value.toString());
    else if (property == QLatin1String("year"))
        emit yearChanged(value.toInt());
    else if (property == QLatin1String("bodyType"))
        emit bodyTypeChanged(value.toString());
    else if (property == QLatin1String("dateVehicleFirstRegistered"))
        emit dateVehicleFirstRegisteredChanged(value.toString());
    else if (property == QLatin1String("meetsEmissionStandard"))
        emit meetsEmissionStandardChanged(value.toString());
    else if (property == QLatin1String("vehicleSpecialUsage"))
        emit vehicleSpecialUsageChanged(value.toString());
    else if (property == QLatin1String("vehicleInteriorColor"))
        emit vehicleInteriorColorChanged(value.toString());
    else if (property == QLatin1String("vehicleInteriorType"))
        emit vehicleInteriorTypeChanged(value.toString());
    else if (property == QLatin1String("knownVehicleDamages"))
        emit knownVehicleDamagesChanged(value.toString());
}

// ===========================================================================
// VehicleDynamicsKuksaBackend
// ===========================================================================

VehicleDynamicsKuksaBackend::VehicleDynamicsKuksaBackend(KuksaClient *client, QObject *parent)
    : VehicleDynamicsBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Vehicle.VehicleDynamics"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

VehicleDynamicsKuksaBackend::~VehicleDynamicsKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Vehicle.VehicleDynamics"));
}

void VehicleDynamicsKuksaBackend::initialize()
{
    emit initializationDone();
}

void VehicleDynamicsKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("speed"))
        emit speedChanged(value.toReal());
    else if (property == QLatin1String("traveledDistance"))
        emit traveledDistanceChanged(value.toReal());
    else if (property == QLatin1String("traveledDistanceSinceStart"))
        emit traveledDistanceSinceStartChanged(value.toReal());
    else if (property == QLatin1String("tripMeterReading"))
        emit tripMeterReadingChanged(value.toReal());
    else if (property == QLatin1String("isMoving"))
        emit isMovingChanged(value.toBool());
    else if (property == QLatin1String("averageSpeed"))
        emit averageSpeedChanged(value.toReal());
    else if (property == QLatin1String("currentOverallWeight"))
        emit currentOverallWeightChanged(value.toInt());
}

// ===========================================================================
// CurrentLocationKuksaBackend
// ===========================================================================

CurrentLocationKuksaBackend::CurrentLocationKuksaBackend(KuksaClient *client, QObject *parent)
    : CurrentLocationBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Vehicle.CurrentLocation"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

CurrentLocationKuksaBackend::~CurrentLocationKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Vehicle.CurrentLocation"));
}

void CurrentLocationKuksaBackend::initialize()
{
    emit initializationDone();
}

void CurrentLocationKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("latitude"))
        emit latitudeChanged(value.toReal());
    else if (property == QLatin1String("longitude"))
        emit longitudeChanged(value.toReal());
    else if (property == QLatin1String("altitude"))
        emit altitudeChanged(value.toReal());
    else if (property == QLatin1String("heading"))
        emit headingChanged(value.toReal());
    else if (property == QLatin1String("horizontalAccuracy"))
        emit horizontalAccuracyChanged(value.toReal());
    else if (property == QLatin1String("verticalAccuracy"))
        emit verticalAccuracyChanged(value.toReal());
    else if (property == QLatin1String("gnssFixType"))
        emit gnssFixTypeChanged(validatedEnum<Common::GNSSFixType>(value, 7));
    else if (property == QLatin1String("timestamp"))
        emit timestampChanged(value.toString());
}

// ===========================================================================
// LowVoltageBatteryKuksaBackend
// ===========================================================================

LowVoltageBatteryKuksaBackend::LowVoltageBatteryKuksaBackend(KuksaClient *client, QObject *parent)
    : LowVoltageBatteryBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Vehicle.LowVoltageBattery"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

LowVoltageBatteryKuksaBackend::~LowVoltageBatteryKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Vehicle.LowVoltageBattery"));
}

void LowVoltageBatteryKuksaBackend::initialize()
{
    emit initializationDone();
}

void LowVoltageBatteryKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("currentVoltage"))
        emit currentVoltageChanged(value.toReal());
    else if (property == QLatin1String("currentCurrent"))
        emit currentCurrentChanged(value.toReal());
    else if (property == QLatin1String("nominalVoltage"))
        emit nominalVoltageChanged(value.toReal());
    else if (property == QLatin1String("nominalCapacity"))
        emit nominalCapacityChanged(value.toInt());
}
