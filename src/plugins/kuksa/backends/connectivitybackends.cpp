#include "connectivitybackends.h"
#include "../kuksaclient.h"
#include "../vsspathmapping.h"

#include <common.h>
#include "../enumvalidation.h"

// ===========================================================================
// ConnectivityControlKuksaBackend
// ===========================================================================

ConnectivityControlKuksaBackend::ConnectivityControlKuksaBackend(KuksaClient *client, QObject *parent)
    : ConnectivityControlBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Connectivity.ConnectivityControl"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

ConnectivityControlKuksaBackend::~ConnectivityControlKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Connectivity.ConnectivityControl"));
}

void ConnectivityControlKuksaBackend::initialize()
{
    emit initializationDone();
}

void ConnectivityControlKuksaBackend::setIsWifiHotspotEnabled(bool isWifiHotspotEnabled)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Connectivity.ConnectivityControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isWifiHotspotEnabled")), isWifiHotspotEnabled);
}

void ConnectivityControlKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("cellularStatus"))
        emit cellularStatusChanged(validatedEnum<Common::ConnectivityStatus>(value, 3));
    else if (property == QLatin1String("cellularSignalStrength"))
        emit cellularSignalStrengthChanged(value.toInt());
    else if (property == QLatin1String("cellularNetworkType"))
        emit cellularNetworkTypeChanged(value.toString());
    else if (property == QLatin1String("cellularOperator"))
        emit cellularOperatorChanged(value.toString());
    else if (property == QLatin1String("wifiStatus"))
        emit wifiStatusChanged(validatedEnum<Common::ConnectivityStatus>(value, 3));
    else if (property == QLatin1String("wifiSignalStrength"))
        emit wifiSignalStrengthChanged(value.toInt());
    else if (property == QLatin1String("wifiSSID"))
        emit wifiSSIDChanged(value.toString());
    else if (property == QLatin1String("isWifiHotspotEnabled"))
        emit isWifiHotspotEnabledChanged(value.toBool());
    else if (property == QLatin1String("bluetoothStatus"))
        emit bluetoothStatusChanged(validatedEnum<Common::ConnectivityStatus>(value, 3));
    else if (property == QLatin1String("bluetoothPairedDevices"))
        emit bluetoothPairedDevicesChanged(value.toInt());
    else if (property == QLatin1String("isBluetoothEnabled"))
        emit isBluetoothEnabledChanged(value.toBool());
}
