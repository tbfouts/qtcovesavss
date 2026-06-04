#ifndef MOTIONMANAGEMENTBACKENDS_H
#define MOTIONMANAGEMENTBACKENDS_H

#include <brakecontrolbackendinterface.h>
#include <steeringcontrolbackendinterface.h>
#include <suspensioncontrolbackendinterface.h>

class KuksaClient;

// ---------------------------------------------------------------------------
// BrakeControlKuksaBackend
// ---------------------------------------------------------------------------

class BrakeControlKuksaBackend : public BrakeControlBackendInterface
{
    Q_OBJECT

public:
    explicit BrakeControlKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

    void setPedalPositionTarget(int pedalPositionTarget) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// SteeringControlKuksaBackend
// ---------------------------------------------------------------------------

class SteeringControlKuksaBackend : public SteeringControlBackendInterface
{
    Q_OBJECT

public:
    explicit SteeringControlKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

    void setSteeringAngleTarget(qreal steeringAngleTarget) override;
    void setIsSteerByWireEnabled(bool isSteerByWireEnabled) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// SuspensionControlKuksaBackend (zoned: FrontLeft, FrontRight, RearLeft, RearRight)
// ---------------------------------------------------------------------------

class SuspensionControlKuksaBackend : public SuspensionControlBackendInterface
{
    Q_OBJECT

public:
    explicit SuspensionControlKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;
    QStringList availableZones() const override;

    void setHeightTarget(int heightTarget, const QString &zone) override;
    void setMode(Common::SuspensionMode mode, const QString &zone) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

#endif // MOTIONMANAGEMENTBACKENDS_H
