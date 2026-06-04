#ifndef DRIVERBACKENDS_H
#define DRIVERBACKENDS_H

#include <drivermonitoringbackendinterface.h>

class KuksaClient;

// ---------------------------------------------------------------------------
// DriverMonitoringKuksaBackend
// ---------------------------------------------------------------------------

class DriverMonitoringKuksaBackend : public DriverMonitoringBackendInterface
{
    Q_OBJECT

public:
    explicit DriverMonitoringKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

#endif // DRIVERBACKENDS_H
