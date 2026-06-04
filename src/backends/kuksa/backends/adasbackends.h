#ifndef ADASBACKENDS_H
#define ADASBACKENDS_H

#include <adascontrolbackendinterface.h>
#include <cruisecontrolbackendinterface.h>
#include <obstacledetectionbackendinterface.h>
#include <common.h>

class KuksaClient;

// ---------------------------------------------------------------------------
// ADASControlKuksaBackend
// ---------------------------------------------------------------------------

class ADASControlKuksaBackend : public ADASControlBackendInterface
{
    Q_OBJECT

public:
    explicit ADASControlKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

    void setIsAEBEnabled(bool isAEBEnabled) override;
    void setIsEBDEnabled(bool isEBDEnabled) override;
    void setIsESCEnabled(bool isESCEnabled) override;
    void setIsTCSEnabled(bool isTCSEnabled) override;
    void setIsLaneDepartureEnabled(bool isLaneDepartureEnabled) override;
    void setIsBlindSpotEnabled(bool isBlindSpotEnabled) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// CruiseControlKuksaBackend
// ---------------------------------------------------------------------------

class CruiseControlKuksaBackend : public CruiseControlBackendInterface
{
    Q_OBJECT

public:
    explicit CruiseControlKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

    void setIsEnabled(bool isEnabled) override;
    void setSpeedSet(qreal speedSet) override;
    void setTargetGap(qreal targetGap) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// ObstacleDetectionKuksaBackend (zoned: Front, Rear, Left, Right)
// ---------------------------------------------------------------------------

class ObstacleDetectionKuksaBackend : public ObstacleDetectionBackendInterface
{
    Q_OBJECT

public:
    explicit ObstacleDetectionKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;
    QStringList availableZones() const override;

    void setIsEnabled(bool isEnabled, const QString &zone) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

#endif // ADASBACKENDS_H
