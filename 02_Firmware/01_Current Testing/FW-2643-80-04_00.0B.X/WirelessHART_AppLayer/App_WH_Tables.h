#ifndef __CENTERO_APP_WH_TABLES_H_
#define __CENTERO_APP_WH_TABLES_H_
/**
 * Table 2. Engineering Unit Codes
 */
typedef enum
{
    UnitsCodes_DegreesCelsius = 32, /*!< 32 */
    UnitsCodes_DegreesFahrenheit = 33, /*!< 33 */
    UnitsCodes_DegreesRankine = 34, /*!< 34 */
    UnitsCodes_Kelvin = 35, /*!< 35 */ 
    UnitsCodes_Minutes = 50, /*!< 50 */
    UnitsCodes_Seconds = 51, /*!< 51 */
    UnitsCodes_Hours = 52, /*!< 52 */
    UnitsCodes_Days = 53, /*!< 53 */
    UnitsCodes_Milliamperes = 39, /*!< 39 */
    UnitsCodes_Percent = 57, /*!< 57 */
    UnitsCodes_Millivolts = 36, /*!< 36 */
    UnitsCodes_Volts = 58, /*!< 58 */
    UnitsCodes_Inches = 47, /*!< 47 */
    UnitsCodes_Millimeters = 49, /*!< 49 */

    // TBD !!!
    UnitsCodes_PartsPerMillion = 139, /*!< 139 */
    UnitsCodes_VolumePercent = 149, /*!< 149 */
    UnitsCodes_PercentLowerExplosionLevel = 161, /*!< 161 */
    UnitsCodes_PartsPerBillion = 169, /*!< 169 */
    UnitsCodes_MMPerYear = 240, /*!< 240 */
    UnitsCodes_MilsPerYear = 241, /*!< 241 */
    UnitsCodes_NotUsed = 250, /*!< 250 */
    UnitsCodes_None = 251 /*!< 251 */
} __attribute__((packed)) UnitsCodes;

/**Table 3. Transfer Function Codes
 */
typedef enum
{
    TransferFunctionCodes_Linear = 0, /*!< 0 */
    TransferFunctionCodes_SquareRoot = 1, /*!< 1 */
    TransferFunctionCodes_SquareRootThirdPower = 2, /*!< 2 */
    TransferFunctionCodes_SquareRootFifthPower = 3, /*!< 3 */
    TransferFunctionCodes_SpecialCurve = 4, /*!< 4 */
    TransferFunctionCodes_Square = 5, /*!< 5 */
    TransferFunctionCodes_Discrete = 230, /*!< 230 */
    TransferFunctionCodes_SquareRootPlusSpecialCurve = 231, /*!< 231 */
    TransferFunctionCodes_SquareRootThirdPowerPlusSpecialCurve = 232, /*!< 232 */
    TransferFunctionCodes_SquareRootFifthPowerPlusSpecialCurve = 233, /*!< 233 */
    //240-249          Enumeration May Be Used For Manufacturer Specific Definitions
    TransferFunctionCodes_None = 251, /*!< 251 */
    TransferFunctionCodes_Unknown = 252, /*!< 252 */
    TransferFunctionCodes_Special = 253 /*!< 253 */
} __attribute__((packed)) TransferFunctionCodes;

/** Table 6. Alarm Selection Code
 */
typedef enum
{
    High = 0, /*!< 0 */
    Low  = 1, /*!< 1 */
    HoldLastOutputValue = 239, /*!< 239 */
    Notused = 250, /*!< 250 */
    None = 251, /*!< 251 */
    Unknown = 252, /*!< 252 */
    Special = 253 /*!< 253 */
} __attribute__((packed)) AlarmSelectionCodes;

/**Table 7. Write Protect Codes
 */

