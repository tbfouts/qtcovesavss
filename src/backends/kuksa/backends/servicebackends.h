#ifndef SERVICEBACKENDS_H
#define SERVICEBACKENDS_H

#include <QPointer>
#include <servicestatusbackendinterface.h>

class KuksaClient;

// ---------------------------------------------------------------------------
// ServiceStatusKuksaBackend
// ---------------------------------------------------------------------------

class ServiceStatusKuksaBackend : public ServiceStatusBackendInterface
{
    Q_OBJECT

public:
    explicit ServiceStatusKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~ServiceStatusKuksaBackend() override;
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

#endif // SERVICEBACKENDS_H
