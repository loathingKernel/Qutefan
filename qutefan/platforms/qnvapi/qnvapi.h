#ifndef QNVAPI_H
#define QNVAPI_H

#include <QMutex>
#include <QLibrary>
#include <QByteArray>
#include "include/nvapi.h"

/*! \class QNvAPI
 *  \brief A wrapper around the Nvidia NvAPI library.
 *
 *  \note nvapi.dll does not directly export the required symbols so
 *  we use the QueryInterface method with known function pointers. This
 *  may break with future versions of the library.
 *
*/

#define NVAPI_MAX_USAGES_PER_GPU    33
typedef struct _NV_GPU_USAGES_V1 {
    NvU32 version;
    NvU32 usage[NVAPI_MAX_USAGES_PER_GPU] = {};
} NV_GPU_USAGES_V1;
#define NV_GPU_USAGES_VER_1 MAKE_NVAPI_VERSION(NV_GPU_USAGES_V1,1)
typedef NV_GPU_USAGES_V1    NV_GPU_USAGES;
#define NV_GPU_USAGES_VER   NV_GPU_USAGES_VER_1


typedef enum _NV_COOLER_TYPE : NvS32 {
    NV_COOLER_TYPE_NONE = 0,
    NV_COOLER_TYPE_FAN,
    NV_COOLER_TYPE_WATER,
    NV_COOLER_TYPE_LIQUID_NO2,
} NV_COOLER_TYPE;

typedef enum _NV_COOLER_CONTROLLER : NvS32 {
    NV_COOLER_CONTROLLER_NONE = 0,
    NV_COOLER_CONTROLLER_ADI,
    NV_COOLER_CONTROLLER_INTERNAL,
} NV_COOLER_CONTROLLER;

typedef enum _NV_COOLER_POLICY : NvS32 {
    NV_COOLER_POLICY_NONE = 0,
    NV_COOLER_POLICY_MANUAL = 1,
    NV_COOLER_POLICY_PERF = 2,
    NV_COOLER_POLICY_DISCRETE = 4,
    NV_COOLER_POLICY_CONTINUOUS_HW = 8,
    NV_COOLER_POLICY_CONTINUOUS_SW = 16,
    NV_COOLER_POLICY_DEFAULT = 32,
} NV_COOLER_POLICY;

typedef enum _NV_COOLER_TARGET : NvS32 {
    NV_COOLER_TARGET_NONE = 0,
    NV_COOLER_TARGET_GPU = 1,
    NV_COOLER_TARGET_MEMORY = 2,
    NV_COOLER_TARGET_POWER_SUPPLY = 4,
    NV_COOLER_TARGET_ALL = 7,
 } NV_COOLER_TARGET;

typedef enum _NV_COOLER_CONTROL : NvS32 {
    NV_COOLER_CONTROL_NONE = 0,
    NV_COOLER_CONTROL_TOGGLE,
    NV_COOLER_CONTROL_VARIABLE,
} NV_COOLER_CONTROL;

typedef enum _NV_COOLER_ACTIVITY_LEVEL : NvS32 {
    NV_COOLER_ACTIVITY_LEVEL_INACTIVE = 0,
    NV_COOLER_ACTIVITY_LEVEL_ACTIVE = 1,
} NV_COOLER_ACTIVITY_LEVEL;

#define NVAPI_MAX_COOLERS_PER_GPU   3
// NV_GPU_COOLER_SETTINGS_V2 version only changed the number of coolers
//#define NVAPI_MAX_COOLERS_PER_GPU   20

