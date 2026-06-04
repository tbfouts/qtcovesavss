#include "kuksaclient.h"

#include <QGrpcHttp2Channel>
#include <QGrpcChannelOptions>
#include <QGrpcServerStream>
#include <QGrpcStatus>

#include "types.qpb.h"
#include "val.qpb.h"
#include "val_client.grpc.qpb.h"
#include "vsspathmapping.h"

#include <QGrpcCallReply>
#include <QDebug>

using namespace kuksa::val::v2;

KuksaClient::KuksaClient(QObject *parent)
    : QObject(parent)
{
    m_reconnectTimer.setSingleShot(true);
    connect(&m_reconnectTimer, &QTimer::timeout, this, &KuksaClient::connectToServer);
}

KuksaClient::~KuksaClient() = default;

void KuksaClient::setHostUrl(const QUrl &url)
{
    if (m_url == url)
        return;
    m_url = url;
    m_stream.reset();
    m_client.reset();
    m_channel.reset();
    m_reconnectDelay = 1000;
    if (m_url.isValid())
        connectToServer();
}

void KuksaClient::connectToServer()
{
    if (!m_url.isValid())
        return;

    qDebug() << "KuksaClient: connecting to" << m_url.toString();
    m_channel = std::make_shared<QGrpcHttp2Channel>(m_url);
    m_client = std::make_unique<VAL::Client>();
    m_client->attachChannel(m_channel);

    if (!m_subscribedPaths.isEmpty())
        subscribe(m_subscribedPaths);
}

void KuksaClient::subscribe(const QStringList &vssPaths)
{
    const QSet<QString> unique(vssPaths.cbegin(), vssPaths.cend());
    m_subscribedPaths = QStringList(unique.cbegin(), unique.cend());

    if (!m_client) {
        if (m_url.isValid())
            connectToServer();
        return;
    }

    // First discover which paths actually exist in the databroker
    discoverAndSubscribe();
}

void KuksaClient::discoverAndSubscribe()
{
    ListMetadataRequest metaReq;
    metaReq.setRoot(QStringLiteral("Vehicle"));

    auto reply = m_client->ListMetadata(metaReq);
    const auto *replyPtr = reply.get();
    connect(replyPtr, &QGrpcCallReply::finished, this,
            [this, reply = std::move(reply)](const QGrpcStatus &status) {
        if (!status.isOk()) {
            qWarning() << "KuksaClient: ListMetadata failed:" << status.message()
                       << "- subscribing to all paths anyway";
            startSubscription(m_subscribedPaths);
            return;
        }

        const auto response = reply->read<ListMetadataResponse>();
        if (!response) {
            startSubscription(m_subscribedPaths);
            return;
        }

        // Build set of known paths from metadata
        QSet<QString> knownPaths;
        for (const auto &meta : response->metadata())
            knownPaths.insert(meta.path());

        // Filter our paths to only those that exist
        QStringList validPaths;
        for (const QString &path : m_subscribedPaths) {
            if (knownPaths.contains(path))
                validPaths.append(path);
        }

        qDebug() << "KuksaClient:" << validPaths.size() << "of"
                 << m_subscribedPaths.size() << "VSS paths found in databroker";

        if (validPaths.isEmpty()) {
            qWarning() << "KuksaClient: no matching paths in databroker";
            return;
        }

        startSubscription(validPaths);
    }, Qt::SingleShotConnection);
}

void KuksaClient::startSubscription(const QStringList &vssPaths)
{
    SubscribeRequest request;
    request.setSignalPaths(vssPaths);
    request.setBufferSize(0);

    qDebug() << "KuksaClient: subscribing to" << vssPaths.size() << "VSS paths";

    m_stream = m_client->Subscribe(request);

    connect(m_stream.get(), &QGrpcServerStream::messageReceived,
            this, [this]() {
        if (const auto response = m_stream->read<SubscribeResponse>())
            handleSubscribeResponse(*response);
    });

    connect(m_stream.get(), &QGrpcServerStream::finished,
            this, [this](const QGrpcStatus &status) {
        if (!status.isOk())
            qWarning() << "KuksaClient: stream error:" << status.message();
        onStreamFinished();
    });

    if (!m_connected) {
        m_connected = true;
        m_reconnectDelay = 1000;
        emit connectedChanged(true);
    }
}

