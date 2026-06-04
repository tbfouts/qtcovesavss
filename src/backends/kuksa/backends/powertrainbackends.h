#ifndef POWERTRAINBACKENDS_H
#define POWERTRAINBACKENDS_H

#include <powertrainstatusbackendinterface.h>
#include <combustionenginebackendinterface.h>
#include <transmissionbackendinterface.h>
#include <electricmotorbackendinterface.h>
#include <tractionbatterybackendinterface.h>
#include <fuelsystembackendinterface.h>

class KuksaClient;

// ---------------------------------------------------------------------------
// PowertrainStatusKuksaBackend
// ---------------------------------------------------------------------------

class PowertrainStatusKuksaBackend : public PowertrainStatusBackendInterface
{
    Q_OBJECT

public:
    explicit PowertrainStatusKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// CombustionEngineKuksaBackend
// ---------------------------------------------------------------------------

class CombustionEngineKuksaBackend : public CombustionEngineBackendInterface
{
    Q_OBJECT

public:
    explicit CombustionEngineKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// TransmissionKuksaBackend
// ---------------------------------------------------------------------------

class TransmissionKuksaBackend : public TransmissionBackendInterface
{
    Q_OBJECT

public:
    explicit TransmissionKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

    void setPerformanceMode(Common::TransmissionPerformanceMode performanceMode) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// ElectricMotorKuksaBackend (zoned: Motor1, Motor2)
// ---------------------------------------------------------------------------

class ElectricMotorKuksaBackend : public ElectricMotorBackendInterface
{
    Q_OBJECT

public:
    explicit ElectricMotorKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;
    QStringList availableZones() const override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// TractionBatteryKuksaBackend
// ---------------------------------------------------------------------------

class TractionBatteryKuksaBackend : public TractionBatteryBackendInterface
{
    Q_OBJECT

public:
    explicit TractionBatteryKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

    void setChargeLimit(qreal chargeLimit) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// FuelSystemKuksaBackend
// ---------------------------------------------------------------------------

class FuelSystemKuksaBackend : public FuelSystemBackendInterface
{
    Q_OBJECT

public:
    explicit FuelSystemKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

#endif // POWERTRAINBACKENDS_H