typedef struct _NV_GPU_COOLER_SETTINGS_V1 {
    NvU32 version;
    NvU32 count = 0;
    struct {
        NV_COOLER_TYPE type;
        NV_COOLER_CONTROLLER controller;
        NvS32 default_level_min;
        NvS32 default_level_max;
        NvS32 current_level_min;
        NvS32 current_level_max;
        NvS32 current_level;
        NV_COOLER_POLICY default_policy;
        NV_COOLER_POLICY current_policy;
        NV_COOLER_TARGET target;
        NV_COOLER_CONTROL control_type;
        NV_COOLER_ACTIVITY_LEVEL active;
    } cooler[NVAPI_MAX_COOLERS_PER_GPU] = {};
} NV_GPU_COOLER_SETTINGS_V1;
#define NV_GPU_COOLER_SETTINGS_VER_1    MAKE_NVAPI_VERSION(NV_GPU_COOLER_SETTINGS_V1,1)
typedef NV_GPU_COOLER_SETTINGS_V1       NV_GPU_COOLER_SETTINGS;
#define NV_GPU_COOLER_SETTINGS_VER      NV_GPU_COOLER_SETTINGS_VER_1

typedef struct _NV_GPU_COOLER_LEVELS_V1 {
    NvU32 version;
    struct {
        NvS32 level;
        NV_COOLER_POLICY policy;
    } cooler[NVAPI_MAX_COOLERS_PER_GPU] = {};
} NV_GPU_COOLER_LEVELS_V1;
#define NV_GPU_COOLER_LEVELS_VER_1  MAKE_NVAPI_VERSION(NV_GPU_COOLER_LEVELS_V1,1)
typedef NV_GPU_COOLER_LEVELS_V1     NV_GPU_COOLER_LEVELS;
#define NV_GPU_COOLER_LEVELS_VER    NV_GPU_COOLER_LEVELS_VER_1


#define NVAPI_CLIENT_MAX_COOLERS_PER_GPU    32

typedef struct _NV_GPU_FAN_COOLERS_INFO_V1 {
    NvU32 version;
    NvU32 _reserved1;
    NvU32 count = 0;
    NvU32 _reserved2[8] = {};
    struct {
        NvU32 handle;
        NvU32 _reserved1;
        NvU32 _reserved2;
        NvU32 rpm_max;
        NvU32 _reserved3[8];
    } cooler[NVAPI_CLIENT_MAX_COOLERS_PER_GPU] = {};
} NV_GPU_FAN_COOLERS_INFO_V1;
#define NV_GPU_FAN_COOLERS_INFO_VER_1   MAKE_NVAPI_VERSION(NV_GPU_FAN_COOLERS_INFO_V1,1)
typedef NV_GPU_FAN_COOLERS_INFO_V1      NV_GPU_FAN_COOLERS_INFO;
#define NV_GPU_FAN_COOLERS_INFO_VER     NV_GPU_FAN_COOLERS_INFO_VER_1

typedef struct _NV_GPU_FAN_COOLERS_STATUS_V1 {
    NvU32 version;
    NvU32 count = 0;
    NvU32 _reserved1[8] = {};
    struct {
        NvU32 handle;
        NvU32 rpm;
        NvU32 minimum_level;
        NvU32 maximum_level;
        NvU32 current_level;
        NvU32 _reserved1[8];
    } cooler[NVAPI_CLIENT_MAX_COOLERS_PER_GPU] = {};
} NV_GPU_FAN_COOLERS_STATUS_V1;
#define NV_GPU_FAN_COOLERS_STATUS_VER_1 MAKE_NVAPI_VERSION(NV_GPU_FAN_COOLERS_STATUS_V1,1)
typedef NV_GPU_FAN_COOLERS_STATUS_V1    NV_GPU_FAN_COOLERS_STATUS;
#define NV_GPU_FAN_COOLERS_STATUS_VER   NV_GPU_FAN_COOLERS_STATUS_VER_1

typedef enum _NV_COOLER_CONTROL_MODE : NvU32 {
    NV_COOLER_CONTROL_MODE_AUTO = 0,
    NV_COOLER_CONTROL_MODE_MANUAL = 1,
} NV_COOLER_CONTROL_MODE;

