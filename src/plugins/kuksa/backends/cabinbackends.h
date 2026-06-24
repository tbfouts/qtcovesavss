#ifndef CABINBACKENDS_H
#define CABINBACKENDS_H

#include <QPointer>
#include <cabinseatbackendinterface.h>
#include <cabinhvacbackendinterface.h>
#include <cabininfotainmentbackendinterface.h>
#include <cabindoorbackendinterface.h>
#include <cabinlightsbackendinterface.h>

class KuksaClient;

// ---------------------------------------------------------------------------
// CabinSeatKuksaBackend (zoned: Row1Left, Row1Right, Row2Left, Row2Right, Row2Center)
// ---------------------------------------------------------------------------

class CabinSeatKuksaBackend : public CabinSeatBackendInterface
{
    Q_OBJECT

public:
    explicit CabinSeatKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~CabinSeatKuksaBackend() override;
    void initialize() override;
    QStringList availableZones() const override;

    void setPosition(int position, const QString &zone) override;
    void setHeight(int height, const QString &zone) override;
    void setTilt(int tilt, const QString &zone) override;
    void setBackrestRecline(int backrestRecline, const QString &zone) override;
    void setBackrestLumbarSupport(int backrestLumbarSupport, const QString &zone) override;
    void setBackrestLumbarHeight(int backrestLumbarHeight, const QString &zone) override;
    void setIsHeatingOn(bool isHeatingOn, const QString &zone) override;
    void setHeatingLevel(int heatingLevel, const QString &zone) override;
    void setIsCoolingOn(bool isCoolingOn, const QString &zone) override;
    void setCoolingLevel(int coolingLevel, const QString &zone) override;
    void setIsMassageOn(bool isMassageOn, const QString &zone) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

// ---------------------------------------------------------------------------
// CabinHVACKuksaBackend (zoned: FrontLeft, FrontRight, RearLeft, RearRight)
// ---------------------------------------------------------------------------

class CabinHVACKuksaBackend : public CabinHVACBackendInterface
{
    Q_OBJECT

public:
    explicit CabinHVACKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~CabinHVACKuksaBackend() override;
    void initialize() override;
    QStringList availableZones() const override;

    void setTargetTemperature(qreal targetTemperature, const QString &zone) override;
    void setFanSpeed(int fanSpeed, const QString &zone) override;
    void setFanDirection(Common::HVACFanDirection fanDirection, const QString &zone) override;
    void setRecirculationMode(Common::RecirculationMode recirculationMode, const QString &zone) override;
    void setIsAirConditioningActive(bool isAirConditioningActive, const QString &zone) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

// ---------------------------------------------------------------------------
// CabinInfotainmentKuksaBackend (non-zoned)
// ---------------------------------------------------------------------------

class CabinInfotainmentKuksaBackend : public CabinInfotainmentBackendInterface
{
    Q_OBJECT

public:
    explicit CabinInfotainmentKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~CabinInfotainmentKuksaBackend() override;
    void initialize() override;

    void setCurrentSource(Common::MediaSource currentSource) override;
    void setVolume(int volume) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

// ---------------------------------------------------------------------------
// CabinDoorKuksaBackend (zoned: FrontLeft, FrontRight, RearLeft, RearRight)
// ---------------------------------------------------------------------------

class CabinDoorKuksaBackend : public CabinDoorBackendInterface
{
    Q_OBJECT

public:
    explicit CabinDoorKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~CabinDoorKuksaBackend() override;
    void initialize() override;
    QStringList availableZones() const override;

    void setIsLocked(bool isLocked, const QString &zone) override;
    void setIsChildLockActive(bool isChildLockActive, const QString &zone) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

// ---------------------------------------------------------------------------
// CabinLightsKuksaBackend (non-zoned)
// ---------------------------------------------------------------------------

class CabinLightsKuksaBackend : public CabinLightsBackendInterface
{
    Q_OBJECT

public:
    explicit CabinLightsKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~CabinLightsKuksaBackend() override;
    void initialize() override;

    void setAmbientLightBrightness(int ambientLightBrightness) override;
    void setAmbientLightRed(int ambientLightRed) override;
    void setAmbientLightGreen(int ambientLightGreen) override;
    void setAmbientLightBlue(int ambientLightBlue) override;
    void setIsDomeLightOn(bool isDomeLightOn) override;
    void setIsSpotlightOn(bool isSpotlightOn) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

#endif // CABINBACKENDS_H
