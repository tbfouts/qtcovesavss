#include "cabinbackends.h"
#include "../kuksaclient.h"
#include "../vsspathmapping.h"

#include <common.h>
#include "../enumvalidation.h"

// ===========================================================================
// CabinSeatKuksaBackend
// ===========================================================================

CabinSeatKuksaBackend::CabinSeatKuksaBackend(KuksaClient *client, QObject *parent)
    : CabinSeatBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Cabin.CabinSeat"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

CabinSeatKuksaBackend::~CabinSeatKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Cabin.CabinSeat"));
}

void CabinSeatKuksaBackend::initialize()
{
    emit initializationDone();
}

QStringList CabinSeatKuksaBackend::availableZones() const
{
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    return VssPathMapping::zones(iid);
}

void CabinSeatKuksaBackend::setPosition(int position, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("position"), zone), position);
}

void CabinSeatKuksaBackend::setHeight(int height, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("height"), zone), height);
}

void CabinSeatKuksaBackend::setTilt(int tilt, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("tilt"), zone), tilt);
}

void CabinSeatKuksaBackend::setBackrestRecline(int backrestRecline, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("backrestRecline"), zone), backrestRecline);
}

void CabinSeatKuksaBackend::setBackrestLumbarSupport(int backrestLumbarSupport, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("backrestLumbarSupport"), zone), backrestLumbarSupport);
}

void CabinSeatKuksaBackend::setBackrestLumbarHeight(int backrestLumbarHeight, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("backrestLumbarHeight"), zone), backrestLumbarHeight);
}

void CabinSeatKuksaBackend::setIsHeatingOn(bool isHeatingOn, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isHeatingOn"), zone), isHeatingOn);
}

void CabinSeatKuksaBackend::setHeatingLevel(int heatingLevel, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("heatingLevel"), zone), heatingLevel);
}

void CabinSeatKuksaBackend::setIsCoolingOn(bool isCoolingOn, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isCoolingOn"), zone), isCoolingOn);
}

void CabinSeatKuksaBackend::setCoolingLevel(int coolingLevel, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("coolingLevel"), zone), coolingLevel);
}

void CabinSeatKuksaBackend::setIsMassageOn(bool isMassageOn, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isMassageOn"), zone), isMassageOn);
}

void CabinSeatKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    if (property == QLatin1String("position"))
        emit positionChanged(value.toInt(), zone);
    else if (property == QLatin1String("height"))
        emit heightChanged(value.toInt(), zone);
    else if (property == QLatin1String("tilt"))
        emit tiltChanged(value.toInt(), zone);
    else if (property == QLatin1String("backrestRecline"))
        emit backrestReclineChanged(value.toInt(), zone);
    else if (property == QLatin1String("backrestLumbarSupport"))
        emit backrestLumbarSupportChanged(value.toInt(), zone);
    else if (property == QLatin1String("backrestLumbarHeight"))
        emit backrestLumbarHeightChanged(value.toInt(), zone);
    else if (property == QLatin1String("isHeatingOn"))
        emit isHeatingOnChanged(value.toBool(), zone);
    else if (property == QLatin1String("heatingLevel"))
        emit heatingLevelChanged(value.toInt(), zone);
    else if (property == QLatin1String("isCoolingOn"))
        emit isCoolingOnChanged(value.toBool(), zone);
    else if (property == QLatin1String("coolingLevel"))
        emit coolingLevelChanged(value.toInt(), zone);
    else if (property == QLatin1String("isMassageOn"))
        emit isMassageOnChanged(value.toBool(), zone);
    else if (property == QLatin1String("isOccupied"))
        emit isOccupiedChanged(value.toBool(), zone);
    else if (property == QLatin1String("occupantStatus"))
        emit occupantStatusChanged(validatedEnum<Common::SeatOccupancyStatus>(value, 3), zone);
    else if (property == QLatin1String("isSeatbeltFastened"))
        emit isSeatbeltFastenedChanged(value.toBool(), zone);
}

// ===========================================================================
// CabinHVACKuksaBackend
// ===========================================================================

CabinHVACKuksaBackend::CabinHVACKuksaBackend(KuksaClient *client, QObject *parent)
    : CabinHVACBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Cabin.CabinHVAC"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

