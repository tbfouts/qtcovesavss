#ifndef SAFETYBACKENDS_H
#define SAFETYBACKENDS_H

#include <crashdetectionbackendinterface.h>
#include <airbagsystembackendinterface.h>
#include <beltsystembackendinterface.h>

class KuksaClient;

// ---------------------------------------------------------------------------
// CrashDetectionKuksaBackend
// ---------------------------------------------------------------------------

class CrashDetectionKuksaBackend : public CrashDetectionBackendInterface
{
    Q_OBJECT

public:
    explicit CrashDetectionKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// AirbagSystemKuksaBackend (zoned: FrontLeft, FrontRight, SideLeft, SideRight, Curtain)
// ---------------------------------------------------------------------------

class AirbagSystemKuksaBackend : public AirbagSystemBackendInterface
{
    Q_OBJECT

public:
    explicit AirbagSystemKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;
    QStringList availableZones() const override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

// ---------------------------------------------------------------------------
// BeltSystemKuksaBackend (zoned: Row1Left, Row1Right, Row2Left, Row2Right, Row2Center)
// ---------------------------------------------------------------------------

class BeltSystemKuksaBackend : public BeltSystemBackendInterface
{
    Q_OBJECT

public:
    explicit BeltSystemKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;
    QStringList availableZones() const override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

#endif // SAFETYBACKENDS_H
