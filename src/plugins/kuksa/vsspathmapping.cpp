#include "vsspathmapping.h"

static QString makeKey3(const QString &a, const QString &b, const QString &c)
{
    return a + QLatin1Char('\0') + b + QLatin1Char('\0') + c;
}

QString VssPathMapping::makeKey(const QString &iid, const QString &property, const QString &zone)
{
    return makeKey3(iid, property, zone);
}

void VssPathMapping::Tables::reg(const QString &iid, const QString &prop, const QString &vss)
{
    const QString key = makeKey3(iid, prop, QString());
    forward.insert(key, vss);
    reverse[vss].append({iid, prop, QString()});
    if (!iidPaths[iid].contains(vss))
        iidPaths[iid].append(vss);
}

void VssPathMapping::Tables::regZoned(const QString &iid, const QString &prop,
                                      const QString &vssTemplate, const QStringList &zones)
{
    for (const QString &zone : zones) {
        const QString instance = zoneInstance.value(
            iid + QLatin1Char('\0') + zone, zone);
        QString vss = vssTemplate;
        const int wildcardCount = vss.count(QLatin1Char('*'));
        if (wildcardCount > 1) {
            // Multi-wildcard template (e.g. Axle.*.Wheel.*): split the
            // instance at '.' and replace each wildcard with one segment.
            const QStringList segments = instance.split(QLatin1Char('.'));
            for (const QString &seg : segments) {
                const int idx = vss.indexOf(QLatin1Char('*'));
                if (idx < 0)
                    break;
                vss.replace(idx, 1, seg);
            }
        } else {
            vss.replace(QLatin1String("*"), instance);
        }
        const QString key = makeKey3(iid, prop, zone);
        forward.insert(key, vss);
        reverse[vss].append({iid, prop, zone});
        if (!iidPaths[iid].contains(vss))
            iidPaths[iid].append(vss);
    }
}

void VssPathMapping::Tables::regZones(const QString &iid, const QStringList &zones,
                                      const QHash<QString, QString> &instanceMap)
{
    iidZones.insert(iid, zones);
    for (auto it = instanceMap.cbegin(); it != instanceMap.cend(); ++it)
        zoneInstance.insert(iid + QLatin1Char('\0') + it.key(), it.value());
}