typedef struct _NV_GPU_FAN_COOLERS_CONTROL_V1 {
    NvU32 version;
    NvU32 _reserved1 = 0;
    NvU32 count = 0;
    NvU32 _reserved2[8] = {};
    struct {
        NvU32 handle;
        NvU32 level;
        NV_COOLER_CONTROL_MODE mode;
        NvU32 _reserved1[8];
    } cooler[NVAPI_CLIENT_MAX_COOLERS_PER_GPU] = {};
} NV_GPU_FAN_COOLERS_CONTROL_V1;
#define NV_GPU_FAN_COOLERS_CONTROL_VER_1    MAKE_NVAPI_VERSION(NV_GPU_FAN_COOLERS_CONTROL_V1,1)
typedef NV_GPU_FAN_COOLERS_CONTROL_V1       NV_GPU_FAN_COOLERS_CONTROL;
#define NV_GPU_FAN_COOLERS_CONTROL_VER      NV_GPU_FAN_COOLERS_CONTROL_VER_1


typedef struct _NV_GPU_CLOCKS_V2 {
    NvU32 version;
    NvU32 clock[NVAPI_MAX_GPU_CLOCKS * 9] = {};
} NV_GPU_CLOCKS_V2;
#define NV_GPU_CLOCKS_VER_2 MAKE_NVAPI_VERSION(NV_GPU_CLOCKS_V2,2)
typedef NV_GPU_CLOCKS_V2    NV_GPU_CLOCKS;
#define NV_GPU_CLOCKS_VER   NV_GPU_CLOCKS_VER_2

class QNvAPI : public QLibrary
{
public:
    QNvAPI();
    ~QNvAPI();

    bool unload(void);

    NvAPI_Status Initialize(void);
    NvAPI_Status Unload(void);

    NvAPI_Status EnumNvidiaDisplayHandle(NvU32, NvDisplayHandle*);
    NvAPI_Status EnumPhysicalGPUs(NvPhysicalGpuHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32*);

    NvAPI_Status GetInterfaceVersionString(NvAPI_ShortString);
    NvAPI_Status GetDisplayDriverVersion(NvDisplayHandle, NV_DISPLAY_DRIVER_VERSION*);
    NvAPI_Status GetPhysicalGPUsFromDisplay(NvDisplayHandle, NvPhysicalGpuHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32*);
    NvAPI_Status GetPhysicalGPUFromGPUID(NvU32, NvPhysicalGpuHandle*);
    NvAPI_Status GetAssociatedNvidiaDisplayHandle(const char*,  NvDisplayHandle*);
    NvAPI_Status GetAssociatedDisplayOutputId(NvDisplayHandle, NvU32*);

    NvAPI_Status GetGPUIDFromPhysicalGPU(NvPhysicalGpuHandle, NvU32*);

    NvAPI_Status GPU_GetThermalSettings(NvPhysicalGpuHandle, NvU32, NV_GPU_THERMAL_SETTINGS*);
    NvAPI_Status GPU_GetFullName(NvPhysicalGpuHandle, NvAPI_ShortString);
    NvAPI_Status GPU_GetEDID(NvPhysicalGpuHandle, NvU32, NV_EDID*);
    NvAPI_Status GPU_GetTachReading(NvPhysicalGpuHandle, NvU32*);
    NvAPI_Status GPU_GetAllClocks(NvPhysicalGpuHandle, NV_GPU_CLOCKS*);
    NvAPI_Status GPU_GetAllClockFrequencies(NvPhysicalGpuHandle, NV_GPU_CLOCK_FREQUENCIES*);
    NvAPI_Status GPU_GetDynamicPstatesInfoEx(NvPhysicalGpuHandle, NV_GPU_DYNAMIC_PSTATES_INFO_EX*);
    NvAPI_Status GPU_GetMemoryInfo(NvPhysicalGpuHandle, NV_DISPLAY_DRIVER_MEMORY_INFO*);
    NvAPI_Status GPU_GetPCIIdentifiers(NvPhysicalGpuHandle, NvU32*, NvU32*, NvU32*, NvU32*);

