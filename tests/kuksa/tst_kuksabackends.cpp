#include <QTest>
#include <QSignalSpy>

#include "kuksaclient.h"
#include "vsspathmapping.h"

#include "backends/vehiclebackends.h"
#include "backends/powertrainbackends.h"
#include "backends/bodybackends.h"
#include "backends/cabinbackends.h"
#include "backends/adasbackends.h"
#include "backends/chassisbackends.h"
#include "backends/driverbackends.h"
#include "backends/safetybackends.h"
#include "backends/exteriorbackends.h"
#include "backends/servicebackends.h"
#include "backends/connectivitybackends.h"
#include "backends/motionmanagementbackends.h"

#include <common.h>

class KuksaBackendTest : public QObject
{
    Q_OBJECT

private:
    KuksaClient m_client;

private slots:
    // ── VssPathMapping ─────────────────────────────────────────────

    void testForwardLookup();
    void testForwardLookupMissing();
    void testReverseLookup();
    void testReverseLookupMissing();
    void testZonedForwardLookup();
    void testZonedReverseLookup();
    void testAvailableZones();
    void testZoneToVssInstance();
    void testAllPathsNoDuplicates();
    void testVssPathsForIid();

    // ── Non-zoned backend dispatch ─────────────────────────────────

    void testVehicleDynamicsSpeed();
    void testVehicleDynamicsBool();
    void testVehicleIdentificationVin();
    void testVehicleIdentificationYear();
    void testCurrentLocationLatitude();
    void testLowVoltageBatteryVoltage();
    void testCombustionEngineSpeed();
    void testCombustionEngineBool();
    void testTransmissionCurrentGear();
    void testTractionBatterySoC();
    void testTractionBatteryBool();
    void testFuelSystemLevel();
    void testBodyControlIsHoodOpen();
    void testBodyLightsHighBeam();
    void testCabinInfotainmentVolume();
    void testCabinLightsDomeLight();
    void testADASControlAEB();
    void testCruiseControlSpeedSet();
    void testChassisSteeringAngle();
    void testChassisBrakePedalPosition();
    void testDriverMonitoringHeartRate();
    void testCrashDetectionBool();
    void testCrashDetectionSeverity();
    void testEnvironmentSensorsTemperature();
    void testServiceStatusDistanceToService();
    void testConnectivityCellularSignal();
    void testBrakeControlTorque();
    void testSteeringControlAngle();

    // ── Zoned backend dispatch ─────────────────────────────────────

    void testElectricMotorZoned();
    void testElectricMotorAvailableZones();
    void testCabinSeatZonedPosition();
    void testCabinSeatAvailableZones();
    void testCabinHVACZonedTemperature();
    void testCabinDoorZonedIsOpen();
    void testWindshieldZoned();
    void testBodyMirrorsZoned();
    void testObstacleDetectionZoned();
    void testChassisAxleZoned();
    void testChassisWheelZoned();
    void testAirbagSystemZoned();
    void testBeltSystemZoned();
    void testSuspensionControlZoned();

    // ── Enum conversion ────────────────────────────────────────────

    void testPowertrainTypeEnum();
    void testEngineStateEnum();
    void testChargingStatusEnum();
    void testGNSSFixTypeEnum();
    void testRoadConditionEnum();
    void testCrashStatusEnum();

    // ── Edge cases ─────────────────────────────────────────────────

    void testUnknownPropertyIgnored();
    void testUnregisteredIidIgnored();
};

// ═══════════════════════════════════════════════════════════════════
// VssPathMapping tests
// ═══════════════════════════════════════════════════════════════════

void KuksaBackendTest::testForwardLookup()
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Vehicle.VehicleDynamics"),
        QStringLiteral("speed"));
    QCOMPARE(path, QStringLiteral("Vehicle.Speed"));
}

void KuksaBackendTest::testForwardLookupMissing()
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Vehicle.VehicleDynamics"),
        QStringLiteral("nonexistent"));
    QVERIFY(path.isEmpty());
}