typedef enum
{
    WriteProtectCodes_NO = 0, /*!< 0 */
    WriteProtectCodes_Yes = 1, /*!< 1 */
    WriteProtectCodes_NotUsed = 250, /*!< 250 */
    WriteProtectCodes_None = 251,   /*!< 251 - Not Implemented */
    WriteProtectCodes_Unknown = 252, /*!< 252 */
    WriteProtectCodes_Special = 253 /*!< 253 */
} __attribute__((packed)) WriteProtectCodes;

/** Table 9. Burst Mode Control Code
 */
typedef enum
{
    Off = 0, /*!< 0 */
    EnableOnTokenPassingDataLinkLayer = 1, /*!< 1 */
    EnableOnTDMADataLinkLayer = 2, /*!< 2 */
    EnableOnTDMAandTokenPassingDataLinkLayers = 3 /*!< 3 */
} __attribute__((packed)) BURST_MODE_CONTROL_CODE;

/** Table 34. Device Variable Code
 */
typedef enum
{
    BatteryLife         = 243, // F3 /*!< 243 */
    PercentRange        = 244, // F4 /*!< 244 */
    LoopCurrent         = 245, // F5 /*!< 245 */
    PrimaryVariable     = 246, // F6 /*!< 246 */
    SecondaryVariable   = 247, // F7 /*!< 247 */
    TertiaryVariable    = 248, // F8 /*!< 248 */
    QuaternaryVariable  = 249, // F9 /*!< 249 */
    NotUsed             = 250  // FA /*!< 250 */
} __attribute__((packed)) DEVICE_VARIABLE_CODE;

/**Table 33. Burst Message Trigger Mode
 */
typedef enum
{
    BurstMessageTriggerMode_Continuous = 0, /*!< 0 */
    BurstMessageTriggerMode_Window = 1, /*!< 1 */
    BurstMessageTriggerMode_Rising = 2, /*!< 2 */
    BurstMessageTriggerMode_Falling = 3, /*!< 3 */
    BurstMessageTriggerMode_OnChange = 4, /*!< 4 */
} __attribute__((packed)) BurstMessageTriggerMode;

/** Table 20 Code Device Variable Family
 */
typedef enum
{
    DeviceVariableFamilyCodes_Temperature = 4, /*!< 4 */
    DeviceVariableFamilyCodes_Pressure = 5, /*!< 5 */
    DeviceVariableFamilyCodes_Valve_Actuator = 6, /*!< 6 */
    DeviceVariableFamilyCodes_Simple_PID_Control = 7, /*!< 7 */
    DeviceVariableFamilyCodes_pH = 8, /*!< 8 */
    DeviceVariableFamilyCodes_Conductivity = 9, /*!< 9 */
    DeviceVariableFamilyCodes_Totalizer = 10, /*!< 10 */
    DeviceVariableFamilyCodes_Level = 11, /*!< 11 */
    DeviceVariableFamilyCodes_Vortex_Flow = 12, /*!< 12 */
    DeviceVariableFamilyCodes_Mag_Flow = 13, /*!< 13 */
    DeviceVariableFamilyCodes_Coriolis_Flow = 14, /*!< 14 */
    DeviceVariableFamilyCodes_NotUsed = 250, /*!< 250 */
} __attribute__((packed)) DeviceVariableFamilyCodes;

/**Table 21 Code	Device Variable Classification
 */