CabinHVACKuksaBackend::~CabinHVACKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Cabin.CabinHVAC"));
}

void CabinHVACKuksaBackend::initialize()
{
    emit initializationDone();
}

QStringList CabinHVACKuksaBackend::availableZones() const
{
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinHVAC");
    return VssPathMapping::zones(iid);
}

void CabinHVACKuksaBackend::setTargetTemperature(qreal targetTemperature, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinHVAC");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("targetTemperature"), zone), targetTemperature);
}

void CabinHVACKuksaBackend::setFanSpeed(int fanSpeed, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinHVAC");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("fanSpeed"), zone), fanSpeed);
}

void CabinHVACKuksaBackend::setFanDirection(Common::HVACFanDirection fanDirection, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinHVAC");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("fanDirection"), zone), static_cast<int>(fanDirection));
}

void CabinHVACKuksaBackend::setRecirculationMode(Common::RecirculationMode recirculationMode, const QString &zone)
{
    if (!m_client) return;
    Q_UNUSED(zone);
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinHVAC");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("recirculationMode")), static_cast<int>(recirculationMode));
}

void CabinHVACKuksaBackend::setIsAirConditioningActive(bool isAirConditioningActive, const QString &zone)
{
    if (!m_client) return;
    Q_UNUSED(zone);
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinHVAC");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isAirConditioningActive")), isAirConditioningActive);
}

void CabinHVACKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    if (property == QLatin1String("targetTemperature"))
        emit targetTemperatureChanged(value.toReal(), zone);
    else if (property == QLatin1String("fanSpeed"))
        emit fanSpeedChanged(value.toInt(), zone);
    else if (property == QLatin1String("fanDirection"))
        emit fanDirectionChanged(validatedEnum<Common::HVACFanDirection>(value, 5), zone);
    else if (property == QLatin1String("recirculationMode"))
        emit recirculationModeChanged(validatedEnum<Common::RecirculationMode>(value, 2), zone);
    else if (property == QLatin1String("isAirConditioningActive"))
        emit isAirConditioningActiveChanged(value.toBool(), zone);
    else if (property == QLatin1String("ambientAirTemperature"))
        emit ambientAirTemperatureChanged(value.toReal(), zone);
}

// ===========================================================================
// CabinInfotainmentKuksaBackend
// ===========================================================================

CabinInfotainmentKuksaBackend::CabinInfotainmentKuksaBackend(KuksaClient *client, QObject *parent)
    : CabinInfotainmentBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Cabin.CabinInfotainment"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

CabinInfotainmentKuksaBackend::~CabinInfotainmentKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Cabin.CabinInfotainment"));
}

void CabinInfotainmentKuksaBackend::initialize()
{
    emit initializationDone();
}

void CabinInfotainmentKuksaBackend::setCurrentSource(Common::MediaSource currentSource)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinInfotainment");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("currentSource")), static_cast<int>(currentSource));
}

void CabinInfotainmentKuksaBackend::setVolume(int volume)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinInfotainment");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("volume")), volume);
}

void CabinInfotainmentKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("currentSource"))
        emit currentSourceChanged(validatedEnum<Common::MediaSource>(value, 5));
    else if (property == QLatin1String("volume"))
        emit volumeChanged(value.toInt());
    else if (property == QLatin1String("currentTrack"))
        emit currentTrackChanged(value.toString());
    else if (property == QLatin1String("currentArtist"))
        emit currentArtistChanged(value.toString());
    else if (property == QLatin1String("currentAlbum"))
        emit currentAlbumChanged(value.toString());
    else if (property == QLatin1String("elapsedTime"))
        emit elapsedTimeChanged(value.toInt());
    else if (property == QLatin1String("selectedLanguage"))
        emit selectedLanguageChanged(value.toString());
    else if (property == QLatin1String("dayNightMode"))
        emit dayNightModeChanged(validatedEnum<Common::DayNightMode>(value, 2));
}

// ===========================================================================
// CabinDoorKuksaBackend
// ===========================================================================

CabinDoorKuksaBackend::CabinDoorKuksaBackend(KuksaClient *client, QObject *parent)
    : CabinDoorBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Cabin.CabinDoor"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