    NvAPI_Status GPU_GetUsages(NvPhysicalGpuHandle, NV_GPU_USAGES*);
    // Non-RTX
    NvAPI_Status GPU_GetCoolerSettings(NvPhysicalGpuHandle, NvU32, NV_GPU_COOLER_SETTINGS*);
    NvAPI_Status GPU_SetCoolerLevels(NvPhysicalGpuHandle, NvU32, NV_GPU_COOLER_LEVELS*);
    //RTX
    NvAPI_Status GPU_GetClientFanCoolersInfo(NvPhysicalGpuHandle, NV_GPU_FAN_COOLERS_INFO*);
    NvAPI_Status GPU_GetClientFanCoolersStatus(NvPhysicalGpuHandle, NV_GPU_FAN_COOLERS_STATUS*);
    NvAPI_Status GPU_GetClientFanCoolersControl(NvPhysicalGpuHandle, NV_GPU_FAN_COOLERS_CONTROL*);
    NvAPI_Status GPU_SetClientFanCoolersControl(NvPhysicalGpuHandle, NV_GPU_FAN_COOLERS_CONTROL*);

    bool isAvailable(void);
    bool isRtx(NvPhysicalGpuHandle);

private:
    typedef NvAPI_Status (__cdecl * QNVAPI_QUERYINTERFACE)                   (unsigned int);

    typedef NvAPI_Status (__cdecl * QNVAPI_INITIALIZE)                       (void);
    typedef NvAPI_Status (__cdecl * QNVAPI_UNLOAD)                           (void);

    typedef NvAPI_Status (__cdecl * QNVAPI_ENUMNVIDIADISPLAYHANDLE)          (NvU32, NvDisplayHandle*);
    typedef NvAPI_Status (__cdecl * QNVAPI_ENUMPHYSICALGPUS)                 (NvPhysicalGpuHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32*);

    typedef NvAPI_Status (__cdecl * QNVAPI_GETINTERFACEVERSIONSTRING)        (NvAPI_ShortString);
    typedef NvAPI_Status (__cdecl * QNVAPI_GETDISPLAYDRIVERVERSION)          (NvDisplayHandle, NV_DISPLAY_DRIVER_VERSION*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GETPHYSICALGPUSFROMDISPLAY)       (NvDisplayHandle, NvPhysicalGpuHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GETPHYSICALGPUFROMGPUID)          (NvU32, NvPhysicalGpuHandle*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GETASSOCIATEDNVIDIADISPLAYHANDLE) (const char*,  NvDisplayHandle*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GETASSOCIATEDDISPLAYOUTPUTID)     (NvDisplayHandle, NvU32*);