typedef enum
{
    DeviceVariableClassificationCodes_DeviceVariableNotClassified = 0, /*!< 0 */
    DeviceVariableClassificationCodes_Temperature = 64, /*!< 64 */
    DeviceVariableClassificationCodes_Pressure = 65, /*!< 65 */
    DeviceVariableClassificationCodes_VolumetricFlow = 66, /*!< 66 */
    DeviceVariableClassificationCodes_Velocity = 67, /*!< 67 */
    DeviceVariableClassificationCodes_Volume = 68, /*!< 68 */
    DeviceVariableClassificationCodes_Length = 69, /*!< 69 */
    DeviceVariableClassificationCodes_Time = 70, /*!< 70 */
    DeviceVariableClassificationCodes_Mass = 71, /*!< 71 */
    DeviceVariableClassificationCodes_MassFlow = 72, /*!< 72 */
    DeviceVariableClassificationCodes_MassPerVolume = 73, /*!< 73 */
    DeviceVariableClassificationCodes_Viscosity = 73, /*!< 74 */
    DeviceVariableClassificationCodes_AngularVelocity = 75, /*!< 75 */
    DeviceVariableClassificationCodes_Area = 76, /*!< 76 */
    DeviceVariableClassificationCodes_Energy = 77, /*!< 77 */
    DeviceVariableClassificationCodes_Force = 78, /*!< 78 */
    DeviceVariableClassificationCodes_Power = 79, /*!< 79 */
    DeviceVariableClassificationCodes_Frequency = 80, /*!< 80 */
    DeviceVariableClassificationCodes_Analytical = 81, /*!< 81 */
    DeviceVariableClassificationCodes_Capacitance = 82, /*!< 82 */
    DeviceVariableClassificationCodes_Emf = 83, /*!< 83 */
    DeviceVariableClassificationCodes_Current = 84, /*!< 84 */
    DeviceVariableClassificationCodes_Resistance = 85, /*!< 85 */
    DeviceVariableClassificationCodes_Angle = 86, /*!< 86 */
    DeviceVariableClassificationCodes_Conductance = 87, /*!< 87 */
    DeviceVariableClassificationCodes_VolumePerVolume = 88, /*!< 88 */
    DeviceVariableClassificationCodes_VolumePerMass = 89, /*!< 89 */
    DeviceVariableClassificationCodes_Concentration = 90, /*!< 90 */
    DeviceVariableClassificationCodes_ValveActuator = 91, /*!< 91 */
    DeviceVariableClassificationCodes_Level = 92, /*!< 92 */
    DeviceVariableClassificationCodes_VortexFlow = 93, /*!< 93 */
    DeviceVariableClassificationCodes_MagFlow = 94, /*!< 94 */
    DeviceVariableClassificationCodes_CoriolisFlow = 95, /*!< 95 */
    DeviceVariableClassificationCodes_NotUsed = 250  /*!< 250 *///spec 127r7.1 page 24
} __attribute__((packed)) DeviceVariableClassificationCodes;

/** Status masks
 */
typedef enum
{
    FieldDeviceStatusesMask_DeviceMalfunction = 0x80, /*!< 0x80 */
    FieldDeviceStatusesMask_ConfigurationChanged = 0x40, /*!< 0x40*/
    FieldDeviceStatusesMask_ColdStart = 0x20, /*!< 0x20 */
    FieldDeviceStatusesMask_MoreStatusAvailable = 0x10, /*!< 0x10 */
    FieldDeviceStatusesMask_LoopCurrentFixed = 0x08, /*!< 0x08 */
    FieldDeviceStatusesMask_LoopCurrentSaturated = 0x04, /*!< 0x04 */
    FieldDeviceStatusesMask_NonPrimaryVariableOutOfLimits = 0x02, /*!< 0x02 */
    FieldDeviceStatusesMask_PrimaryVariableOutOfLimits = 0x01 /*!< 0x01 */
} __attribute__((packed)) FieldDeviceStatusesMasks;

/**Table 19. Write Device Variable Codes
 */
typedef enum
{
    WriteDeviceVariableCodes_Normal = 0, /*!< 0 */
    WriteDeviceVariableCodes_FixedValue = 1 /*!< 1 */
} __attribute__((packed)) WriteDeviceVariableCodes;

/**Process data quality (bits 6,7 from DeviceVariableStatus)
 */
typedef enum
{
    ProcessDataQuality_Good = 3, /*!< 3 */
    ProcessDataQuality_PoorAccurace = 1, /*!< 1 */
    ProcessDataQuality_ManualFixed = 2, /*!< 2 */
    ProcessDataQuality_Bad = 0 /*!< 0 */
} __attribute__((packed)) ProcessDataQuality;

