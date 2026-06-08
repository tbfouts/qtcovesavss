#include "powertrainbackends.h"
#include "../kuksaclient.h"
#include "../vsspathmapping.h"

#include <common.h>

// ===========================================================================
// PowertrainStatusKuksaBackend
// ===========================================================================

PowertrainStatusKuksaBackend::PowertrainStatusKuksaBackend(KuksaClient *client, QObject *parent)
    : PowertrainStatusBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Powertrain.PowertrainStatus"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

PowertrainStatusKuksaBackend::~PowertrainStatusKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Powertrain.PowertrainStatus"));
}

void PowertrainStatusKuksaBackend::initialize()
{
    emit initializationDone();
}

void PowertrainStatusKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("type"))
        emit typeChanged(static_cast<Common::PowertrainType>(value.toInt()));
    else if (property == QLatin1String("accumulatedBrakingEnergy"))
        emit accumulatedBrakingEnergyChanged(value.toReal());
    else if (property == QLatin1String("range"))
        emit rangeChanged(value.toReal());
}

// ===========================================================================
// CombustionEngineKuksaBackend
// ===========================================================================

CombustionEngineKuksaBackend::CombustionEngineKuksaBackend(KuksaClient *client, QObject *parent)
    : CombustionEngineBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Powertrain.CombustionEngine"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

CombustionEngineKuksaBackend::~CombustionEngineKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Powertrain.CombustionEngine"));
}

void CombustionEngineKuksaBackend::initialize()
{
    emit initializationDone();
}

void CombustionEngineKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("engineState"))
        emit engineStateChanged(static_cast<Common::EngineState>(value.toInt()));
    else if (property == QLatin1String("speed"))
        emit speedChanged(value.toInt());
    else if (property == QLatin1String("engineHours"))
        emit engineHoursChanged(value.toInt());
    else if (property == QLatin1String("engineCoolantTemperature"))
        emit engineCoolantTemperatureChanged(value.toReal());
    else if (property == QLatin1String("engineOilTemperature"))
        emit engineOilTemperatureChanged(value.toReal());
    else if (property == QLatin1String("engineOilPressure"))
        emit engineOilPressureChanged(value.toReal());
    else if (property == QLatin1String("engineOilLevel"))
        emit engineOilLevelChanged(value.toInt());
    else if (property == QLatin1String("power"))
        emit powerChanged(value.toReal());
    else if (property == QLatin1String("torque"))
        emit torqueChanged(value.toReal());
    else if (property == QLatin1String("displacement"))
        emit displacementChanged(value.toInt());
    else if (property == QLatin1String("configuration"))
        emit configurationChanged(value.toString());
    else if (property == QLatin1String("numberOfCylinders"))
        emit numberOfCylindersChanged(value.toInt());
    else if (property == QLatin1String("maf"))
        emit mafChanged(value.toReal());
    else if (property == QLatin1String("manifoldAbsolutePressure"))
        emit manifoldAbsolutePressureChanged(value.toInt());
    else if (property == QLatin1String("tps"))
        emit tpsChanged(value.toReal());
    else if (property == QLatin1String("isRunning"))
        emit isRunningChanged(value.toBool());
    else if (property == QLatin1String("idleHours"))
        emit idleHoursChanged(value.toInt());
}

// ===========================================================================
// TransmissionKuksaBackend
// ===========================================================================

TransmissionKuksaBackend::TransmissionKuksaBackend(KuksaClient *client, QObject *parent)
    : TransmissionBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Powertrain.Transmission"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

TransmissionKuksaBackend::~TransmissionKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Powertrain.Transmission"));
}

void TransmissionKuksaBackend::initialize()
{
    emit initializationDone();
}

void TransmissionKuksaBackend::setPerformanceMode(Common::TransmissionPerformanceMode performanceMode)
{
    if (!m_client) return;
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Powertrain.Transmission"), QStringLiteral("performanceMode"));
    m_client->actuate(path, QVariant::fromValue(static_cast<int>(performanceMode)));
}

void TransmissionKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("type"))
        emit typeChanged(static_cast<Common::TransmissionGearType>(value.toInt()));
    else if (property == QLatin1String("gearCount"))
        emit gearCountChanged(value.toInt());
    else if (property == QLatin1String("driveType"))
        emit driveTypeChanged(static_cast<Common::TransmissionDriveType>(value.toInt()));
    else if (property == QLatin1String("currentGear"))
        emit currentGearChanged(value.toInt());
    else if (property == QLatin1String("selectedGear"))
        emit selectedGearChanged(value.toInt());
    else if (property == QLatin1String("isParkLockEngaged"))
        emit isParkLockEngagedChanged(value.toBool());
    else if (property == QLatin1String("performanceMode"))
        emit performanceModeChanged(static_cast<Common::TransmissionPerformanceMode>(value.toInt()));
    else if (property == QLatin1String("temperature"))
        emit temperatureChanged(value.toReal());
    else if (property == QLatin1String("clutchWear"))
        emit clutchWearChanged(value.toReal());
    else if (property == QLatin1String("travelledDistance"))
        emit travelledDistanceChanged(value.toReal());
}

// ===========================================================================
// ElectricMotorKuksaBackend (zoned: Motor1, Motor2)
// ===========================================================================

ElectricMotorKuksaBackend::ElectricMotorKuksaBackend(KuksaClient *client, QObject *parent)
    : ElectricMotorBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Powertrain.ElectricMotor"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

ElectricMotorKuksaBackend::~ElectricMotorKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Powertrain.ElectricMotor"));
}

void ElectricMotorKuksaBackend::initialize()
{
    emit initializationDone();
}