    typedef NvAPI_Status (__cdecl * QNVAPI_GETGPUIDFROMPHYSICALGPU)          (NvPhysicalGpuHandle, NvU32*);

    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETTHERMALSETTINGS)           (NvPhysicalGpuHandle, NvU32, NV_GPU_THERMAL_SETTINGS*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETFULLNAME)                  (NvPhysicalGpuHandle, NvAPI_ShortString);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETEDID)                      (NvPhysicalGpuHandle, NvU32, NV_EDID*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETTACHREADING)               (NvPhysicalGpuHandle, NvU32*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETALLCLOCKS)                 (NvPhysicalGpuHandle, NV_GPU_CLOCKS*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETALLCLOCKFREQUENCIES)       (NvPhysicalGpuHandle, NV_GPU_CLOCK_FREQUENCIES*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETDYNAMICPSTATESINFOEX)      (NvPhysicalGpuHandle, NV_GPU_DYNAMIC_PSTATES_INFO_EX*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETMEMORYINFO)                (NvPhysicalGpuHandle, NV_DISPLAY_DRIVER_MEMORY_INFO*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETPCIIDENTIFIERS)            (NvPhysicalGpuHandle, NvU32*, NvU32*, NvU32*, NvU32*);

    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETUSAGES)                    (NvPhysicalGpuHandle, NV_GPU_USAGES*);
    // Non-RTX
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETCOOLERSETTINGS)            (NvPhysicalGpuHandle, NvU32, NV_GPU_COOLER_SETTINGS*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_SETCOOLERLEVELS)              (NvPhysicalGpuHandle, NvU32, NV_GPU_COOLER_LEVELS*);
    // RTX
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETCLIENTFANCOOLERSINFO)      (NvPhysicalGpuHandle, NV_GPU_FAN_COOLERS_INFO*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETCLIENTFANCOOLERSSTATUS)    (NvPhysicalGpuHandle, NV_GPU_FAN_COOLERS_STATUS*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETCLIENTFANCOOLERSCONTROL)   (NvPhysicalGpuHandle, NV_GPU_FAN_COOLERS_CONTROL*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_SETCLIENTFANCOOLERSCONTROL)   (NvPhysicalGpuHandle, NV_GPU_FAN_COOLERS_CONTROL*);

    QNVAPI_QUERYINTERFACE                   nvapi_QueryInterface                    = nullptr;

    QNVAPI_INITIALIZE                       nvapi_Initialize                        = nullptr;
    QNVAPI_UNLOAD                           nvapi_Unload                            = nullptr;

    QNVAPI_ENUMNVIDIADISPLAYHANDLE          nvapi_EnumNvidiaDisplayHandle           = nullptr;
    QNVAPI_ENUMPHYSICALGPUS                 nvapi_EnumPhysicalGPUs                  = nullptr;

    QNVAPI_GETINTERFACEVERSIONSTRING        nvapi_GetInterfaceVersionString         = nullptr;
    QNVAPI_GETDISPLAYDRIVERVERSION          nvapi_GetDisplayDriverVersion           = nullptr;
    QNVAPI_GETPHYSICALGPUSFROMDISPLAY       nvapi_GetPhysicalGPUsFromDisplay        = nullptr;
    QNVAPI_GETPHYSICALGPUFROMGPUID          nvapi_GetPhysicalGPUFromGPUID           = nullptr;
    QNVAPI_GETASSOCIATEDNVIDIADISPLAYHANDLE nvapi_GetAssociatedNvidiaDisplayHandle  = nullptr;
    QNVAPI_GETASSOCIATEDDISPLAYOUTPUTID     nvapi_GetAssociatedDisplayOutputId      = nullptr;

    QNVAPI_GETGPUIDFROMPHYSICALGPU          nvapi_GetGPUIDFromPhysicalGPU           = nullptr;

    QNVAPI_GPU_GETTHERMALSETTINGS           nvapi_GPU_GetThermalSettings            = nullptr;
    QNVAPI_GPU_GETFULLNAME                  nvapi_GPU_GetFullName                   = nullptr;
    QNVAPI_GPU_GETEDID                      nvapi_GPU_GetEDID                       = nullptr;
    QNVAPI_GPU_GETTACHREADING               nvapi_GPU_GetTachReading                = nullptr;
    QNVAPI_GPU_GETALLCLOCKS                 nvapi_GPU_GetAllClocks                  = nullptr;
    QNVAPI_GPU_GETALLCLOCKFREQUENCIES       nvapi_GPU_GetAllClockFrequencies        = nullptr;
    QNVAPI_GPU_GETDYNAMICPSTATESINFOEX      nvapi_GPU_GetDynamicPstatesInfoEx       = nullptr;
    QNVAPI_GPU_GETMEMORYINFO                nvapi_GPU_GetMemoryInfo                 = nullptr;
    QNVAPI_GPU_GETPCIIDENTIFIERS            nvapi_GPU_GetPCIIdentifiers             = nullptr;

    QNVAPI_GPU_GETUSAGES                    nvapi_GPU_GetUsages                     = nullptr;
    // Non-RTX
    QNVAPI_GPU_GETCOOLERSETTINGS            nvapi_GPU_GetCoolerSettings             = nullptr;
    QNVAPI_GPU_SETCOOLERLEVELS              nvapi_GPU_SetCoolerLevels               = nullptr;
    // RTX
    QNVAPI_GPU_GETCLIENTFANCOOLERSINFO      nvapi_GPU_GetClientFanCoolersInfo       = nullptr;
    QNVAPI_GPU_GETCLIENTFANCOOLERSSTATUS    nvapi_GPU_GetClientFanCoolersStatus     = nullptr;
    QNVAPI_GPU_GETCLIENTFANCOOLERSCONTROL   nvapi_GPU_GetClientFanCoolersControl    = nullptr;
    QNVAPI_GPU_SETCLIENTFANCOOLERSCONTROL   nvapi_GPU_SetClientFanCoolersControl    = nullptr;
};

#endif // QNVAPI_H