/**Limit status (bits 4,5 from DeviceVariableStatus)
 */
typedef enum
{
    LimitStatus_Constant = 3, /*!< 3 */
    LimitStatus_LowLimited = 1, /*!< 1 */
    LimitStatus_HighLimited = 2, /*!< 2 */
    LimitStatus_NotLimited = 0 /*!< 0 */
} __attribute__((packed)) LimitStatus;

/** Device Variable Status
 */
typedef struct
{
    uint8 device_family_status : 3; /*!< 3 */
    uint8 more_status_available : 1; /*!< 1 */
    /*LimitStatus*/uint8 limit_status : 2; /*!< 2 */
    /*ProcessDataQuality*/uint8 process_quality : 2; /*!< 2 */
} __attribute__((packed)) DeviceVariableStatus;

/**Table 16. Loop Current Mode Codes
 */
typedef enum
{
    LoopCurrent_Disabled = 0, /*!< 0 */
    LoopCurrent_Enabled = 1 /*!< 1 */
} __attribute__((packed)) LoopCurrentMode;

/**Table 35. Event Notification Control Code
*/
typedef enum
{
    EventNotificationControlCodes_Off = 0, /*!< 0 */
    EventNotificationControlCodes_EnableEventNotificationOnTokenPassingDataLinkLayer = 1,/*!< 1 */
    EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer = 2, /*!< 2 */
    EventNotificationControlCodes_EnableEventNotificationOnTDMATokenPassingDataLinkLayers = 3 /*!< 3 */
} __attribute__((packed)) EventNotificationControlCodes;

/**Table 36. Event Status
*/
typedef enum
{
    EventStatusCodesMask_ConfigurationChangedEventPending = 0x01, /*!< 0 */
    EventStatusCodesMask_DeviceStatusEventPending = 0x02, /*!< 2 */
    EventStatusCodesMask_MoreStatusAvailableEventPending = 0x04 /*!< 4 */
} __attribute__((packed)) EventStatusCodesMasks;

/**Table 57. Device Profile Code
 */
typedef enum
{
    DeviceProfileCodes_HART_PROCESS_AUTOMATION_DEVICE = 1, /*!< 1 */
    DeviceProfileCodes_HART_DISCRETE_DEVICE = 2, /*!< 2 */
    DeviceProfileCodes_HYBRID_PROCAUTOMATION_AND_DISCRETE = 3, /*!< 3 */
    DeviceProfileCodes_IO_SYSTEM = 4, /*!< 4 */
    DeviceProfileCodes_WIRELESSHART_PROCESS_AUTOMATION_DEVICE = 129, /*!< 129 */
    DeviceProfileCodes_WIRELESSHART_DISCRETE_DEVICE = 130, /*!< 130 */
    DeviceProfileCodes_WIRELESSHART_HYBRID_PROCAUTOMATION_AND_DISCRETE = 131, /*!< 131 */
    DeviceProfileCodes_WIRELESSHART_GATEWAY = 132, /*!< 132 */
    DeviceProfileCodes_WIRELESSHART_PROCESS_ADAPTER = 141, /*!< 141 */
    DeviceProfileCodes_WIRELESSHART_DISCRETE_ADAPTER = 142, /*!< 142 */
    DeviceProfileCodes_WIRELESSHART_HANDHELD_MAINTENANCE_TOOL = 144 /*!< 144 */
    //CHECKME too many - spec183r19.0
} __attribute__((packed)) DeviceProfileCodes;

/**Table 44. Device Power Source
 */
typedef enum
{
    DevicePowerSource_LinePower = 0, /*!< 0 */
    DevicePowerSource_BatteryPower = 1, /*!< 1 */
    DevicePowerSource_RechargeableBatteryPowerOrPowerScavenging = 2 /*!< 2 */
} __attribute__((packed)) DevicePowerSource;


#endif // __CENTERO_APP_WH_TABLES_H_