CabinDoorKuksaBackend::~CabinDoorKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Cabin.CabinDoor"));
}

void CabinDoorKuksaBackend::initialize()
{
    emit initializationDone();
}

QStringList CabinDoorKuksaBackend::availableZones() const
{
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinDoor");
    return VssPathMapping::zones(iid);
}

void CabinDoorKuksaBackend::setIsLocked(bool isLocked, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinDoor");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isLocked"), zone), isLocked);
}

void CabinDoorKuksaBackend::setIsChildLockActive(bool isChildLockActive, const QString &zone)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinDoor");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isChildLockActive"), zone), isChildLockActive);
}

void CabinDoorKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    if (property == QLatin1String("isOpen"))
        emit isOpenChanged(value.toBool(), zone);
    else if (property == QLatin1String("isLocked"))
        emit isLockedChanged(value.toBool(), zone);
    else if (property == QLatin1String("windowPosition"))
        emit windowPositionChanged(value.toInt(), zone);
    else if (property == QLatin1String("isChildLockActive"))
        emit isChildLockActiveChanged(value.toBool(), zone);
    else if (property == QLatin1String("doorState"))
        emit doorStateChanged(validatedEnum<Common::DoorState>(value, 2), zone);
}

// ===========================================================================
// CabinLightsKuksaBackend
// ===========================================================================

CabinLightsKuksaBackend::CabinLightsKuksaBackend(KuksaClient *client, QObject *parent)
    : CabinLightsBackendInterface(parent)
    , m_client(client)
{
    if (m_client) {
        m_client->registerBackend(QStringLiteral("COVESA.VSS.Cabin.CabinLights"),
            [this](const QString &property, const QString &zone, const QVariant &value) {
                onVssValue(property, zone, value);
            });
    }
}

CabinLightsKuksaBackend::~CabinLightsKuksaBackend()
{
    if (m_client)
        m_client->unregisterBackend(QStringLiteral("COVESA.VSS.Cabin.CabinLights"));
}

void CabinLightsKuksaBackend::initialize()
{
    emit initializationDone();
}

void CabinLightsKuksaBackend::setAmbientLightBrightness(int ambientLightBrightness)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinLights");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("ambientLightBrightness")), ambientLightBrightness);
}

void CabinLightsKuksaBackend::setAmbientLightRed(int ambientLightRed)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinLights");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("ambientLightRed")), ambientLightRed);
}

void CabinLightsKuksaBackend::setAmbientLightGreen(int ambientLightGreen)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinLights");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("ambientLightGreen")), ambientLightGreen);
}

void CabinLightsKuksaBackend::setAmbientLightBlue(int ambientLightBlue)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinLights");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("ambientLightBlue")), ambientLightBlue);
}

void CabinLightsKuksaBackend::setIsDomeLightOn(bool isDomeLightOn)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinLights");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isDomeLightOn")), isDomeLightOn);
}

void CabinLightsKuksaBackend::setIsSpotlightOn(bool isSpotlightOn)
{
    if (!m_client) return;
    static const QString iid = QStringLiteral("COVESA.VSS.Cabin.CabinLights");
    m_client->actuate(VssPathMapping::vssPath(iid, QStringLiteral("isSpotlightOn")), isSpotlightOn);
}

void CabinLightsKuksaBackend::onVssValue(const QString &property, const QString &zone, const QVariant &value)
{
    Q_UNUSED(zone)
    if (property == QLatin1String("ambientLightBrightness"))
        emit ambientLightBrightnessChanged(value.toInt());
    else if (property == QLatin1String("ambientLightRed"))
        emit ambientLightRedChanged(value.toInt());
    else if (property == QLatin1String("ambientLightGreen"))
        emit ambientLightGreenChanged(value.toInt());
    else if (property == QLatin1String("ambientLightBlue"))
        emit ambientLightBlueChanged(value.toInt());
    else if (property == QLatin1String("isDomeLightOn"))
        emit isDomeLightOnChanged(value.toBool());
    else if (property == QLatin1String("lightIntensity"))
        emit lightIntensityChanged(value.toInt());
    else if (property == QLatin1String("isSpotlightOn"))
        emit isSpotlightOnChanged(value.toBool());
}