void KuksaBackendTest::testReverseLookup()
{
    const auto entries = VssPathMapping::entries(QStringLiteral("Vehicle.Speed"));
    QCOMPARE(entries.size(), 1);
    QCOMPARE(entries.at(0).iid, QStringLiteral("COVESA.VSS.Vehicle.VehicleDynamics"));
    QCOMPARE(entries.at(0).property, QStringLiteral("speed"));
    QVERIFY(entries.at(0).zone.isEmpty());
}

void KuksaBackendTest::testReverseLookupMissing()
{
    const auto entries = VssPathMapping::entries(QStringLiteral("Vehicle.Nonexistent"));
    QVERIFY(entries.isEmpty());
}

void KuksaBackendTest::testZonedForwardLookup()
{
    const QString path = VssPathMapping::vssPath(
        QStringLiteral("COVESA.VSS.Powertrain.ElectricMotor"),
        QStringLiteral("speed"),
        QStringLiteral("Motor1"));
    QCOMPARE(path, QStringLiteral("Vehicle.Powertrain.ElectricMotor.Motor1.Speed"));
}

void KuksaBackendTest::testZonedReverseLookup()
{
    const auto entries = VssPathMapping::entries(
        QStringLiteral("Vehicle.Powertrain.ElectricMotor.Motor2.Power"));
    QCOMPARE(entries.size(), 1);
    QCOMPARE(entries.at(0).iid, QStringLiteral("COVESA.VSS.Powertrain.ElectricMotor"));
    QCOMPARE(entries.at(0).property, QStringLiteral("power"));
    QCOMPARE(entries.at(0).zone, QStringLiteral("Motor2"));
}

void KuksaBackendTest::testAvailableZones()
{
    const QStringList zones = VssPathMapping::zones(
        QStringLiteral("COVESA.VSS.Powertrain.ElectricMotor"));
    QCOMPARE(zones.size(), 2);
    QVERIFY(zones.contains(QStringLiteral("Motor1")));
    QVERIFY(zones.contains(QStringLiteral("Motor2")));
}

void KuksaBackendTest::testZoneToVssInstance()
{
    // CabinSeat maps Row1Left -> Row1.DriverSide
    const QString instance = VssPathMapping::zoneToVssInstance(
        QStringLiteral("COVESA.VSS.Cabin.CabinSeat"),
        QStringLiteral("Row1Left"));
    QCOMPARE(instance, QStringLiteral("Row1.DriverSide"));

    // ChassisAxle maps Front -> Row1
    const QString axleInstance = VssPathMapping::zoneToVssInstance(
        QStringLiteral("COVESA.VSS.Chassis.ChassisAxle"),
        QStringLiteral("Front"));
    QCOMPARE(axleInstance, QStringLiteral("Row1"));
}

void KuksaBackendTest::testAllPathsNoDuplicates()
{
    const QStringList all = VssPathMapping::allVssPaths();
    QVERIFY(all.size() > 100);

    const QSet<QString> unique(all.cbegin(), all.cend());
    QCOMPARE(unique.size(), all.size());
}

void KuksaBackendTest::testVssPathsForIid()
{
    const QStringList paths = VssPathMapping::vssPaths(
        QStringLiteral("COVESA.VSS.Vehicle.VehicleDynamics"));
    QVERIFY(paths.contains(QStringLiteral("Vehicle.Speed")));
    QVERIFY(paths.contains(QStringLiteral("Vehicle.IsMoving")));
    QCOMPARE(paths.size(), 7);
}

// ═══════════════════════════════════════════════════════════════════
// Non-zoned backend dispatch tests
// ═══════════════════════════════════════════════════════════════════

void KuksaBackendTest::testVehicleDynamicsSpeed()
{
    VehicleDynamicsKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &VehicleDynamicsBackendInterface::speedChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Vehicle.VehicleDynamics"),
        QStringLiteral("speed"), QString(), QVariant(72.5));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 72.5);
}

void KuksaBackendTest::testVehicleDynamicsBool()
{
    VehicleDynamicsKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &VehicleDynamicsBackendInterface::isMovingChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Vehicle.VehicleDynamics"),
        QStringLiteral("isMoving"), QString(), QVariant(true));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
}

