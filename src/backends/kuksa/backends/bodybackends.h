#ifndef BODYBACKENDS_H
#define BODYBACKENDS_H

#include <bodycontrolbackendinterface.h>
#include <bodylightsbackendinterface.h>
#include <windshieldbackendinterface.h>
#include <bodymirrorsbackendinterface.h>

class KuksaClient;

// ---------------------------------------------------------------------------
// BodyControlKuksaBackend
// ---------------------------------------------------------------------------

class BodyControlKuksaBackend : public BodyControlBackendInterface
{
    Q_OBJECT

public:
    explicit BodyControlKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

    void setIsTrunkLocked(bool isTrunkLocked) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// BodyLightsKuksaBackend
// ---------------------------------------------------------------------------

class BodyLightsKuksaBackend : public BodyLightsBackendInterface
{
    Q_OBJECT

public:
    explicit BodyLightsKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

    void setLightSwitch(Common::LightSwitch lightSwitch) override;
    void setIsHighBeamOn(bool isHighBeamOn) override;
    void setIsLowBeamOn(bool isLowBeamOn) override;
    void setIsFogFrontOn(bool isFogFrontOn) override;
    void setIsFogRearOn(bool isFogRearOn) override;
    void setIsParkingOn(bool isParkingOn) override;
    void setIsHazardOn(bool isHazardOn) override;
    void setInteriorBrightness(int interiorBrightness) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// WindshieldKuksaBackend (zoned: Front, Rear)
// ---------------------------------------------------------------------------

class WindshieldKuksaBackend : public WindshieldBackendInterface
{
    Q_OBJECT

public:
    explicit WindshieldKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;
    QStringList availableZones() const override;

    void setWiperMode(Common::WiperMode wiperMode, const QString &zone) override;
    void setIsHeatingOn(bool isHeatingOn, const QString &zone) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// BodyMirrorsKuksaBackend (zoned: Left, Right)
// ---------------------------------------------------------------------------

class BodyMirrorsKuksaBackend : public BodyMirrorsBackendInterface
{
    Q_OBJECT

public:
    explicit BodyMirrorsKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;
    QStringList availableZones() const override;

    void setTilt(int tilt, const QString &zone) override;
    void setPan(int pan, const QString &zone) override;
    void setIsHeatingOn(bool isHeatingOn, const QString &zone) override;
    void setIsFolded(bool isFolded, const QString &zone) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

#endif // BODYBACKENDS_H
