#ifndef KUKSACLIENT_H
#define KUKSACLIENT_H

#include <QObject>
#include <QUrl>
#include <QVariant>
#include <QTimer>

#include <functional>
#include <memory>

class QGrpcHttp2Channel;
class QGrpcServerStream;

namespace kuksa::val::v2 {
namespace VAL { class Client; }
class SubscribeResponse;
class ListMetadataResponse;
class Value;
}

class KuksaClient : public QObject
{
    Q_OBJECT

public:
    explicit KuksaClient(QObject *parent = nullptr);
    ~KuksaClient() override;

    void setHostUrl(const QUrl &url);
    QUrl hostUrl() const { return m_url; }

    void subscribe(const QStringList &vssPaths);
    void actuate(const QString &vssPath, const QVariant &value);

    bool isConnected() const { return m_connected; }

    // Register a callback for a specific IID. Only updates for VSS paths
    // mapped to this IID will be delivered.
    using ValueCallback = std::function<void(const QString &property,
                                             const QString &zone,
                                             const QVariant &value)>;
    void registerBackend(const QString &iid, ValueCallback callback);

    // Invoke a registered backend callback directly (for testing and
    // alternative data sources that bypass gRPC/VSS path resolution).
    void invokeBackendCallback(const QString &iid, const QString &property,
                               const QString &zone, const QVariant &value);

Q_SIGNALS:
    void connectedChanged(bool connected);
    void errorOccurred(const QString &message);

private:
    void connectToServer();
    void discoverAndSubscribe();
    void startSubscription(const QStringList &vssPaths);
    void handleSubscribeResponse(const kuksa::val::v2::SubscribeResponse &response);
    void onStreamFinished();
    void scheduleReconnect();

    static QVariant convertValue(const kuksa::val::v2::Value &value);
    static kuksa::val::v2::Value toProtoValue(const QVariant &value);

    QUrl m_url;
    std::shared_ptr<QGrpcHttp2Channel> m_channel;
    std::unique_ptr<kuksa::val::v2::VAL::Client> m_client;
    std::unique_ptr<QGrpcServerStream> m_stream;
    QStringList m_subscribedPaths;
    // IID -> callback for targeted dispatch
    QHash<QString, ValueCallback> m_backendCallbacks;
    QTimer m_reconnectTimer;
    int m_reconnectDelay = 1000;
    bool m_connected = false;
};

#endif // KUKSACLIENT_H