void KuksaBackendTest::testVehicleIdentificationVin()
{
    VehicleIdentificationKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &VehicleIdentificationBackendInterface::vinChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Vehicle.VehicleIdentification"),
        QStringLiteral("vin"), QString(), QVariant(QStringLiteral("WBA12345678901234")));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QStringLiteral("WBA12345678901234"));
}

void KuksaBackendTest::testVehicleIdentificationYear()
{
    VehicleIdentificationKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &VehicleIdentificationBackendInterface::yearChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Vehicle.VehicleIdentification"),
        QStringLiteral("year"), QString(), QVariant(2024));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 2024);
}

void KuksaBackendTest::testCurrentLocationLatitude()
{
    CurrentLocationKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CurrentLocationBackendInterface::latitudeChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Vehicle.CurrentLocation"),
        QStringLiteral("latitude"), QString(), QVariant(48.8566));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 48.8566);
}

void KuksaBackendTest::testLowVoltageBatteryVoltage()
{
    LowVoltageBatteryKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &LowVoltageBatteryBackendInterface::currentVoltageChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Vehicle.LowVoltageBattery"),
        QStringLiteral("currentVoltage"), QString(), QVariant(12.6));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 12.6);
}

void KuksaBackendTest::testCombustionEngineSpeed()
{
    CombustionEngineKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CombustionEngineBackendInterface::speedChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Powertrain.CombustionEngine"),
        QStringLiteral("speed"), QString(), QVariant(3500));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 3500);
}

void KuksaBackendTest::testCombustionEngineBool()
{
    CombustionEngineKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CombustionEngineBackendInterface::isRunningChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Powertrain.CombustionEngine"),
        QStringLiteral("isRunning"), QString(), QVariant(true));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
}

void KuksaBackendTest::testTransmissionCurrentGear()
{
    TransmissionKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &TransmissionBackendInterface::currentGearChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Powertrain.Transmission"),
        QStringLiteral("currentGear"), QString(), QVariant(3));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 3);
}

void KuksaBackendTest::testTractionBatterySoC()
{
    TractionBatteryKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &TractionBatteryBackendInterface::stateOfChargeChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Powertrain.TractionBattery"),
        QStringLiteral("stateOfCharge"), QString(), QVariant(85.5));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 85.5);
}

void KuksaBackendTest::testTractionBatteryBool()
{
    TractionBatteryKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &TractionBatteryBackendInterface::isChargingChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Powertrain.TractionBattery"),
        QStringLiteral("isCharging"), QString(), QVariant(true));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
}

void KuksaBackendTest::testFuelSystemLevel()
{
    FuelSystemKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &FuelSystemBackendInterface::fuelLevelChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Powertrain.FuelSystem"),
        QStringLiteral("fuelLevel"), QString(), QVariant(67.3));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 67.3);
}

void KuksaBackendTest::testBodyControlIsHoodOpen()
{
    BodyControlKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &BodyControlBackendInterface::isHoodOpenChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Body.BodyControl"),
        QStringLiteral("isHoodOpen"), QString(), QVariant(true));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
}

void KuksaBackendTest::testBodyLightsHighBeam()
{
    BodyLightsKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &BodyLightsBackendInterface::isHighBeamOnChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Body.BodyLights"),
        QStringLiteral("isHighBeamOn"), QString(), QVariant(true));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
}

void KuksaBackendTest::testCabinInfotainmentVolume()
{
    CabinInfotainmentKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CabinInfotainmentBackendInterface::volumeChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Cabin.CabinInfotainment"),
        QStringLiteral("volume"), QString(), QVariant(75));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 75);
}

void KuksaBackendTest::testCabinLightsDomeLight()
{
    CabinLightsKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CabinLightsBackendInterface::isDomeLightOnChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Cabin.CabinLights"),
        QStringLiteral("isDomeLightOn"), QString(), QVariant(true));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
}

void KuksaBackendTest::testADASControlAEB()
{
    ADASControlKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &ADASControlBackendInterface::isAEBActiveChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.ADAS.ADASControl"),
        QStringLiteral("isAEBActive"), QString(), QVariant(true));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
}

