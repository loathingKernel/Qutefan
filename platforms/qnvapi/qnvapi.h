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

#define NVAPI_MAX_COOLERS_PER_GPU   20
#define NVAPI_MAX_USAGES_PER_GPU    33
#define NVAPI_MIN_COOLER_LEVEL      30

#define NV_GPU_USAGES_VER_1 MAKE_NVAPI_VERSION(NV_GPU_USAGES_V1,1)
#define NV_GPU_USAGES_VER   NV_GPU_USAGES_VER_1

#define NV_GPU_COOLER_SETTINGS_VER_2    MAKE_NVAPI_VERSION(NV_GPU_COOLER_SETTINGS_V2,2)
#define NV_GPU_COOLER_SETTINGS_VER      NV_GPU_COOLER_SETTINGS_VER_2

#define NV_GPU_COOLER_LEVELS_VER_1  MAKE_NVAPI_VERSION(NV_GPU_COOLER_LEVELS_V1,1)
#define NV_GPU_COOLER_LEVELS_VER    NV_GPU_COOLER_LEVELS_VER_1

typedef struct {
    NvU32 version;
    NvU32 usage[NVAPI_MAX_USAGES_PER_GPU];
} NV_GPU_USAGES_V1;
typedef NV_GPU_USAGES_V1    NV_GPU_USAGES;

typedef struct {
    NvU32 version;
    NvU32 count;
    struct {
        NvS32 type;
        NvS32 controller;
        NvS32 defaultMin;
        NvS32 defaultMax;
        NvS32 currentMin;
        NvS32 currentMax;
        NvS32 currentLevel;
        NvS32 defaultPolicy;
        NvS32 currentPolicy;
        NvS32 target;
        NvS32 controlType;
        NvS32 active;
    } cooler[NVAPI_MAX_COOLERS_PER_GPU];
} NV_GPU_COOLER_SETTINGS_V2;
typedef NV_GPU_COOLER_SETTINGS_V2   NV_GPU_COOLER_SETTINGS;

typedef struct {
    NvU32 version;
    struct {
        NvS32 level;
        NvS32 policy;
    } cooler[NVAPI_MAX_COOLERS_PER_GPU];
} NV_GPU_COOLER_LEVELS_V1;
typedef NV_GPU_COOLER_LEVELS_V1 NV_GPU_COOLER_LEVELS;


class QNvAPI : public QLibrary
{
public:

#pragma warning(disable : 4351)
    NvAPI_Status status = NVAPI_OK;
    NvAPI_ShortString version = {};

    NV_DISPLAY_DRIVER_VERSION driverVersion = {};

    NvU32 displayCount = 0;
    NvDisplayHandle displayHandle[NVAPI_MAX_PHYSICAL_GPUS*NVAPI_MAX_DISPLAY_HEADS] = {};

    NvU32 gpuCount = 0;
    typedef struct {
        NvPhysicalGpuHandle handle;
        NvAPI_ShortString name;
        NvU32 coolerTach;
        NV_GPU_THERMAL_SETTINGS thermalSettings;
        NV_GPU_COOLER_SETTINGS coolerSettings;
        NV_GPU_COOLER_LEVELS coolerLevels;
    } NvGPU;
    NvGPU gpu[NVAPI_MAX_PHYSICAL_GPUS] = {};

    QNvAPI(void);

    NvAPI_Status Initialize(void);

    NvAPI_Status EnumNvidiaDisplayHandle(NvU32, NvDisplayHandle*);
    NvAPI_Status EnumPhysicalGPUs(NvPhysicalGpuHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32*);
    NvAPI_Status EnumPhysicalGPUs();

    NvAPI_Status GetInterfaceVersionString(NvAPI_ShortString);
    NvAPI_Status GetDisplayDriverVersion(NvDisplayHandle, NV_DISPLAY_DRIVER_VERSION*);
    NvAPI_Status GetPhysicalGPUsFromDisplay(NvDisplayHandle, NvPhysicalGpuHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32*);
    NvAPI_Status GetAssociatedNvidiaDisplayHandle(const char*,  NvDisplayHandle*);
    NvAPI_Status GetAssociatedDisplayOutputId(NvDisplayHandle, NvU32*);

    NvAPI_Status GPU_GetThermalSettings(NvPhysicalGpuHandle, NvU32, NV_GPU_THERMAL_SETTINGS*);
    NvAPI_Status GPU_GetFullName(NvPhysicalGpuHandle, NvAPI_ShortString);
    NvAPI_Status GPU_GetEDID(NvPhysicalGpuHandle, NvU32, NV_EDID*);
    NvAPI_Status GPU_GetTachReading(NvPhysicalGpuHandle, NvU32*);
    NvAPI_Status GPU_GetAllClockFrequencies(NvPhysicalGpuHandle, NV_GPU_CLOCK_FREQUENCIES*);
    NvAPI_Status GPU_GetPstates20(NvPhysicalGpuHandle, NV_GPU_PERF_PSTATES20_INFO*);
    NvAPI_Status GPU_GetMemoryInfo(NvPhysicalGpuHandle, NV_DISPLAY_DRIVER_MEMORY_INFO*);
    NvAPI_Status GPU_GetPCIIdentifiers(NvPhysicalGpuHandle, NvU32*, NvU32*, NvU32*, NvU32*);

