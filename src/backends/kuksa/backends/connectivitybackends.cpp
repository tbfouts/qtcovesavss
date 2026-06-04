#include "connectivitybackends.h"
#include "../kuksaclient.h"
#include "../vsspathmapping.h"

#include <common.h>

// ===========================================================================
// ConnectivityControlKuksaBackend
// ===========================================================================

ConnectivityControlKuksaBackend::ConnectivityControlKuksaBackend(KuksaClient *client, QObject *parent)
    : ConnectivityControlBackendInterface(parent)
    , m_client(client)
{
    m_client->registerBackend(QStringLiteral("COVESA.VSS.Connectivity.ConnectivityControl"),
        [this](const QString &property, const QString &zone, const QVariant &value) {
            onVssValue(property, zone, value);
        });
}

void ConnectivityControlKuksaBackend::initialize()
{
    emit initializationDone();
}

void ConnectivityControlKuksaBackend::setIsWifiHotspotEnabled(bool isWifiHotspotEnabled)
{
    static const QString iid = QStringLiteral("COVESA.VSS.Connectivity.ConnectivityControl");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isWifiHotspotEnabled")), isWifiHotspotEnabled);
}

void ConnectivityControlKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("cellularStatus"))
        emit cellularStatusChanged(static_cast<Common::ConnectivityStatus>(value.toInt()));
    else if (property == QLatin1String("cellularSignalStrength"))
        emit cellularSignalStrengthChanged(value.toInt());
    else if (property == QLatin1String("cellularNetworkType"))
        emit cellularNetworkTypeChanged(value.toString());
    else if (property == QLatin1String("cellularOperator"))
        emit cellularOperatorChanged(value.toString());
    else if (property == QLatin1String("wifiStatus"))
        emit wifiStatusChanged(static_cast<Common::ConnectivityStatus>(value.toInt()));
    else if (property == QLatin1String("wifiSignalStrength"))
        emit wifiSignalStrengthChanged(value.toInt());
    else if (property == QLatin1String("wifiSSID"))
        emit wifiSSIDChanged(value.toString());
    else if (property == QLatin1String("isWifiHotspotEnabled"))
        emit isWifiHotspotEnabledChanged(value.toBool());
    else if (property == QLatin1String("bluetoothStatus"))
        emit bluetoothStatusChanged(static_cast<Common::ConnectivityStatus>(value.toInt()));
    else if (property == QLatin1String("bluetoothPairedDevices"))
        emit bluetoothPairedDevicesChanged(value.toInt());
    else if (property == QLatin1String("isBluetoothEnabled"))
        emit isBluetoothEnabledChanged(value.toBool());
}