void KuksaClient::registerBackend(const QString &iid, ValueCallback callback)
{
    m_backendCallbacks.insert(iid, std::move(callback));
}

void KuksaClient::invokeBackendCallback(const QString &iid, const QString &property,
                                         const QString &zone, const QVariant &value)
{
    auto it = m_backendCallbacks.constFind(iid);
    if (it != m_backendCallbacks.constEnd())
        it.value()(property, zone, value);
}

void KuksaClient::handleSubscribeResponse(const SubscribeResponse &response)
{
    const auto &entries = response.entries();
    for (auto it = entries.cbegin(); it != entries.cend(); ++it) {
        const QString &path = it.key();
        const Datapoint &dp = it.value();
        if (!dp.hasValue())
            continue;
        const QVariant val = convertValue(dp.value());
        if (!val.isValid())
            continue;

        // Dispatch to registered backends by IID
        const auto mappedEntries = VssPathMapping::entries(path);
        for (const auto &e : mappedEntries) {
            auto cbIt = m_backendCallbacks.constFind(e.iid);
            if (cbIt != m_backendCallbacks.constEnd())
                cbIt.value()(e.property, e.zone, val);
        }
    }
}

void KuksaClient::onStreamFinished()
{
    if (m_connected) {
        m_connected = false;
        emit connectedChanged(false);
    }
    m_stream.reset();
    scheduleReconnect();
}

void KuksaClient::scheduleReconnect()
{
    m_reconnectTimer.start(m_reconnectDelay);
    m_reconnectDelay = qMin(m_reconnectDelay * 2, 30000);
}

void KuksaClient::actuate(const QString &vssPath, const QVariant &value)
{
    if (!m_client) {
        emit errorOccurred(QStringLiteral("Not connected to KUKSA Databroker"));
        return;
    }

    ActuateRequest request;
    SignalID signalId;
    signalId.setPath(vssPath);
    request.setSignalId(signalId);
    request.setValue(toProtoValue(value));

    auto reply = m_client->Actuate(request);
    const auto *replyPtr = reply.get();
    connect(replyPtr, &QGrpcCallReply::finished, replyPtr,
            [this, reply = std::move(reply), vssPath](const QGrpcStatus &status) {
        if (!status.isOk())
            emit errorOccurred(QStringLiteral("Actuate failed for %1: %2")
                               .arg(vssPath, status.message()));
    }, Qt::SingleShotConnection);
}

QVariant KuksaClient::convertValue(const Value &value)
{
    if (value.hasString())
        return QVariant(value.string());
    if (value.hasBool_())
        return QVariant(value.bool_());
    if (value.hasInt32())
        return QVariant(static_cast<int>(value.int32()));
    if (value.hasInt64())
        return QVariant(static_cast<qlonglong>(value.int64()));
    if (value.hasUint32())
        return QVariant(static_cast<uint>(value.uint32()));
    if (value.hasUint64())
        return QVariant(static_cast<qulonglong>(value.uint64()));
    if (value.hasFloat_())
        return QVariant(static_cast<double>(value.float_()));
    if (value.hasDouble_())
        return QVariant(value.double_());

    return QVariant();
}

kuksa::val::v2::Value KuksaClient::toProtoValue(const QVariant &value)
{
    kuksa::val::v2::Value v;

    switch (value.typeId()) {
    case QMetaType::Bool:
        v.setBool_(value.toBool());
        break;
    case QMetaType::Int:
        v.setInt32(value.toInt());
        break;
    case QMetaType::LongLong:
        v.setInt64(value.toLongLong());
        break;
    case QMetaType::UInt:
        v.setUint32(value.toUInt());
        break;
    case QMetaType::ULongLong:
        v.setUint64(value.toULongLong());
        break;
    case QMetaType::Float:
        v.setFloat_(value.toFloat());
        break;
    case QMetaType::Double:
        v.setDouble_(value.toDouble());
        break;
    case QMetaType::QString:
        v.setString(value.toString());
        break;
    default:
        // Try numeric conversion for enum types
        if (value.canConvert<int>())
            v.setInt32(value.toInt());
        else if (value.canConvert<double>())
            v.setDouble_(value.toDouble());
        else
            v.setString(value.toString());
        break;
    }

    return v;
}
