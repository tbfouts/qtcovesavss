#ifndef DRIVERBACKENDS_H
#define DRIVERBACKENDS_H

#include <QPointer>
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
    ~DriverMonitoringKuksaBackend() override;
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

#endif // DRIVERBACKENDS_H
