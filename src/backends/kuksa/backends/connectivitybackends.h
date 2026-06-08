#ifndef CONNECTIVITYBACKENDS_H
#define CONNECTIVITYBACKENDS_H

#include <QPointer>
#include <connectivitycontrolbackendinterface.h>

class KuksaClient;

// ---------------------------------------------------------------------------
// ConnectivityControlKuksaBackend
// ---------------------------------------------------------------------------

class ConnectivityControlKuksaBackend : public ConnectivityControlBackendInterface
{
    Q_OBJECT

public:
    explicit ConnectivityControlKuksaBackend(KuksaClient *client, QObject *parent = nullptr);
    ~ConnectivityControlKuksaBackend() override;
    void initialize() override;

    void setIsWifiHotspotEnabled(bool isWifiHotspotEnabled) override;

private:
    void onVssValue(const QString &property, const QString &zone, const QVariant &value);

    QPointer<KuksaClient> m_client;
};

#endif // CONNECTIVITYBACKENDS_H