QStringList ElectricMotorKuksaBackend::availableZones() const
{
    return VssPathMapping::zones(QStringLiteral("COVESA.VSS.Powertrain.ElectricMotor"));
}

void ElectricMotorKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    if (property == QLatin1String("maxPower"))
        emit maxPowerChanged(value.toInt(), zone);
    else if (property == QLatin1String("maxTorque"))
        emit maxTorqueChanged(value.toInt(), zone);
    else if (property == QLatin1String("maxRegenPower"))
        emit maxRegenPowerChanged(value.toInt(), zone);
    else if (property == QLatin1String("maxRegenTorque"))
        emit maxRegenTorqueChanged(value.toInt(), zone);
    else if (property == QLatin1String("speed"))
        emit speedChanged(value.toInt(), zone);
    else if (property == QLatin1String("temperature"))
        emit temperatureChanged(value.toReal(), zone);
    else if (property == QLatin1String("coolantTemperature"))
        emit coolantTemperatureChanged(value.toReal(), zone);
    else if (property == QLatin1String("power"))
        emit powerChanged(value.toReal(), zone);
    else if (property == QLatin1String("torque"))
        emit torqueChanged(value.toReal(), zone);
    else if (property == QLatin1String("operatingMode"))
        emit operatingModeChanged(static_cast<Common::ElectricMotorMode>(value.toInt()), zone);
}

// ===========================================================================
// TractionBatteryKuksaBackend
// ===========================================================================

TractionBatteryKuksaBackend::TractionBatteryKuksaBackend(KuksaClient *client, QObject *parent)
    : TractionBatteryBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Powertrain.TractionBattery"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

TractionBatteryKuksaBackend::~TractionBatteryKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Powertrain.TractionBattery"));
}

void TractionBatteryKuksaBackend::initialize()
{
    emit initializationDone();
}

void TractionBatteryKuksaBackend::setChargeLimit(qreal chargeLimit)
{
    if (!m_client) return;
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Powertrain.TractionBattery"), QStringLiteral("chargeLimit"));
    m_client->actuate(path, QVariant(chargeLimit));
}

void TractionBatteryKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("id"))
        emit idChanged(value.toString());
    else if (property == QLatin1String("stateOfCharge"))
        emit stateOfChargeChanged(value.toReal());
    else if (property == QLatin1String("stateOfHealth"))
        emit stateOfHealthChanged(value.toReal());
    else if (property == QLatin1String("currentVoltage"))
        emit currentVoltageChanged(value.toReal());
    else if (property == QLatin1String("currentCurrent"))
        emit currentCurrentChanged(value.toReal());
    else if (property == QLatin1String("currentPower"))
        emit currentPowerChanged(value.toReal());
    else if (property == QLatin1String("temperature"))
        emit temperatureChanged(value.toReal());
    else if (property == QLatin1String("nominalVoltage"))
        emit nominalVoltageChanged(value.toInt());
    else if (property == QLatin1String("maxVoltage"))
        emit maxVoltageChanged(value.toInt());
    else if (property == QLatin1String("grossCapacity"))
        emit grossCapacityChanged(value.toInt());
    else if (property == QLatin1String("netCapacity"))
        emit netCapacityChanged(value.toInt());
    else if (property == QLatin1String("accumulatedChargedEnergy"))
        emit accumulatedChargedEnergyChanged(value.toReal());
    else if (property == QLatin1String("accumulatedConsumedEnergy"))
        emit accumulatedConsumedEnergyChanged(value.toReal());
    else if (property == QLatin1String("chargingStatus"))
        emit chargingStatusChanged(static_cast<Common::ChargingStatus>(value.toInt()));
    else if (property == QLatin1String("chargePlugType"))
        emit chargePlugTypeChanged(static_cast<Common::ChargePlugType>(value.toInt()));
    else if (property == QLatin1String("isCharging"))
        emit isChargingChanged(value.toBool());
    else if (property == QLatin1String("isChargingCableConnected"))
        emit isChargingCableConnectedChanged(value.toBool());
    else if (property == QLatin1String("chargingRate"))
        emit chargingRateChanged(value.toReal());
    else if (property == QLatin1String("chargeLimit"))
        emit chargeLimitChanged(value.toReal());
    else if (property == QLatin1String("timeToComplete"))
        emit timeToCompleteChanged(value.toReal());
}

// ===========================================================================
// FuelSystemKuksaBackend
// ===========================================================================

FuelSystemKuksaBackend::FuelSystemKuksaBackend(KuksaClient *client, QObject *parent)
    : FuelSystemBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Powertrain.FuelSystem"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

FuelSystemKuksaBackend::~FuelSystemKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Powertrain.FuelSystem"));
}

void FuelSystemKuksaBackend::initialize()
{
    emit initializationDone();
}

void FuelSystemKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("supportedFuelType"))
        emit supportedFuelTypeChanged(static_cast<Common::FuelType>(value.toInt()));
    else if (property == QLatin1String("fuelLevel"))
        emit fuelLevelChanged(value.toReal());
    else if (property == QLatin1String("range"))
        emit rangeChanged(value.toReal());
    else if (property == QLatin1String("instantConsumption"))
        emit instantConsumptionChanged(value.toReal());
    else if (property == QLatin1String("averageConsumption"))
        emit averageConsumptionChanged(value.toReal());
    else if (property == QLatin1String("consumptionSinceStart"))
        emit consumptionSinceStartChanged(value.toReal());
    else if (property == QLatin1String("tankCapacity"))
        emit tankCapacityChanged(value.toInt());
    else if (property == QLatin1String("isEngineStopStartEnabled"))
        emit isEngineStopStartEnabledChanged(value.toBool());
    else if (property == QLatin1String("isFuelLevelLow"))
        emit isFuelLevelLowChanged(value.toBool());
}