void KuksaBackendTest::testCruiseControlSpeedSet()
{
    CruiseControlKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CruiseControlBackendInterface::speedSetChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.ADAS.CruiseControl"),
        QStringLiteral("speedSet"), QString(), QVariant(120));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 120);
}

void KuksaBackendTest::testChassisSteeringAngle()
{
    ChassisSteeringKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &ChassisSteeringBackendInterface::steeringWheelAngleChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Chassis.ChassisSteering"),
        QStringLiteral("steeringWheelAngle"), QString(), QVariant(15.5));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 15.5);
}

void KuksaBackendTest::testChassisBrakePedalPosition()
{
    ChassisBrakeKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &ChassisBrakeBackendInterface::pedalPositionChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Chassis.ChassisBrake"),
        QStringLiteral("pedalPosition"), QString(), QVariant(42));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 42);
}

void KuksaBackendTest::testDriverMonitoringHeartRate()
{
    DriverMonitoringKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &DriverMonitoringBackendInterface::heartRateChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Driver.DriverMonitoring"),
        QStringLiteral("heartRate"), QString(), QVariant(72));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 72);
}

void KuksaBackendTest::testCrashDetectionBool()
{
    CrashDetectionKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CrashDetectionBackendInterface::isCrashDetectedChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Safety.CrashDetection"),
        QStringLiteral("isCrashDetected"), QString(), QVariant(false));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);
}

void KuksaBackendTest::testCrashDetectionSeverity()
{
    CrashDetectionKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CrashDetectionBackendInterface::crashSeverityChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Safety.CrashDetection"),
        QStringLiteral("crashSeverity"), QString(), QVariant(7.2));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 7.2);
}

void KuksaBackendTest::testEnvironmentSensorsTemperature()
{
    EnvironmentSensorsKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &EnvironmentSensorsBackendInterface::ambientTemperatureChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Exterior.EnvironmentSensors"),
        QStringLiteral("ambientTemperature"), QString(), QVariant(22.5));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 22.5);
}

void KuksaBackendTest::testServiceStatusDistanceToService()
{
    ServiceStatusKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &ServiceStatusBackendInterface::distanceToServiceChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Service.ServiceStatus"),
        QStringLiteral("distanceToService"), QString(), QVariant(5000));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 5000);
}

void KuksaBackendTest::testConnectivityCellularSignal()
{
    ConnectivityControlKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &ConnectivityControlBackendInterface::cellularSignalStrengthChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Connectivity.ConnectivityControl"),
        QStringLiteral("cellularSignalStrength"), QString(), QVariant(-65));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), -65);
}

void KuksaBackendTest::testBrakeControlTorque()
{
    BrakeControlKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &BrakeControlBackendInterface::brakeTorqueChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.MotionManagement.BrakeControl"),
        QStringLiteral("brakeTorque"), QString(), QVariant(150.0));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 150.0);
}

void KuksaBackendTest::testSteeringControlAngle()
{
    SteeringControlKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &SteeringControlBackendInterface::steeringAngleActualChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.MotionManagement.SteeringControl"),
        QStringLiteral("steeringAngleActual"), QString(), QVariant(-12.3));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), -12.3);
}

// ═══════════════════════════════════════════════════════════════════
// Zoned backend dispatch tests
// ═══════════════════════════════════════════════════════════════════

void KuksaBackendTest::testElectricMotorZoned()
{
    ElectricMotorKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &ElectricMotorBackendInterface::speedChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Powertrain.ElectricMotor"),
        QStringLiteral("speed"), QStringLiteral("Motor1"), QVariant(8000));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 8000);
    QCOMPARE(spy.at(0).at(1).toString(), QStringLiteral("Motor1"));
}

void KuksaBackendTest::testElectricMotorAvailableZones()
{
    ElectricMotorKuksaBackend backend(&m_client);
    const QStringList zones = backend.availableZones();
    QCOMPARE(zones.size(), 2);
    QVERIFY(zones.contains(QStringLiteral("Motor1")));
    QVERIFY(zones.contains(QStringLiteral("Motor2")));
}