VssPathMapping::Tables::Tables()
{
    // ── Vehicle module ──────────────────────────────────────────────

    const QString viId = QStringLiteral("COVESA.VSS.Vehicle.VehicleIdentification");
    reg(viId, QStringLiteral("vin"), QStringLiteral("Vehicle.VehicleIdentification.VIN"));
    reg(viId, QStringLiteral("brand"), QStringLiteral("Vehicle.VehicleIdentification.Brand"));
    reg(viId, QStringLiteral("vehicleModel"), QStringLiteral("Vehicle.VehicleIdentification.Model"));
    reg(viId, QStringLiteral("year"), QStringLiteral("Vehicle.VehicleIdentification.Year"));
    reg(viId, QStringLiteral("bodyType"), QStringLiteral("Vehicle.VehicleIdentification.BodyType"));
    reg(viId, QStringLiteral("dateVehicleFirstRegistered"), QStringLiteral("Vehicle.VehicleIdentification.DateVehicleFirstRegistered"));
    reg(viId, QStringLiteral("meetsEmissionStandard"), QStringLiteral("Vehicle.VehicleIdentification.MeetsEmissionStandard"));
    reg(viId, QStringLiteral("vehicleSpecialUsage"), QStringLiteral("Vehicle.VehicleIdentification.VehicleSpecialUsage"));
    reg(viId, QStringLiteral("vehicleInteriorColor"), QStringLiteral("Vehicle.VehicleIdentification.VehicleInteriorColor"));
    reg(viId, QStringLiteral("vehicleInteriorType"), QStringLiteral("Vehicle.VehicleIdentification.VehicleInteriorType"));
    reg(viId, QStringLiteral("knownVehicleDamages"), QStringLiteral("Vehicle.VehicleIdentification.KnownVehicleDamages"));

    const QString vdId = QStringLiteral("COVESA.VSS.Vehicle.VehicleDynamics");
    reg(vdId, QStringLiteral("speed"), QStringLiteral("Vehicle.Speed"));
    reg(vdId, QStringLiteral("traveledDistance"), QStringLiteral("Vehicle.TraveledDistance"));
    reg(vdId, QStringLiteral("traveledDistanceSinceStart"), QStringLiteral("Vehicle.TraveledDistanceSinceStart"));
    reg(vdId, QStringLiteral("tripMeterReading"), QStringLiteral("Vehicle.TripMeterReading"));
    reg(vdId, QStringLiteral("isMoving"), QStringLiteral("Vehicle.IsMoving"));
    reg(vdId, QStringLiteral("averageSpeed"), QStringLiteral("Vehicle.AverageSpeed"));
    reg(vdId, QStringLiteral("currentOverallWeight"), QStringLiteral("Vehicle.CurrentOverallWeight"));

    const QString clId = QStringLiteral("COVESA.VSS.Vehicle.CurrentLocation");
    reg(clId, QStringLiteral("latitude"), QStringLiteral("Vehicle.CurrentLocation.Latitude"));
    reg(clId, QStringLiteral("longitude"), QStringLiteral("Vehicle.CurrentLocation.Longitude"));
    reg(clId, QStringLiteral("altitude"), QStringLiteral("Vehicle.CurrentLocation.Altitude"));
    reg(clId, QStringLiteral("heading"), QStringLiteral("Vehicle.CurrentLocation.Heading"));
    reg(clId, QStringLiteral("horizontalAccuracy"), QStringLiteral("Vehicle.CurrentLocation.HorizontalAccuracy"));
    reg(clId, QStringLiteral("verticalAccuracy"), QStringLiteral("Vehicle.CurrentLocation.VerticalAccuracy"));
    reg(clId, QStringLiteral("gnssFixType"), QStringLiteral("Vehicle.CurrentLocation.GNSSReceiver.FixType"));
    reg(clId, QStringLiteral("timestamp"), QStringLiteral("Vehicle.CurrentLocation.Timestamp"));

    const QString lvbId = QStringLiteral("COVESA.VSS.Vehicle.LowVoltageBattery");
    reg(lvbId, QStringLiteral("currentVoltage"), QStringLiteral("Vehicle.LowVoltageBattery.CurrentVoltage"));
    reg(lvbId, QStringLiteral("currentCurrent"), QStringLiteral("Vehicle.LowVoltageBattery.CurrentCurrent"));
    reg(lvbId, QStringLiteral("nominalVoltage"), QStringLiteral("Vehicle.LowVoltageBattery.NominalVoltage"));
    reg(lvbId, QStringLiteral("nominalCapacity"), QStringLiteral("Vehicle.LowVoltageBattery.NominalCapacity"));

    // ── Powertrain module ───────────────────────────────────────────

    const QString psId = QStringLiteral("COVESA.VSS.Powertrain.PowertrainStatus");
    reg(psId, QStringLiteral("type"), QStringLiteral("Vehicle.Powertrain.Type"));
    reg(psId, QStringLiteral("accumulatedBrakingEnergy"), QStringLiteral("Vehicle.Powertrain.AccumulatedBrakingEnergy"));
    reg(psId, QStringLiteral("range"), QStringLiteral("Vehicle.Powertrain.Range"));

    const QString ceId = QStringLiteral("COVESA.VSS.Powertrain.CombustionEngine");
    reg(ceId, QStringLiteral("engineState"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.EngineState"));
    reg(ceId, QStringLiteral("speed"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.Speed"));
    reg(ceId, QStringLiteral("engineHours"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.EngineHours"));
    reg(ceId, QStringLiteral("engineCoolantTemperature"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.ECT"));
    reg(ceId, QStringLiteral("engineOilTemperature"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.EOT"));
    reg(ceId, QStringLiteral("engineOilPressure"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.OilPressure"));
    reg(ceId, QStringLiteral("engineOilLevel"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.OilLevel"));
    reg(ceId, QStringLiteral("power"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.Power"));
    reg(ceId, QStringLiteral("torque"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.Torque"));
    reg(ceId, QStringLiteral("displacement"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.Displacement"));
    reg(ceId, QStringLiteral("configuration"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.Configuration"));
    reg(ceId, QStringLiteral("numberOfCylinders"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.NumberOfCylinders"));
    reg(ceId, QStringLiteral("maf"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.MAF"));
    reg(ceId, QStringLiteral("manifoldAbsolutePressure"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.MAP"));
    reg(ceId, QStringLiteral("tps"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.TPS"));
    reg(ceId, QStringLiteral("isRunning"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.IsRunning"));
    reg(ceId, QStringLiteral("idleHours"), QStringLiteral("Vehicle.Powertrain.CombustionEngine.IdleHours"));

    const QString trId = QStringLiteral("COVESA.VSS.Powertrain.Transmission");
    reg(trId, QStringLiteral("type"), QStringLiteral("Vehicle.Powertrain.Transmission.Type"));
    reg(trId, QStringLiteral("gearCount"), QStringLiteral("Vehicle.Powertrain.Transmission.GearCount"));
    reg(trId, QStringLiteral("driveType"), QStringLiteral("Vehicle.Powertrain.Transmission.DriveType"));
    reg(trId, QStringLiteral("currentGear"), QStringLiteral("Vehicle.Powertrain.Transmission.CurrentGear"));
    reg(trId, QStringLiteral("selectedGear"), QStringLiteral("Vehicle.Powertrain.Transmission.SelectedGear"));
    reg(trId, QStringLiteral("isParkLockEngaged"), QStringLiteral("Vehicle.Powertrain.Transmission.IsParkLockEngaged"));
    reg(trId, QStringLiteral("performanceMode"), QStringLiteral("Vehicle.Powertrain.Transmission.PerformanceMode"));
    reg(trId, QStringLiteral("temperature"), QStringLiteral("Vehicle.Powertrain.Transmission.Temperature"));
    reg(trId, QStringLiteral("clutchWear"), QStringLiteral("Vehicle.Powertrain.Transmission.ClutchWear"));
    reg(trId, QStringLiteral("travelledDistance"), QStringLiteral("Vehicle.Powertrain.Transmission.TravelledDistance"));

    // ElectricMotor - zoned (Motor1, Motor2)
    const QString emId = QStringLiteral("COVESA.VSS.Powertrain.ElectricMotor");
    const QStringList emZones = {QStringLiteral("Motor1"), QStringLiteral("Motor2")};
    regZones(emId, emZones, {
        {QStringLiteral("Motor1"), QStringLiteral("Motor1")},
        {QStringLiteral("Motor2"), QStringLiteral("Motor2")}
    });
    regZoned(emId, QStringLiteral("maxPower"), QStringLiteral("Vehicle.Powertrain.ElectricMotor.*.MaxPower"), emZones);
    regZoned(emId, QStringLiteral("maxTorque"), QStringLiteral("Vehicle.Powertrain.ElectricMotor.*.MaxTorque"), emZones);
    regZoned(emId, QStringLiteral("maxRegenPower"), QStringLiteral("Vehicle.Powertrain.ElectricMotor.*.MaxRegenPower"), emZones);
    regZoned(emId, QStringLiteral("maxRegenTorque"), QStringLiteral("Vehicle.Powertrain.ElectricMotor.*.MaxRegenTorque"), emZones);
    regZoned(emId, QStringLiteral("speed"), QStringLiteral("Vehicle.Powertrain.ElectricMotor.*.Speed"), emZones);
    regZoned(emId, QStringLiteral("temperature"), QStringLiteral("Vehicle.Powertrain.ElectricMotor.*.Temperature"), emZones);
    regZoned(emId, QStringLiteral("coolantTemperature"), QStringLiteral("Vehicle.Powertrain.ElectricMotor.*.CoolantTemperature"), emZones);
    regZoned(emId, QStringLiteral("power"), QStringLiteral("Vehicle.Powertrain.ElectricMotor.*.Power"), emZones);
    regZoned(emId, QStringLiteral("torque"), QStringLiteral("Vehicle.Powertrain.ElectricMotor.*.Torque"), emZones);
    regZoned(emId, QStringLiteral("operatingMode"), QStringLiteral("Vehicle.Powertrain.ElectricMotor.*.OperatingMode"), emZones);

    const QString tbId = QStringLiteral("COVESA.VSS.Powertrain.TractionBattery");
    reg(tbId, QStringLiteral("id"), QStringLiteral("Vehicle.Powertrain.TractionBattery.Id"));
    reg(tbId, QStringLiteral("stateOfCharge"), QStringLiteral("Vehicle.Powertrain.TractionBattery.StateOfCharge.Current"));
    reg(tbId, QStringLiteral("stateOfHealth"), QStringLiteral("Vehicle.Powertrain.TractionBattery.StateOfHealth"));
    reg(tbId, QStringLiteral("currentVoltage"), QStringLiteral("Vehicle.Powertrain.TractionBattery.CurrentVoltage"));
    reg(tbId, QStringLiteral("currentCurrent"), QStringLiteral("Vehicle.Powertrain.TractionBattery.CurrentCurrent"));
    reg(tbId, QStringLiteral("currentPower"), QStringLiteral("Vehicle.Powertrain.TractionBattery.CurrentPower"));
    reg(tbId, QStringLiteral("temperature"), QStringLiteral("Vehicle.Powertrain.TractionBattery.Temperature"));
    reg(tbId, QStringLiteral("nominalVoltage"), QStringLiteral("Vehicle.Powertrain.TractionBattery.NominalVoltage"));
    reg(tbId, QStringLiteral("maxVoltage"), QStringLiteral("Vehicle.Powertrain.TractionBattery.MaxVoltage"));
    reg(tbId, QStringLiteral("grossCapacity"), QStringLiteral("Vehicle.Powertrain.TractionBattery.GrossCapacity"));
    reg(tbId, QStringLiteral("netCapacity"), QStringLiteral("Vehicle.Powertrain.TractionBattery.NetCapacity"));
    reg(tbId, QStringLiteral("accumulatedChargedEnergy"), QStringLiteral("Vehicle.Powertrain.TractionBattery.AccumulatedChargedEnergy"));
    reg(tbId, QStringLiteral("accumulatedConsumedEnergy"), QStringLiteral("Vehicle.Powertrain.TractionBattery.AccumulatedConsumedEnergy"));
    reg(tbId, QStringLiteral("chargingStatus"), QStringLiteral("Vehicle.Powertrain.TractionBattery.Charging.Status"));
    reg(tbId, QStringLiteral("chargePlugType"), QStringLiteral("Vehicle.Powertrain.TractionBattery.Charging.ChargePlugType"));
    reg(tbId, QStringLiteral("isCharging"), QStringLiteral("Vehicle.Powertrain.TractionBattery.Charging.IsCharging"));
    reg(tbId, QStringLiteral("isChargingCableConnected"), QStringLiteral("Vehicle.Powertrain.TractionBattery.Charging.IsChargingCableConnected"));
    reg(tbId, QStringLiteral("chargingRate"), QStringLiteral("Vehicle.Powertrain.TractionBattery.Charging.ChargeRate"));
    reg(tbId, QStringLiteral("chargeLimit"), QStringLiteral("Vehicle.Powertrain.TractionBattery.Charging.ChargeLimit"));
    reg(tbId, QStringLiteral("timeToComplete"), QStringLiteral("Vehicle.Powertrain.TractionBattery.Charging.TimeToComplete"));

    const QString fsId = QStringLiteral("COVESA.VSS.Powertrain.FuelSystem");
    reg(fsId, QStringLiteral("supportedFuelType"), QStringLiteral("Vehicle.Powertrain.FuelSystem.SupportedFuelTypes"));
    reg(fsId, QStringLiteral("fuelLevel"), QStringLiteral("Vehicle.Powertrain.FuelSystem.Level"));
    reg(fsId, QStringLiteral("range"), QStringLiteral("Vehicle.Powertrain.FuelSystem.Range"));
    reg(fsId, QStringLiteral("instantConsumption"), QStringLiteral("Vehicle.Powertrain.FuelSystem.InstantConsumption"));
    reg(fsId, QStringLiteral("averageConsumption"), QStringLiteral("Vehicle.Powertrain.FuelSystem.AverageConsumption"));
    reg(fsId, QStringLiteral("consumptionSinceStart"), QStringLiteral("Vehicle.Powertrain.FuelSystem.ConsumptionSinceStart"));
    reg(fsId, QStringLiteral("tankCapacity"), QStringLiteral("Vehicle.Powertrain.FuelSystem.TankCapacity"));
    reg(fsId, QStringLiteral("isEngineStopStartEnabled"), QStringLiteral("Vehicle.Powertrain.FuelSystem.IsEngineStopStartEnabled"));
    reg(fsId, QStringLiteral("isFuelLevelLow"), QStringLiteral("Vehicle.Powertrain.FuelSystem.IsFuelLevelLow"));

    // ── Body module ─────────────────────────────────────────────────

    const QString bcId = QStringLiteral("COVESA.VSS.Body.BodyControl");
    reg(bcId, QStringLiteral("rearMainSpoilerPosition"), QStringLiteral("Vehicle.Body.RearMainSpoilerPosition"));
    reg(bcId, QStringLiteral("turnSignalState"), QStringLiteral("Vehicle.Body.Lights.DirectionIndicator"));
    reg(bcId, QStringLiteral("isHoodOpen"), QStringLiteral("Vehicle.Body.Hood.IsOpen"));
    reg(bcId, QStringLiteral("isTrunkOpen"), QStringLiteral("Vehicle.Body.Trunk.IsOpen"));
    reg(bcId, QStringLiteral("isTrunkLocked"), QStringLiteral("Vehicle.Body.Trunk.IsLocked"));
    reg(bcId, QStringLiteral("rainIntensity"), QStringLiteral("Vehicle.Body.Raindetection.Intensity"));

    const QString blId = QStringLiteral("COVESA.VSS.Body.BodyLights");
    reg(blId, QStringLiteral("lightSwitch"), QStringLiteral("Vehicle.Body.Lights.LightSwitch"));
    reg(blId, QStringLiteral("isHighBeamOn"), QStringLiteral("Vehicle.Body.Lights.Beam.High.IsOn"));
    reg(blId, QStringLiteral("isLowBeamOn"), QStringLiteral("Vehicle.Body.Lights.Beam.Low.IsOn"));
    reg(blId, QStringLiteral("isFogFrontOn"), QStringLiteral("Vehicle.Body.Lights.Fog.Front.IsOn"));
    reg(blId, QStringLiteral("isFogRearOn"), QStringLiteral("Vehicle.Body.Lights.Fog.Rear.IsOn"));
    reg(blId, QStringLiteral("isRunningOn"), QStringLiteral("Vehicle.Body.Lights.Running.IsOn"));
    reg(blId, QStringLiteral("isParkingOn"), QStringLiteral("Vehicle.Body.Lights.Parking.IsOn"));
    reg(blId, QStringLiteral("isBrakeOn"), QStringLiteral("Vehicle.Body.Lights.Brake.IsOn"));
    reg(blId, QStringLiteral("isHazardOn"), QStringLiteral("Vehicle.Body.Lights.Hazard.IsOn"));
    reg(blId, QStringLiteral("isBackupOn"), QStringLiteral("Vehicle.Body.Lights.Backup.IsOn"));
    reg(blId, QStringLiteral("interiorBrightness"), QStringLiteral("Vehicle.Body.Lights.Interior.Brightness"));

    // Windshield - zoned (Front, Rear)
    const QString wsId = QStringLiteral("COVESA.VSS.Body.Windshield");
    const QStringList wsZones = {QStringLiteral("Front"), QStringLiteral("Rear")};
    regZones(wsId, wsZones, {
        {QStringLiteral("Front"), QStringLiteral("Front")},
        {QStringLiteral("Rear"), QStringLiteral("Rear")}
    });
    regZoned(wsId, QStringLiteral("wiperMode"), QStringLiteral("Vehicle.Body.Windshield.*.Wiping.Mode"), wsZones);
    regZoned(wsId, QStringLiteral("wiperWearLevel"), QStringLiteral("Vehicle.Body.Windshield.*.Wiping.WiperWear"), wsZones);
    regZoned(wsId, QStringLiteral("isWiperFluidLevelLow"), QStringLiteral("Vehicle.Body.Windshield.*.WasherFluid.IsLevelLow"), wsZones);
    regZoned(wsId, QStringLiteral("isHeatingOn"), QStringLiteral("Vehicle.Body.Windshield.*.Heating.Status"), wsZones);
    regZoned(wsId, QStringLiteral("isWindshieldHeatingOn"), QStringLiteral("Vehicle.Body.Windshield.*.Heating.IsOn"), wsZones);

    // BodyMirrors - zoned (Left, Right)
    const QString bmId = QStringLiteral("COVESA.VSS.Body.BodyMirrors");
    const QStringList bmZones = {QStringLiteral("Left"), QStringLiteral("Right")};
    regZones(bmId, bmZones, {
        {QStringLiteral("Left"), QStringLiteral("Left")},
        {QStringLiteral("Right"), QStringLiteral("Right")}
    });
    regZoned(bmId, QStringLiteral("tilt"), QStringLiteral("Vehicle.Body.Mirrors.*.Tilt"), bmZones);
    regZoned(bmId, QStringLiteral("pan"), QStringLiteral("Vehicle.Body.Mirrors.*.Pan"), bmZones);
    regZoned(bmId, QStringLiteral("isHeatingOn"), QStringLiteral("Vehicle.Body.Mirrors.*.IsHeatingOn"), bmZones);
    regZoned(bmId, QStringLiteral("isFolded"), QStringLiteral("Vehicle.Body.Mirrors.*.IsFolded"), bmZones);

    // ── Cabin module ────────────────────────────────────────────────

    // CabinSeat - zoned (Row1Left, Row1Right, Row2Left, Row2Right, Row2Center)
    const QString csId = QStringLiteral("COVESA.VSS.Cabin.CabinSeat");
    const QStringList csZones = {
        QStringLiteral("Row1Left"), QStringLiteral("Row1Right"),
        QStringLiteral("Row2Left"), QStringLiteral("Row2Right"),
        QStringLiteral("Row2Center")
    };
    regZones(csId, csZones, {
        {QStringLiteral("Row1Left"), QStringLiteral("Row1.DriverSide")},
        {QStringLiteral("Row1Right"), QStringLiteral("Row1.PassengerSide")},
        {QStringLiteral("Row2Left"), QStringLiteral("Row2.DriverSide")},
        {QStringLiteral("Row2Right"), QStringLiteral("Row2.PassengerSide")},
        {QStringLiteral("Row2Center"), QStringLiteral("Row2.Middle")}
    });
    regZoned(csId, QStringLiteral("position"), QStringLiteral("Vehicle.Cabin.Seat.*.Position"), csZones);
    regZoned(csId, QStringLiteral("height"), QStringLiteral("Vehicle.Cabin.Seat.*.Height"), csZones);
    regZoned(csId, QStringLiteral("tilt"), QStringLiteral("Vehicle.Cabin.Seat.*.Tilt"), csZones);
    regZoned(csId, QStringLiteral("backrestRecline"), QStringLiteral("Vehicle.Cabin.Seat.*.Backrest.Recline"), csZones);
    regZoned(csId, QStringLiteral("backrestLumbarSupport"), QStringLiteral("Vehicle.Cabin.Seat.*.Backrest.Lumbar.Support"), csZones);
    regZoned(csId, QStringLiteral("backrestLumbarHeight"), QStringLiteral("Vehicle.Cabin.Seat.*.Backrest.Lumbar.Height"), csZones);
    regZoned(csId, QStringLiteral("isHeatingOn"), QStringLiteral("Vehicle.Cabin.Seat.*.Heating"), csZones);
    regZoned(csId, QStringLiteral("heatingLevel"), QStringLiteral("Vehicle.Cabin.Seat.*.HeatingLevel"), csZones);
    regZoned(csId, QStringLiteral("isCoolingOn"), QStringLiteral("Vehicle.Cabin.Seat.*.Cooling"), csZones);
    regZoned(csId, QStringLiteral("coolingLevel"), QStringLiteral("Vehicle.Cabin.Seat.*.CoolingLevel"), csZones);
    regZoned(csId, QStringLiteral("isMassageOn"), QStringLiteral("Vehicle.Cabin.Seat.*.Massage"), csZones);
    regZoned(csId, QStringLiteral("isOccupied"), QStringLiteral("Vehicle.Cabin.Seat.*.IsOccupied"), csZones);
    regZoned(csId, QStringLiteral("occupantStatus"), QStringLiteral("Vehicle.Cabin.Seat.*.Occupant.Status"), csZones);
    regZoned(csId, QStringLiteral("isSeatbeltFastened"), QStringLiteral("Vehicle.Cabin.Seat.*.IsBelted"), csZones);

    // CabinHVAC - zoned (FrontLeft, FrontRight, RearLeft, RearRight)
    // Some properties are non-zoned but interface is zoned
    const QString hvacId = QStringLiteral("COVESA.VSS.Cabin.CabinHVAC");
    const QStringList hvacZones = {
        QStringLiteral("FrontLeft"), QStringLiteral("FrontRight"),
        QStringLiteral("RearLeft"), QStringLiteral("RearRight")
    };
    regZones(hvacId, hvacZones, {
        {QStringLiteral("FrontLeft"), QStringLiteral("Row1.DriverSide")},
        {QStringLiteral("FrontRight"), QStringLiteral("Row1.PassengerSide")},
        {QStringLiteral("RearLeft"), QStringLiteral("Row2.DriverSide")},
        {QStringLiteral("RearRight"), QStringLiteral("Row2.PassengerSide")}
    });
    regZoned(hvacId, QStringLiteral("targetTemperature"), QStringLiteral("Vehicle.Cabin.HVAC.Station.*.Temperature"), hvacZones);
    regZoned(hvacId, QStringLiteral("fanSpeed"), QStringLiteral("Vehicle.Cabin.HVAC.Station.*.FanSpeed"), hvacZones);
    regZoned(hvacId, QStringLiteral("fanDirection"), QStringLiteral("Vehicle.Cabin.HVAC.Station.*.FanDirection"), hvacZones);
    // Non-zoned HVAC properties - register with empty zone
    reg(hvacId, QStringLiteral("recirculationMode"), QStringLiteral("Vehicle.Cabin.HVAC.RecirculationMode"));
    reg(hvacId, QStringLiteral("isAirConditioningActive"), QStringLiteral("Vehicle.Cabin.HVAC.IsAirConditioningActive"));
    reg(hvacId, QStringLiteral("ambientAirTemperature"), QStringLiteral("Vehicle.Cabin.HVAC.AmbientAirTemperature"));

    const QString ciId = QStringLiteral("COVESA.VSS.Cabin.CabinInfotainment");
    reg(ciId, QStringLiteral("currentSource"), QStringLiteral("Vehicle.Cabin.Infotainment.Media.Source"));
    reg(ciId, QStringLiteral("volume"), QStringLiteral("Vehicle.Cabin.Infotainment.Media.Volume"));
    reg(ciId, QStringLiteral("currentTrack"), QStringLiteral("Vehicle.Cabin.Infotainment.Media.Track"));
    reg(ciId, QStringLiteral("currentArtist"), QStringLiteral("Vehicle.Cabin.Infotainment.Media.Artist"));
    reg(ciId, QStringLiteral("currentAlbum"), QStringLiteral("Vehicle.Cabin.Infotainment.Media.Album"));
    reg(ciId, QStringLiteral("elapsedTime"), QStringLiteral("Vehicle.Cabin.Infotainment.Media.ElapsedTime"));
    reg(ciId, QStringLiteral("selectedLanguage"), QStringLiteral("Vehicle.Cabin.Infotainment.HMI.CurrentLanguage"));
    reg(ciId, QStringLiteral("dayNightMode"), QStringLiteral("Vehicle.Cabin.Infotainment.HMI.DayNightMode"));

    // CabinDoor - zoned (FrontLeft, FrontRight, RearLeft, RearRight)
    const QString cdId = QStringLiteral("COVESA.VSS.Cabin.CabinDoor");
    const QStringList cdZones = {
        QStringLiteral("FrontLeft"), QStringLiteral("FrontRight"),
        QStringLiteral("RearLeft"), QStringLiteral("RearRight")
    };
    regZones(cdId, cdZones, {
        {QStringLiteral("FrontLeft"), QStringLiteral("Row1.DriverSide")},
        {QStringLiteral("FrontRight"), QStringLiteral("Row1.PassengerSide")},
        {QStringLiteral("RearLeft"), QStringLiteral("Row2.DriverSide")},
        {QStringLiteral("RearRight"), QStringLiteral("Row2.PassengerSide")}
    });
    regZoned(cdId, QStringLiteral("isOpen"), QStringLiteral("Vehicle.Cabin.Door.*.IsOpen"), cdZones);
    regZoned(cdId, QStringLiteral("isLocked"), QStringLiteral("Vehicle.Cabin.Door.*.IsLocked"), cdZones);
    regZoned(cdId, QStringLiteral("windowPosition"), QStringLiteral("Vehicle.Cabin.Door.*.Window.Position"), cdZones);
    regZoned(cdId, QStringLiteral("isChildLockActive"), QStringLiteral("Vehicle.Cabin.Door.*.IsChildLockActive"), cdZones);
    regZoned(cdId, QStringLiteral("doorState"), QStringLiteral("Vehicle.Cabin.Door.*.State"), cdZones);

    const QString clgId = QStringLiteral("COVESA.VSS.Cabin.CabinLights");
    reg(clgId, QStringLiteral("ambientLightBrightness"), QStringLiteral("Vehicle.Cabin.Lights.AmbientLight.Brightness"));
    reg(clgId, QStringLiteral("ambientLightRed"), QStringLiteral("Vehicle.Cabin.Lights.AmbientLight.Red"));
    reg(clgId, QStringLiteral("ambientLightGreen"), QStringLiteral("Vehicle.Cabin.Lights.AmbientLight.Green"));
    reg(clgId, QStringLiteral("ambientLightBlue"), QStringLiteral("Vehicle.Cabin.Lights.AmbientLight.Blue"));
    reg(clgId, QStringLiteral("isDomeLightOn"), QStringLiteral("Vehicle.Cabin.Lights.IsDomeOn"));
    reg(clgId, QStringLiteral("lightIntensity"), QStringLiteral("Vehicle.Cabin.Lights.LightIntensity"));
    reg(clgId, QStringLiteral("isSpotlightOn"), QStringLiteral("Vehicle.Cabin.Lights.Spotlight.IsOn"));

    // ── ADAS module ─────────────────────────────────────────────────

    const QString acId = QStringLiteral("COVESA.VSS.ADAS.ADASControl");
    reg(acId, QStringLiteral("activeAutonomyLevel"), QStringLiteral("Vehicle.ADAS.ActiveAutonomyLevel"));
    reg(acId, QStringLiteral("supportedAutonomyLevel"), QStringLiteral("Vehicle.ADAS.SupportedAutonomyLevel"));
    reg(acId, QStringLiteral("isAEBEnabled"), QStringLiteral("Vehicle.ADAS.AEB.IsEnabled"));
    reg(acId, QStringLiteral("isAEBActive"), QStringLiteral("Vehicle.ADAS.AEB.IsActive"));
    reg(acId, QStringLiteral("isAEBError"), QStringLiteral("Vehicle.ADAS.AEB.IsError"));
    reg(acId, QStringLiteral("isEBDEnabled"), QStringLiteral("Vehicle.ADAS.EBD.IsEnabled"));
    reg(acId, QStringLiteral("isEBDActive"), QStringLiteral("Vehicle.ADAS.EBD.IsActive"));
    reg(acId, QStringLiteral("isEBDError"), QStringLiteral("Vehicle.ADAS.EBD.IsError"));
    reg(acId, QStringLiteral("isESCEnabled"), QStringLiteral("Vehicle.ADAS.ESC.IsEnabled"));
    reg(acId, QStringLiteral("isESCActive"), QStringLiteral("Vehicle.ADAS.ESC.IsActive"));
    reg(acId, QStringLiteral("isESCError"), QStringLiteral("Vehicle.ADAS.ESC.IsError"));
    reg(acId, QStringLiteral("isTCSEnabled"), QStringLiteral("Vehicle.ADAS.TCS.IsEnabled"));
    reg(acId, QStringLiteral("isTCSActive"), QStringLiteral("Vehicle.ADAS.TCS.IsActive"));
    reg(acId, QStringLiteral("isTCSError"), QStringLiteral("Vehicle.ADAS.TCS.IsError"));
    reg(acId, QStringLiteral("laneDepartureState"), QStringLiteral("Vehicle.ADAS.LaneDepartureDetection.State"));
    reg(acId, QStringLiteral("isLaneDepartureEnabled"), QStringLiteral("Vehicle.ADAS.LaneDepartureDetection.IsEnabled"));
    reg(acId, QStringLiteral("isLaneDepartureWarning"), QStringLiteral("Vehicle.ADAS.LaneDepartureDetection.IsWarning"));
    reg(acId, QStringLiteral("blindSpotState"), QStringLiteral("Vehicle.ADAS.BlindSpotDetection.State"));
    reg(acId, QStringLiteral("isBlindSpotEnabled"), QStringLiteral("Vehicle.ADAS.BlindSpotDetection.IsEnabled"));

    const QString ccId = QStringLiteral("COVESA.VSS.ADAS.CruiseControl");
    reg(ccId, QStringLiteral("isEnabled"), QStringLiteral("Vehicle.ADAS.CruiseControl.IsEnabled"));
    reg(ccId, QStringLiteral("isActive"), QStringLiteral("Vehicle.ADAS.CruiseControl.IsActive"));
    reg(ccId, QStringLiteral("isError"), QStringLiteral("Vehicle.ADAS.CruiseControl.IsError"));
    reg(ccId, QStringLiteral("speedSet"), QStringLiteral("Vehicle.ADAS.CruiseControl.SpeedSet"));
    reg(ccId, QStringLiteral("mode"), QStringLiteral("Vehicle.ADAS.CruiseControl.Mode"));
    reg(ccId, QStringLiteral("targetGap"), QStringLiteral("Vehicle.ADAS.CruiseControl.TargetGap"));
    reg(ccId, QStringLiteral("isAdaptive"), QStringLiteral("Vehicle.ADAS.CruiseControl.IsAdaptive"));

    // ObstacleDetection - zoned (Front, Rear, Left, Right)
    const QString odId = QStringLiteral("COVESA.VSS.ADAS.ObstacleDetection");
    const QStringList odZones = {
        QStringLiteral("Front"), QStringLiteral("Rear"),
        QStringLiteral("Left"), QStringLiteral("Right")
    };
    regZones(odId, odZones, {
        {QStringLiteral("Front"), QStringLiteral("Front")},
        {QStringLiteral("Rear"), QStringLiteral("Rear")},
        {QStringLiteral("Left"), QStringLiteral("Left")},
        {QStringLiteral("Right"), QStringLiteral("Right")}
    });
    regZoned(odId, QStringLiteral("distance"), QStringLiteral("Vehicle.ADAS.ObstacleDetection.*.Distance"), odZones);
    regZoned(odId, QStringLiteral("warningLevel"), QStringLiteral("Vehicle.ADAS.ObstacleDetection.*.WarningLevel"), odZones);
    regZoned(odId, QStringLiteral("isEnabled"), QStringLiteral("Vehicle.ADAS.ObstacleDetection.*.IsEnabled"), odZones);
    regZoned(odId, QStringLiteral("isDetected"), QStringLiteral("Vehicle.ADAS.ObstacleDetection.*.IsDetected"), odZones);

    // ── Chassis module ──────────────────────────────────────────────

    // ChassisAxle - zoned (Front, Rear)
    const QString caId = QStringLiteral("COVESA.VSS.Chassis.ChassisAxle");
    const QStringList caZones = {QStringLiteral("Front"), QStringLiteral("Rear")};
    regZones(caId, caZones, {
        {QStringLiteral("Front"), QStringLiteral("Row1")},
        {QStringLiteral("Rear"), QStringLiteral("Row2")}
    });
    regZoned(caId, QStringLiteral("wheelCount"), QStringLiteral("Vehicle.Chassis.Axle.*.WheelCount"), caZones);
    regZoned(caId, QStringLiteral("wheelDiameter"), QStringLiteral("Vehicle.Chassis.Axle.*.WheelDiameter"), caZones);
    regZoned(caId, QStringLiteral("wheelWidth"), QStringLiteral("Vehicle.Chassis.Axle.*.WheelWidth"), caZones);
    regZoned(caId, QStringLiteral("tireDiameter"), QStringLiteral("Vehicle.Chassis.Axle.*.TireDiameter"), caZones);
    regZoned(caId, QStringLiteral("tireWidth"), QStringLiteral("Vehicle.Chassis.Axle.*.TireWidth"), caZones);
    regZoned(caId, QStringLiteral("tireAspectRatio"), QStringLiteral("Vehicle.Chassis.Axle.*.TireAspectRatio"), caZones);

    // ChassisWheel - zoned (FrontLeft, FrontRight, RearLeft, RearRight)
    const QString cwId = QStringLiteral("COVESA.VSS.Chassis.ChassisWheel");
    const QStringList cwZones = {
        QStringLiteral("FrontLeft"), QStringLiteral("FrontRight"),
        QStringLiteral("RearLeft"), QStringLiteral("RearRight")
    };
    regZones(cwId, cwZones, {
        {QStringLiteral("FrontLeft"), QStringLiteral("Row1.Left")},
        {QStringLiteral("FrontRight"), QStringLiteral("Row1.Right")},
        {QStringLiteral("RearLeft"), QStringLiteral("Row2.Left")},
        {QStringLiteral("RearRight"), QStringLiteral("Row2.Right")}
    });
    regZoned(cwId, QStringLiteral("tirePressure"), QStringLiteral("Vehicle.Chassis.Axle.*.Wheel.*.Tire.Pressure"), cwZones);
    regZoned(cwId, QStringLiteral("isPressureLow"), QStringLiteral("Vehicle.Chassis.Axle.*.Wheel.*.Tire.IsPressureLow"), cwZones);
    regZoned(cwId, QStringLiteral("tireTemperature"), QStringLiteral("Vehicle.Chassis.Axle.*.Wheel.*.Tire.Temperature"), cwZones);
    regZoned(cwId, QStringLiteral("brakeWear"), QStringLiteral("Vehicle.Chassis.Axle.*.Wheel.*.Brake.PadWear"), cwZones);
    regZoned(cwId, QStringLiteral("isBrakeFluidLevelLow"), QStringLiteral("Vehicle.Chassis.Axle.*.Wheel.*.Brake.IsFluidLevelLow"), cwZones);
    regZoned(cwId, QStringLiteral("speed"), QStringLiteral("Vehicle.Chassis.Axle.*.Wheel.*.Speed"), cwZones);

    const QString csrId = QStringLiteral("COVESA.VSS.Chassis.ChassisSteering");
    reg(csrId, QStringLiteral("steeringWheelAngle"), QStringLiteral("Vehicle.Chassis.SteeringWheel.Angle"));
    reg(csrId, QStringLiteral("steeringWheelTilt"), QStringLiteral("Vehicle.Chassis.SteeringWheel.Tilt"));
    reg(csrId, QStringLiteral("steeringWheelExtension"), QStringLiteral("Vehicle.Chassis.SteeringWheel.Extension"));
    reg(csrId, QStringLiteral("position"), QStringLiteral("Vehicle.Chassis.SteeringWheel.Position"));

    const QString cbId = QStringLiteral("COVESA.VSS.Chassis.ChassisBrake");
    reg(cbId, QStringLiteral("pedalPosition"), QStringLiteral("Vehicle.Chassis.Brake.PedalPosition"));
    reg(cbId, QStringLiteral("isDriverBraking"), QStringLiteral("Vehicle.Chassis.Brake.IsDriverBraking"));
    reg(cbId, QStringLiteral("absStatus"), QStringLiteral("Vehicle.Chassis.Brake.ABS.Status"));
    reg(cbId, QStringLiteral("fluidLevelWarning"), QStringLiteral("Vehicle.Chassis.Brake.FluidLevel"));
    reg(cbId, QStringLiteral("isParkingBrakeEngaged"), QStringLiteral("Vehicle.Chassis.ParkingBrake.IsEngaged"));
    reg(cbId, QStringLiteral("parkingBrakeType"), QStringLiteral("Vehicle.Chassis.ParkingBrake.Type"));

    // ── Driver module ───────────────────────────────────────────────

    const QString dmId = QStringLiteral("COVESA.VSS.Driver.DriverMonitoring");
    reg(dmId, QStringLiteral("heartRate"), QStringLiteral("Vehicle.Driver.HeartRate"));
    reg(dmId, QStringLiteral("isAttentive"), QStringLiteral("Vehicle.Driver.IsAttentive"));
    reg(dmId, QStringLiteral("attentivenessLevel"), QStringLiteral("Vehicle.Driver.AttentivenessLevel"));
    reg(dmId, QStringLiteral("isFatigued"), QStringLiteral("Vehicle.Driver.Fatigue.IsWarning"));
    reg(dmId, QStringLiteral("fatigueLevel"), QStringLiteral("Vehicle.Driver.Fatigue.Level"));
    reg(dmId, QStringLiteral("isEyesOnRoad"), QStringLiteral("Vehicle.Driver.IsEyesOnRoad"));
    reg(dmId, QStringLiteral("isHandsOnWheel"), QStringLiteral("Vehicle.Driver.IsHandsOnWheel"));
    reg(dmId, QStringLiteral("distractionLevel"), QStringLiteral("Vehicle.Driver.DistractionLevel"));
    reg(dmId, QStringLiteral("identifierSubject"), QStringLiteral("Vehicle.Driver.Identifier.Subject"));
    reg(dmId, QStringLiteral("identifierIssuer"), QStringLiteral("Vehicle.Driver.Identifier.Issuer"));

    // ── Safety module ───────────────────────────────────────────────

    const QString crId = QStringLiteral("COVESA.VSS.Safety.CrashDetection");
    reg(crId, QStringLiteral("crashStatus"), QStringLiteral("Vehicle.Safety.CrashDetection.Status"));
    reg(crId, QStringLiteral("crashSeverity"), QStringLiteral("Vehicle.Safety.CrashDetection.Severity"));
    reg(crId, QStringLiteral("isCrashDetected"), QStringLiteral("Vehicle.Safety.CrashDetection.IsDetected"));
    reg(crId, QStringLiteral("crashTimestamp"), QStringLiteral("Vehicle.Safety.CrashDetection.Timestamp"));
    reg(crId, QStringLiteral("impactForce"), QStringLiteral("Vehicle.Safety.CrashDetection.ImpactForce"));
    reg(crId, QStringLiteral("impactAngle"), QStringLiteral("Vehicle.Safety.CrashDetection.ImpactAngle"));

    // AirbagSystem - zoned (FrontLeft, FrontRight, SideLeft, SideRight, Curtain)
    const QString asId = QStringLiteral("COVESA.VSS.Safety.AirbagSystem");
    const QStringList asZones = {
        QStringLiteral("FrontLeft"), QStringLiteral("FrontRight"),
        QStringLiteral("SideLeft"), QStringLiteral("SideRight"),
        QStringLiteral("Curtain")
    };
    regZones(asId, asZones, {
        {QStringLiteral("FrontLeft"), QStringLiteral("Row1.DriverSide.Front")},
        {QStringLiteral("FrontRight"), QStringLiteral("Row1.PassengerSide.Front")},
        {QStringLiteral("SideLeft"), QStringLiteral("Row1.DriverSide.Side")},
        {QStringLiteral("SideRight"), QStringLiteral("Row1.PassengerSide.Side")},
        {QStringLiteral("Curtain"), QStringLiteral("Row1.Curtain")}
    });
    regZoned(asId, QStringLiteral("status"), QStringLiteral("Vehicle.Safety.Airbag.*.Status"), asZones);
    regZoned(asId, QStringLiteral("isDeployed"), QStringLiteral("Vehicle.Safety.Airbag.*.IsDeployed"), asZones);

    // BeltSystem - zoned (Row1Left, Row1Right, Row2Left, Row2Right, Row2Center)
    const QString bsId = QStringLiteral("COVESA.VSS.Safety.BeltSystem");
    const QStringList bsZones = {
        QStringLiteral("Row1Left"), QStringLiteral("Row1Right"),
        QStringLiteral("Row2Left"), QStringLiteral("Row2Right"),
        QStringLiteral("Row2Center")
    };
    regZones(bsId, bsZones, {
        {QStringLiteral("Row1Left"), QStringLiteral("Row1.DriverSide")},
        {QStringLiteral("Row1Right"), QStringLiteral("Row1.PassengerSide")},
        {QStringLiteral("Row2Left"), QStringLiteral("Row2.DriverSide")},
        {QStringLiteral("Row2Right"), QStringLiteral("Row2.PassengerSide")},
        {QStringLiteral("Row2Center"), QStringLiteral("Row2.Middle")}
    });
    regZoned(bsId, QStringLiteral("isFastened"), QStringLiteral("Vehicle.Safety.Belt.*.IsFastened"), bsZones);
    regZoned(bsId, QStringLiteral("isWarning"), QStringLiteral("Vehicle.Safety.Belt.*.IsWarning"), bsZones);

    // ── Exterior module ─────────────────────────────────────────────

    const QString esId = QStringLiteral("COVESA.VSS.Exterior.EnvironmentSensors");
    reg(esId, QStringLiteral("ambientTemperature"), QStringLiteral("Vehicle.Exterior.AirTemperature"));
    reg(esId, QStringLiteral("humidity"), QStringLiteral("Vehicle.Exterior.Humidity"));
    reg(esId, QStringLiteral("atmosphericPressure"), QStringLiteral("Vehicle.Exterior.AtmosphericPressure"));
    reg(esId, QStringLiteral("visibility"), QStringLiteral("Vehicle.Exterior.Visibility"));
    reg(esId, QStringLiteral("lightIntensity"), QStringLiteral("Vehicle.Exterior.LightIntensity"));
    reg(esId, QStringLiteral("roadCondition"), QStringLiteral("Vehicle.Exterior.RoadCondition"));
    reg(esId, QStringLiteral("windSpeed"), QStringLiteral("Vehicle.Exterior.WindSpeed"));
    reg(esId, QStringLiteral("windDirection"), QStringLiteral("Vehicle.Exterior.WindDirection"));
    reg(esId, QStringLiteral("isRaining"), QStringLiteral("Vehicle.Exterior.IsRaining"));

    // ── Service module ──────────────────────────────────────────────

    const QString ssId = QStringLiteral("COVESA.VSS.Service.ServiceStatus");
    reg(ssId, QStringLiteral("overallStatus"), QStringLiteral("Vehicle.Service.OverallStatus"));
    reg(ssId, QStringLiteral("distanceToService"), QStringLiteral("Vehicle.Service.DistanceToService"));
    reg(ssId, QStringLiteral("timeToService"), QStringLiteral("Vehicle.Service.TimeToService"));
    reg(ssId, QStringLiteral("isServiceDue"), QStringLiteral("Vehicle.Service.IsServiceDue"));
    reg(ssId, QStringLiteral("lastServiceDate"), QStringLiteral("Vehicle.Service.LastServiceDate"));
    reg(ssId, QStringLiteral("odometerAtLastService"), QStringLiteral("Vehicle.Service.OdometerAtLastService"));
    reg(ssId, QStringLiteral("isOilChangeRequired"), QStringLiteral("Vehicle.Service.IsOilChangeRequired"));
    reg(ssId, QStringLiteral("isBrakeInspectionRequired"), QStringLiteral("Vehicle.Service.IsBrakeInspectionRequired"));
    reg(ssId, QStringLiteral("isTireRotationRequired"), QStringLiteral("Vehicle.Service.IsTireRotationRequired"));
    reg(ssId, QStringLiteral("engineOilLifeRemaining"), QStringLiteral("Vehicle.Service.EngineOilLifeRemaining"));

    // ── Connectivity module ─────────────────────────────────────────

    const QString cnId = QStringLiteral("COVESA.VSS.Connectivity.ConnectivityControl");
    reg(cnId, QStringLiteral("cellularStatus"), QStringLiteral("Vehicle.Connectivity.Cellular.Status"));
    reg(cnId, QStringLiteral("cellularSignalStrength"), QStringLiteral("Vehicle.Connectivity.Cellular.SignalStrength"));
    reg(cnId, QStringLiteral("cellularNetworkType"), QStringLiteral("Vehicle.Connectivity.Cellular.NetworkType"));
    reg(cnId, QStringLiteral("cellularOperator"), QStringLiteral("Vehicle.Connectivity.Cellular.Operator"));
    reg(cnId, QStringLiteral("wifiStatus"), QStringLiteral("Vehicle.Connectivity.WiFi.Status"));
    reg(cnId, QStringLiteral("wifiSignalStrength"), QStringLiteral("Vehicle.Connectivity.WiFi.SignalStrength"));
    reg(cnId, QStringLiteral("wifiSSID"), QStringLiteral("Vehicle.Connectivity.WiFi.SSID"));
    reg(cnId, QStringLiteral("isWifiHotspotEnabled"), QStringLiteral("Vehicle.Connectivity.WiFi.IsHotspotEnabled"));
    reg(cnId, QStringLiteral("bluetoothStatus"), QStringLiteral("Vehicle.Connectivity.Bluetooth.Status"));
    reg(cnId, QStringLiteral("bluetoothPairedDevices"), QStringLiteral("Vehicle.Connectivity.Bluetooth.PairedDevices"));
    reg(cnId, QStringLiteral("isBluetoothEnabled"), QStringLiteral("Vehicle.Connectivity.Bluetooth.IsEnabled"));

    // ── MotionManagement module ─────────────────────────────────────

    const QString bkId = QStringLiteral("COVESA.VSS.MotionManagement.BrakeControl");
    reg(bkId, QStringLiteral("pedalPositionTarget"), QStringLiteral("Vehicle.MotionManagement.Brake.PedalPosition"));
    reg(bkId, QStringLiteral("pedalPositionActual"), QStringLiteral("Vehicle.MotionManagement.Brake.PedalPositionActual"));
    reg(bkId, QStringLiteral("isBrakeActive"), QStringLiteral("Vehicle.MotionManagement.Brake.IsActive"));
    reg(bkId, QStringLiteral("brakeTorque"), QStringLiteral("Vehicle.MotionManagement.Brake.Torque"));

    const QString scId = QStringLiteral("COVESA.VSS.MotionManagement.SteeringControl");
    reg(scId, QStringLiteral("steeringAngleTarget"), QStringLiteral("Vehicle.MotionManagement.Steering.AngleTarget"));
    reg(scId, QStringLiteral("steeringAngleActual"), QStringLiteral("Vehicle.MotionManagement.Steering.AngleActual"));
    reg(scId, QStringLiteral("steeringTorque"), QStringLiteral("Vehicle.MotionManagement.Steering.Torque"));
    reg(scId, QStringLiteral("isSteerByWireEnabled"), QStringLiteral("Vehicle.MotionManagement.Steering.IsSteerByWireEnabled"));

    // SuspensionControl - zoned (FrontLeft, FrontRight, RearLeft, RearRight)
    const QString spId = QStringLiteral("COVESA.VSS.MotionManagement.SuspensionControl");
    const QStringList spZones = {
        QStringLiteral("FrontLeft"), QStringLiteral("FrontRight"),
        QStringLiteral("RearLeft"), QStringLiteral("RearRight")
    };
    regZones(spId, spZones, {
        {QStringLiteral("FrontLeft"), QStringLiteral("Row1.Left")},
        {QStringLiteral("FrontRight"), QStringLiteral("Row1.Right")},
        {QStringLiteral("RearLeft"), QStringLiteral("Row2.Left")},
        {QStringLiteral("RearRight"), QStringLiteral("Row2.Right")}
    });
    regZoned(spId, QStringLiteral("heightTarget"), QStringLiteral("Vehicle.MotionManagement.Suspension.*.HeightTarget"), spZones);
    regZoned(spId, QStringLiteral("heightActual"), QStringLiteral("Vehicle.MotionManagement.Suspension.*.HeightActual"), spZones);
    regZoned(spId, QStringLiteral("damperForce"), QStringLiteral("Vehicle.MotionManagement.Suspension.*.DamperForce"), spZones);
    // mode is non-zoned even though interface is zoned
    reg(spId, QStringLiteral("mode"), QStringLiteral("Vehicle.MotionManagement.Suspension.Mode"));
}

const VssPathMapping::Tables &VssPathMapping::tables()
{
    static const Tables t;
    return t;
}

QString VssPathMapping::vssPath(const QString &iid, const QString &property, const QString &zone)
{
    return tables().forward.value(makeKey(iid, property, zone));
}

QList<VssEntry> VssPathMapping::entries(const QString &vssPath)
{
    return tables().reverse.value(vssPath);
}

QStringList VssPathMapping::vssPaths(const QString &iid)
{
    return tables().iidPaths.value(iid);
}

QStringList VssPathMapping::allVssPaths()
{
    return tables().reverse.keys();
}

QStringList VssPathMapping::zones(const QString &iid)
{
    return tables().iidZones.value(iid);
}

QString VssPathMapping::zoneToVssInstance(const QString &iid, const QString &zone)
{
    return tables().zoneInstance.value(iid + QLatin1Char('\0') + zone, zone);
}
