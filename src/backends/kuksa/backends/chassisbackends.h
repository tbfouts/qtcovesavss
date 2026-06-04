#ifndef CHASSISBACKENDS_H
#define CHASSISBACKENDS_H

#include <chassisaxlebackendinterface.h>
#include <chassiswheelbackendinterface.h>
#include <chassissteeringbackendinterface.h>
#include <chassisbrakebackendinterface.h>
#include <common.h>

class KuksaClient;

// ---------------------------------------------------------------------------
// ChassisAxleKuksaBackend (zoned: Front, Rear)
// ---------------------------------------------------------------------------

class ChassisAxleKuksaBackend : public ChassisAxleBackendInterface
{
    Q_OBJECT

public:
    explicit ChassisAxleKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;
    QStringList availableZones() const override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// ChassisWheelKuksaBackend (zoned: FrontLeft, FrontRight, RearLeft, RearRight)
// ---------------------------------------------------------------------------

class ChassisWheelKuksaBackend : public ChassisWheelBackendInterface
{
    Q_OBJECT

public:
    explicit ChassisWheelKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;
    QStringList availableZones() const override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// ChassisSteeringKuksaBackend
// ---------------------------------------------------------------------------

class ChassisSteeringKuksaBackend : public ChassisSteeringBackendInterface
{
    Q_OBJECT

public:
    explicit ChassisSteeringKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// ChassisBrakeKuksaBackend
// ---------------------------------------------------------------------------

class ChassisBrakeKuksaBackend : public ChassisBrakeBackendInterface
{
    Q_OBJECT

public:
    explicit ChassisBrakeKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

    void setIsParkingBrakeEngaged(bool isParkingBrakeEngaged) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

#endif // CHASSISBACKENDS_H