void KuksaBackendTest::testCabinSeatZonedPosition()
{
    CabinSeatKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CabinSeatBackendInterface::positionChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Cabin.CabinSeat"),
        QStringLiteral("position"), QStringLiteral("Row1Left"), QVariant(50));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 50);
    QCOMPARE(spy.at(0).at(1).toString(), QStringLiteral("Row1Left"));
}

void KuksaBackendTest::testCabinSeatAvailableZones()
{
    CabinSeatKuksaBackend backend(&m_client);
    const QStringList zones = backend.availableZones();
    QCOMPARE(zones.size(), 5);
    QVERIFY(zones.contains(QStringLiteral("Row1Left")));
    QVERIFY(zones.contains(QStringLiteral("Row1Right")));
    QVERIFY(zones.contains(QStringLiteral("Row2Left")));
    QVERIFY(zones.contains(QStringLiteral("Row2Right")));
    QVERIFY(zones.contains(QStringLiteral("Row2Center")));
}

void KuksaBackendTest::testCabinHVACZonedTemperature()
{
    CabinHVACKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CabinHVACBackendInterface::targetTemperatureChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Cabin.CabinHVAC"),
        QStringLiteral("targetTemperature"), QStringLiteral("FrontLeft"), QVariant(22.0));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 22.0);
    QCOMPARE(spy.at(0).at(1).toString(), QStringLiteral("FrontLeft"));
}

void KuksaBackendTest::testCabinDoorZonedIsOpen()
{
    CabinDoorKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CabinDoorBackendInterface::isOpenChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Cabin.CabinDoor"),
        QStringLiteral("isOpen"), QStringLiteral("FrontRight"), QVariant(true));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
    QCOMPARE(spy.at(0).at(1).toString(), QStringLiteral("FrontRight"));
}

void KuksaBackendTest::testWindshieldZoned()
{
    WindshieldKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &WindshieldBackendInterface::wiperWearLevelChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Body.Windshield"),
        QStringLiteral("wiperWearLevel"), QStringLiteral("Front"), QVariant(30));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 30);
    QCOMPARE(spy.at(0).at(1).toString(), QStringLiteral("Front"));
}

void KuksaBackendTest::testBodyMirrorsZoned()
{
    BodyMirrorsKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &BodyMirrorsBackendInterface::tiltChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Body.BodyMirrors"),
        QStringLiteral("tilt"), QStringLiteral("Left"), QVariant(5));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 5);
    QCOMPARE(spy.at(0).at(1).toString(), QStringLiteral("Left"));
}

void KuksaBackendTest::testObstacleDetectionZoned()
{
    ObstacleDetectionKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &ObstacleDetectionBackendInterface::distanceChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.ADAS.ObstacleDetection"),
        QStringLiteral("distance"), QStringLiteral("Rear"), QVariant(1.5));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 1.5);
    QCOMPARE(spy.at(0).at(1).toString(), QStringLiteral("Rear"));
}

void KuksaBackendTest::testChassisAxleZoned()
{
    ChassisAxleKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &ChassisAxleBackendInterface::wheelCountChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Chassis.ChassisAxle"),
        QStringLiteral("wheelCount"), QStringLiteral("Front"), QVariant(2));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 2);
    QCOMPARE(spy.at(0).at(1).toString(), QStringLiteral("Front"));
}

void KuksaBackendTest::testChassisWheelZoned()
{
    ChassisWheelKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &ChassisWheelBackendInterface::tirePressureChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Chassis.ChassisWheel"),
        QStringLiteral("tirePressure"), QStringLiteral("FrontLeft"), QVariant(2.4));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 2.4);
    QCOMPARE(spy.at(0).at(1).toString(), QStringLiteral("FrontLeft"));
}

void KuksaBackendTest::testAirbagSystemZoned()
{
    AirbagSystemKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &AirbagSystemBackendInterface::isDeployedChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Safety.AirbagSystem"),
        QStringLiteral("isDeployed"), QStringLiteral("FrontLeft"), QVariant(false));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);
    QCOMPARE(spy.at(0).at(1).toString(), QStringLiteral("FrontLeft"));
}

