#ifndef EXTERIORBACKENDS_H
#define EXTERIORBACKENDS_H

#include <QPointer>
#include <environmentsensorsbackendinterface.h>

class KuksaClient;

// ---------------------------------------------------------------------------
// EnvironmentSensorsKuksaBackend
// ---------------------------------------------------------------------------

class EnvironmentSensorsKuksaBackend : public EnvironmentSensorsBackendInterface
{
    Q_OBJECT

public:
    explicit EnvironmentSensorsKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~EnvironmentSensorsKuksaBackend() override;
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

#endif // EXTERIORBACKENDS_H
