#ifndef EXTERIORBACKENDS_H
#define EXTERIORBACKENDS_H

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
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

#endif // EXTERIORBACKENDS_H
