#include "kuksaplugin.h"
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

#include <QUrl>

KuksaPlugin::KuksaPlugin(QObject *parent)
    : QObject(parent)
    , m_client(std::make_unique<KuksaClient>())
{
    createBackends();

    // Auto-connect from environment variables if set
    const QByteArray envHost = qgetenv("KUKSA_HOST");
    const QByteArray envPort = qgetenv("KUKSA_PORT");
    const QByteArray envTls = qgetenv("KUKSA_TLS");
    if (!envHost.isEmpty()) {
        const QString host = QString::fromUtf8(envHost);
        const int port = envPort.isEmpty() ? 55555 : envPort.toInt();
        const bool tls = (envTls == "1" || envTls == "true");
        connectToDatabroker(host, port, tls);
    }
}

KuksaPlugin::~KuksaPlugin() = default;

void KuksaPlugin::createBackends()
{
    auto reg = [this](const QString &iid, QIfFeatureInterface *backend) {
        m_backends.insert(iid, backend);
        m_interfaces.append(iid);
    };

    // Vehicle
    reg(QStringLiteral("COVESA.VSS.Vehicle.VehicleIdentification"),
        new VehicleIdentificationKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Vehicle.VehicleDynamics"),
        new VehicleDynamicsKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Vehicle.CurrentLocation"),
        new CurrentLocationKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Vehicle.LowVoltageBattery"),
        new LowVoltageBatteryKuksaBackend(m_client.get(), this));

    // Powertrain
    reg(QStringLiteral("COVESA.VSS.Powertrain.PowertrainStatus"),
        new PowertrainStatusKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Powertrain.CombustionEngine"),
        new CombustionEngineKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Powertrain.Transmission"),
        new TransmissionKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Powertrain.ElectricMotor"),
        new ElectricMotorKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Powertrain.TractionBattery"),
        new TractionBatteryKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Powertrain.FuelSystem"),
        new FuelSystemKuksaBackend(m_client.get(), this));

    // Body
    reg(QStringLiteral("COVESA.VSS.Body.BodyControl"),
        new BodyControlKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Body.BodyLights"),
        new BodyLightsKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Body.Windshield"),
        new WindshieldKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Body.BodyMirrors"),
        new BodyMirrorsKuksaBackend(m_client.get(), this));

    // Cabin
    reg(QStringLiteral("COVESA.VSS.Cabin.CabinSeat"),
        new CabinSeatKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Cabin.CabinHVAC"),
        new CabinHVACKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Cabin.CabinInfotainment"),
        new CabinInfotainmentKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Cabin.CabinDoor"),
        new CabinDoorKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Cabin.CabinLights"),
        new CabinLightsKuksaBackend(m_client.get(), this));

    // ADAS
    reg(QStringLiteral("COVESA.VSS.ADAS.ADASControl"),
        new ADASControlKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.ADAS.CruiseControl"),
        new CruiseControlKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.ADAS.ObstacleDetection"),
        new ObstacleDetectionKuksaBackend(m_client.get(), this));

    // Chassis
    reg(QStringLiteral("COVESA.VSS.Chassis.ChassisAxle"),
        new ChassisAxleKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Chassis.ChassisWheel"),
        new ChassisWheelKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Chassis.ChassisSteering"),
        new ChassisSteeringKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Chassis.ChassisBrake"),
        new ChassisBrakeKuksaBackend(m_client.get(), this));

    // Driver
    reg(QStringLiteral("COVESA.VSS.Driver.DriverMonitoring"),
        new DriverMonitoringKuksaBackend(m_client.get(), this));

    // Safety
    reg(QStringLiteral("COVESA.VSS.Safety.CrashDetection"),
        new CrashDetectionKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Safety.AirbagSystem"),
        new AirbagSystemKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.Safety.BeltSystem"),
        new BeltSystemKuksaBackend(m_client.get(), this));

    // Exterior
    reg(QStringLiteral("COVESA.VSS.Exterior.EnvironmentSensors"),
        new EnvironmentSensorsKuksaBackend(m_client.get(), this));

    // Service
    reg(QStringLiteral("COVESA.VSS.Service.ServiceStatus"),
        new ServiceStatusKuksaBackend(m_client.get(), this));

    // Connectivity
    reg(QStringLiteral("COVESA.VSS.Connectivity.ConnectivityControl"),
        new ConnectivityControlKuksaBackend(m_client.get(), this));

    // MotionManagement
    reg(QStringLiteral("COVESA.VSS.MotionManagement.BrakeControl"),
        new BrakeControlKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.MotionManagement.SteeringControl"),
        new SteeringControlKuksaBackend(m_client.get(), this));
    reg(QStringLiteral("COVESA.VSS.MotionManagement.SuspensionControl"),
        new SuspensionControlKuksaBackend(m_client.get(), this));
}

QStringList KuksaPlugin::interfaces() const
{
    return m_interfaces;
}

QIfFeatureInterface *KuksaPlugin::interfaceInstance(const QString &interfaceName) const
{
    return m_backends.value(interfaceName, nullptr);
}

void KuksaPlugin::updateServiceSettings(const QVariantMap &settings)
{
    const QString host = settings.value(QStringLiteral("host"),
                                        QStringLiteral("localhost")).toString();
    const int port = settings.value(QStringLiteral("port"), 55555).toInt();
    const bool tls = settings.value(QStringLiteral("tls"), false).toBool();
    connectToDatabroker(host, port, tls);
}

void KuksaPlugin::connectToDatabroker(const QString &host, int port, bool tls)
{
    const QString scheme = tls ? QStringLiteral("https") : QStringLiteral("http");
    const QUrl url(QStringLiteral("%1://%2:%3").arg(scheme, host).arg(port));
    if (m_client->hostUrl() == url)
        return;
    m_client->setHostUrl(url);
    m_client->subscribe(VssPathMapping::allVssPaths());
}
