#ifndef SAFETYBACKENDS_H
#define SAFETYBACKENDS_H

#include <QPointer>
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
    ~CrashDetectionKuksaBackend() override;
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

// ---------------------------------------------------------------------------
// AirbagSystemKuksaBackend (zoned: FrontLeft, FrontRight, SideLeft, SideRight, Curtain)
// ---------------------------------------------------------------------------

class AirbagSystemKuksaBackend : public AirbagSystemBackendInterface
{
    Q_OBJECT

public:
    explicit AirbagSystemKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~AirbagSystemKuksaBackend() override;
    void initialize() override;
    QStringList availableZones() const override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

// ---------------------------------------------------------------------------
// BeltSystemKuksaBackend (zoned: Row1Left, Row1Right, Row2Left, Row2Right, Row2Center)
// ---------------------------------------------------------------------------

class BeltSystemKuksaBackend : public BeltSystemBackendInterface
{
    Q_OBJECT

public:
    explicit BeltSystemKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~BeltSystemKuksaBackend() override;
    void initialize() override;
    QStringList availableZones() const override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

#endif // SAFETYBACKENDS_H