    NvAPI_Status GPU_GetUsages(NvPhysicalGpuHandle, NV_GPU_USAGES*);
    NvAPI_Status GPU_GetCoolerSettings(NvPhysicalGpuHandle, NvU32, NV_GPU_COOLER_SETTINGS*);

    NvAPI_Status GPU_SetCoolerLevels(NvPhysicalGpuHandle, NvU32, NV_GPU_COOLER_LEVELS*);

    bool isAvailable(void);

private:
    typedef NvAPI_Status (__cdecl * QNVAPI_QUERYINTERFACE)                   (unsigned int);
    typedef NvAPI_Status (__cdecl * QNVAPI_INITIALIZE)                       (void);

    typedef NvAPI_Status (__cdecl * QNVAPI_ENUMNVIDIADISPLAYHANDLE)          (NvU32, NvDisplayHandle*);
    typedef NvAPI_Status (__cdecl * QNVAPI_ENUMPHYSICALGPUS)                 (NvPhysicalGpuHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32*);

    typedef NvAPI_Status (__cdecl * QNVAPI_GETINTERFACEVERSIONSTRING)        (NvAPI_ShortString);
    typedef NvAPI_Status (__cdecl * QNVAPI_GETDISPLAYDRIVERVERSION)          (NvDisplayHandle, NV_DISPLAY_DRIVER_VERSION*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GETPHYSICALGPUSFROMDISPLAY)       (NvDisplayHandle, NvPhysicalGpuHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GETASSOCIATEDNVIDIADISPLAYHANDLE) (const char*,  NvDisplayHandle*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GETASSOCIATEDDISPLAYOUTPUTID)     (NvDisplayHandle, NvU32*);

    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETTHERMALSETTINGS)           (NvPhysicalGpuHandle, NvU32, NV_GPU_THERMAL_SETTINGS*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETFULLNAME)                  (NvPhysicalGpuHandle, NvAPI_ShortString);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETEDID)                      (NvPhysicalGpuHandle, NvU32, NV_EDID*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETTACHREADING)               (NvPhysicalGpuHandle, NvU32*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETALLCLOCKFREQUENCIES)       (NvPhysicalGpuHandle, NV_GPU_CLOCK_FREQUENCIES*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETPSTATES20)                 (NvPhysicalGpuHandle, NV_GPU_PERF_PSTATES20_INFO*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETMEMORYINFO)                (NvPhysicalGpuHandle, NV_DISPLAY_DRIVER_MEMORY_INFO*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETPCIIDENTIFIERS)            (NvPhysicalGpuHandle, NvU32*, NvU32*, NvU32*, NvU32*);

    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETUSAGES)                    (NvPhysicalGpuHandle, NV_GPU_USAGES*);
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETCOOLERSETTINGS)            (NvPhysicalGpuHandle, NvU32, NV_GPU_COOLER_SETTINGS*);

    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_SETCOOLERLEVELS)              (NvPhysicalGpuHandle, NvU32, NV_GPU_COOLER_LEVELS*);

    QNVAPI_QUERYINTERFACE                   nvapi_QueryInterface;
    QNVAPI_INITIALIZE                       nvapi_Initialize;

    QNVAPI_ENUMNVIDIADISPLAYHANDLE          nvapi_EnumNvidiaDisplayHandle;
    QNVAPI_ENUMPHYSICALGPUS                 nvapi_EnumPhysicalGPUs;

    QNVAPI_GETINTERFACEVERSIONSTRING        nvapi_GetInterfaceVersionString;
    QNVAPI_GETDISPLAYDRIVERVERSION          nvapi_GetDisplayDriverVersion;
    QNVAPI_GETPHYSICALGPUSFROMDISPLAY       nvapi_GetPhysicalGPUsFromDisplay;
    QNVAPI_GETASSOCIATEDNVIDIADISPLAYHANDLE nvapi_GetAssociatedNvidiaDisplayHandle;
    QNVAPI_GETASSOCIATEDDISPLAYOUTPUTID     nvapi_GetAssociatedDisplayOutputId;

    QNVAPI_GPU_GETTHERMALSETTINGS           nvapi_GPU_GetThermalSettings;
    QNVAPI_GPU_GETFULLNAME                  nvapi_GPU_GetFullName;
    QNVAPI_GPU_GETEDID                      nvapi_GPU_GetEDID;
    QNVAPI_GPU_GETTACHREADING               nvapi_GPU_GetTachReading;
    QNVAPI_GPU_GETALLCLOCKFREQUENCIES       nvapi_GPU_GetAllClockFrequencies;
    QNVAPI_GPU_GETPSTATES20                 nvapi_GPU_GetPstates20;
    QNVAPI_GPU_GETMEMORYINFO                nvapi_GPU_GetMemoryInfo;
    QNVAPI_GPU_GETPCIIDENTIFIERS            nvapi_GPU_GetPCIIdentifiers;

    QNVAPI_GPU_GETUSAGES                    nvapi_GPU_GetUsages;
    QNVAPI_GPU_GETCOOLERSETTINGS            nvapi_GPU_GetCoolerSettings;

    QNVAPI_GPU_SETCOOLERLEVELS              nvapi_GPU_SetCoolerLevels;

    bool                                    nvapi_initialized;
    NvAPI_Status                            nvapi_status;
};

#endif // QNVAPI_H
