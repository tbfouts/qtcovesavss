#ifndef KUKSAPLUGIN_H
#define KUKSAPLUGIN_H

#include <QIfServiceInterface>
#include <QObject>

#include <memory>

class KuksaClient;
class QIfFeatureInterface;

class KuksaPlugin : public QObject, public QIfServiceInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QIfServiceInterface_iid FILE "plugin.json")
    Q_INTERFACES(QIfServiceInterface)

public:
    explicit KuksaPlugin(QObject *parent = nullptr);
    ~KuksaPlugin() override;

    QStringList interfaces() const override;
    QIfFeatureInterface *interfaceInstance(const QString &interfaceName) const override;
    void updateServiceSettings(const QVariantMap &settings) override;

private:
    void createBackends();
    void connectToDatabroker(const QString &host, int port);

    std::unique_ptr<KuksaClient> m_client;
    QHash<QString, QIfFeatureInterface *> m_backends;
    QStringList m_interfaces;
};

#endif // KUKSAPLUGIN_H