void KuksaBackendTest::testBeltSystemZoned()
{
    BeltSystemKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &BeltSystemBackendInterface::isFastenedChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Safety.BeltSystem"),
        QStringLiteral("isFastened"), QStringLiteral("Row1Left"), QVariant(true));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
    QCOMPARE(spy.at(0).at(1).toString(), QStringLiteral("Row1Left"));
}

void KuksaBackendTest::testSuspensionControlZoned()
{
    SuspensionControlKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &SuspensionControlBackendInterface::heightActualChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.MotionManagement.SuspensionControl"),
        QStringLiteral("heightActual"), QStringLiteral("RearRight"), QVariant(155.0));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toReal(), 155.0);
    QCOMPARE(spy.at(0).at(1).toString(), QStringLiteral("RearRight"));
}

// ═══════════════════════════════════════════════════════════════════
// Enum conversion tests
// ═══════════════════════════════════════════════════════════════════

void KuksaBackendTest::testPowertrainTypeEnum()
{
    PowertrainStatusKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &PowertrainStatusBackendInterface::typeChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Powertrain.PowertrainStatus"),
        QStringLiteral("type"), QString(),
        QVariant(static_cast<int>(Common::PowertrainCombustion)));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<Common::PowertrainType>(), Common::PowertrainCombustion);
}

void KuksaBackendTest::testEngineStateEnum()
{
    CombustionEngineKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CombustionEngineBackendInterface::engineStateChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Powertrain.CombustionEngine"),
        QStringLiteral("engineState"), QString(),
        QVariant(static_cast<int>(Common::EngineRunning)));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<Common::EngineState>(), Common::EngineRunning);
}

void KuksaBackendTest::testChargingStatusEnum()
{
    TractionBatteryKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &TractionBatteryBackendInterface::chargingStatusChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Powertrain.TractionBattery"),
        QStringLiteral("chargingStatus"), QString(),
        QVariant(static_cast<int>(Common::ChargeActive)));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<Common::ChargingStatus>(), Common::ChargeActive);
}

void KuksaBackendTest::testGNSSFixTypeEnum()
{
    CurrentLocationKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CurrentLocationBackendInterface::gnssFixTypeChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Vehicle.CurrentLocation"),
        QStringLiteral("gnssFixType"), QString(),
        QVariant(static_cast<int>(Common::GNSSFix3D)));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<Common::GNSSFixType>(), Common::GNSSFix3D);
}

void KuksaBackendTest::testRoadConditionEnum()
{
    EnvironmentSensorsKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &EnvironmentSensorsBackendInterface::roadConditionChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Exterior.EnvironmentSensors"),
        QStringLiteral("roadCondition"), QString(),
        QVariant(static_cast<int>(Common::RoadWet)));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<Common::RoadSurfaceCondition>(), Common::RoadWet);
}

void KuksaBackendTest::testCrashStatusEnum()
{
    CrashDetectionKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &CrashDetectionBackendInterface::crashStatusChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Safety.CrashDetection"),
        QStringLiteral("crashStatus"), QString(),
        QVariant(static_cast<int>(Common::CrashFront)));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<Common::CrashStatus>(), Common::CrashFront);
}

// ═══════════════════════════════════════════════════════════════════
// Edge cases
// ═══════════════════════════════════════════════════════════════════

void KuksaBackendTest::testUnknownPropertyIgnored()
{
    VehicleDynamicsKuksaBackend backend(&m_client);
    QSignalSpy spy(&backend, &VehicleDynamicsBackendInterface::speedChanged);
    QVERIFY(spy.isValid());

    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Vehicle.VehicleDynamics"),
        QStringLiteral("nonexistentProperty"), QString(), QVariant(42));

    QCOMPARE(spy.count(), 0);
}

void KuksaBackendTest::testUnregisteredIidIgnored()
{
    // No backend registered for this IID - should not crash
    m_client.invokeBackendCallback(
        QStringLiteral("COVESA.VSS.Fake.Module"),
        QStringLiteral("speed"), QString(), QVariant(100));
    // If we reach here without crashing, the test passes
}

QTEST_MAIN(KuksaBackendTest)
#include "tst_kuksabackends.moc"
