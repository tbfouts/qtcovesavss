#ifndef SERVICEBACKENDS_H
#define SERVICEBACKENDS_H

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
    void initialize() override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    KuksaClient *m_client;
};

#endif // SERVICEBACKENDS_H
