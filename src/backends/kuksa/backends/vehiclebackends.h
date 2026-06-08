#ifndef VEHICLEBACKENDS_H
#define VEHICLEBACKENDS_H

#include <QPointer>

#include <vehicleidentificationbackendinterface.h>
#include <vehicledynamicsbackendinterface.h>
#include <currentlocationbackendinterface.h>
#include <lowvoltagebatterybackendinterface.h>

class KuksaClient;

// ---------------------------------------------------------------------------
// VehicleIdentificationKuksaBackend
// ---------------------------------------------------------------------------

class VehicleIdentificationKuksaBackend : public VehicleIdentificationBackendInterface
{
    Q_OBJECT

public:
    explicit VehicleIdentificationKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~VehicleIdentificationKuksaBackend() override;
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

// ---------------------------------------------------------------------------
// VehicleDynamicsKuksaBackend
// ---------------------------------------------------------------------------

class VehicleDynamicsKuksaBackend : public VehicleDynamicsBackendInterface
{
    Q_OBJECT

public:
    explicit VehicleDynamicsKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~VehicleDynamicsKuksaBackend() override;
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

// ---------------------------------------------------------------------------
// CurrentLocationKuksaBackend
// ---------------------------------------------------------------------------

class CurrentLocationKuksaBackend : public CurrentLocationBackendInterface
{
    Q_OBJECT

public:
    explicit CurrentLocationKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~CurrentLocationKuksaBackend() override;
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

// ---------------------------------------------------------------------------
// LowVoltageBatteryKuksaBackend
// ---------------------------------------------------------------------------

class LowVoltageBatteryKuksaBackend : public LowVoltageBatteryBackendInterface
{
    Q_OBJECT

public:
    explicit LowVoltageBatteryKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~LowVoltageBatteryKuksaBackend() override;
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

#endif // VEHICLEBACKENDS_H
