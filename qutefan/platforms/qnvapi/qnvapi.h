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
typedef struct {
    NvU32 version;
    NvU32 usage[NVAPI_MAX_USAGES_PER_GPU];
} NV_GPU_USAGES_V1;
#define NV_GPU_USAGES_VER_1 MAKE_NVAPI_VERSION(NV_GPU_USAGES_V1,1)
typedef NV_GPU_USAGES_V1    NV_GPU_USAGES;
#define NV_GPU_USAGES_VER   NV_GPU_USAGES_VER_1


#define NVAPI_MAX_COOLERS_PER_GPU   20
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
#define NV_GPU_COOLER_SETTINGS_VER_2    MAKE_NVAPI_VERSION(NV_GPU_COOLER_SETTINGS_V2,2)
typedef NV_GPU_COOLER_SETTINGS_V2       NV_GPU_COOLER_SETTINGS;
#define NV_GPU_COOLER_SETTINGS_VER      NV_GPU_COOLER_SETTINGS_VER_2


typedef struct {
    NvU32 version;
    struct {
        NvS32 level;
        NvS32 policy;
    } cooler[NVAPI_MAX_COOLERS_PER_GPU];
} NV_GPU_COOLER_LEVELS_V1;
#define NV_GPU_COOLER_LEVELS_VER_1  MAKE_NVAPI_VERSION(NV_GPU_COOLER_LEVELS_V1,1)
typedef NV_GPU_COOLER_LEVELS_V1     NV_GPU_COOLER_LEVELS;
#define NV_GPU_COOLER_LEVELS_VER    NV_GPU_COOLER_LEVELS_VER_1


typedef struct {
    NvU32 version;
    NvU32 clock[NVAPI_MAX_GPU_CLOCKS * 9];
} NV_GPU_CLOCKS_V2;
#define NV_GPU_CLOCKS_VER_2 MAKE_NVAPI_VERSION(NV_GPU_CLOCKS_V2,2)
typedef NV_GPU_CLOCKS_V2    NV_GPU_CLOCKS;
#define NV_GPU_CLOCKS_VER   NV_GPU_CLOCKS_VER_2


class QNvAPI : public QLibrary
{
public:
    QNvAPI();
    ~QNvAPI();

    NvAPI_Status Initialize(void);

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
    NvAPI_Status GPU_GetDynamicPstatesInfoEx(NvPhysicalGpuHandle, NV_GPU_DYNAMIC_PSTATES_INFO_EX*);
    NvAPI_Status GPU_GetMemoryInfo(NvPhysicalGpuHandle, NV_DISPLAY_DRIVER_MEMORY_INFO*);
    NvAPI_Status GPU_GetPCIIdentifiers(NvPhysicalGpuHandle, NvU32*, NvU32*, NvU32*, NvU32*);

    NvAPI_Status GPU_GetUsages(NvPhysicalGpuHandle, NV_GPU_USAGES*);
    NvAPI_Status GPU_GetCoolerSettings(NvPhysicalGpuHandle, NvU32, NV_GPU_COOLER_SETTINGS*);

    NvAPI_Status GPU_SetCoolerLevels(NvPhysicalGpuHandle, NvU32, NV_GPU_COOLER_LEVELS*);

    bool isAvailable(void);

private:
    typedef NvAPI_Status (__cdecl * QNVAPI_QUERYINTERFACE)                   (unsigned int);
    QNVAPI_QUERYINTERFACE                   nvapi_QueryInterface;

    typedef NvAPI_Status (__cdecl * QNVAPI_INITIALIZE)                       (void);
    QNVAPI_INITIALIZE                       nvapi_Initialize;

    typedef NvAPI_Status (__cdecl * QNVAPI_ENUMNVIDIADISPLAYHANDLE)          (NvU32, NvDisplayHandle*);
    QNVAPI_ENUMNVIDIADISPLAYHANDLE          nvapi_EnumNvidiaDisplayHandle;
    typedef NvAPI_Status (__cdecl * QNVAPI_ENUMPHYSICALGPUS)                 (NvPhysicalGpuHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32*);
    QNVAPI_ENUMPHYSICALGPUS                 nvapi_EnumPhysicalGPUs;

    typedef NvAPI_Status (__cdecl * QNVAPI_GETINTERFACEVERSIONSTRING)        (NvAPI_ShortString);
    QNVAPI_GETINTERFACEVERSIONSTRING        nvapi_GetInterfaceVersionString;
    typedef NvAPI_Status (__cdecl * QNVAPI_GETDISPLAYDRIVERVERSION)          (NvDisplayHandle, NV_DISPLAY_DRIVER_VERSION*);
    QNVAPI_GETDISPLAYDRIVERVERSION          nvapi_GetDisplayDriverVersion;
    typedef NvAPI_Status (__cdecl * QNVAPI_GETPHYSICALGPUSFROMDISPLAY)       (NvDisplayHandle, NvPhysicalGpuHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32*);
    QNVAPI_GETPHYSICALGPUSFROMDISPLAY       nvapi_GetPhysicalGPUsFromDisplay;
    typedef NvAPI_Status (__cdecl * QNVAPI_GETPHYSICALGPUFROMGPUID)          (NvU32, NvPhysicalGpuHandle*);
    QNVAPI_GETPHYSICALGPUFROMGPUID          nvapi_GetPhysicalGPUFromGPUID;
    typedef NvAPI_Status (__cdecl * QNVAPI_GETASSOCIATEDNVIDIADISPLAYHANDLE) (const char*,  NvDisplayHandle*);
    QNVAPI_GETASSOCIATEDNVIDIADISPLAYHANDLE nvapi_GetAssociatedNvidiaDisplayHandle;
    typedef NvAPI_Status (__cdecl * QNVAPI_GETASSOCIATEDDISPLAYOUTPUTID)     (NvDisplayHandle, NvU32*);
    QNVAPI_GETASSOCIATEDDISPLAYOUTPUTID     nvapi_GetAssociatedDisplayOutputId;

    typedef NvAPI_Status (__cdecl * QNVAPI_GETGPUIDFROMPHYSICALGPU)          (NvPhysicalGpuHandle, NvU32*);
    QNVAPI_GETGPUIDFROMPHYSICALGPU          nvapi_GetGPUIDFromPhysicalGPU;

    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETTHERMALSETTINGS)           (NvPhysicalGpuHandle, NvU32, NV_GPU_THERMAL_SETTINGS*);
    QNVAPI_GPU_GETTHERMALSETTINGS           nvapi_GPU_GetThermalSettings;
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETFULLNAME)                  (NvPhysicalGpuHandle, NvAPI_ShortString);
    QNVAPI_GPU_GETFULLNAME                  nvapi_GPU_GetFullName;
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETEDID)                      (NvPhysicalGpuHandle, NvU32, NV_EDID*);
    QNVAPI_GPU_GETEDID                      nvapi_GPU_GetEDID;
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETTACHREADING)               (NvPhysicalGpuHandle, NvU32*);
    QNVAPI_GPU_GETTACHREADING               nvapi_GPU_GetTachReading;
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETALLCLOCKS)                 (NvPhysicalGpuHandle, NV_GPU_CLOCKS*);
    QNVAPI_GPU_GETALLCLOCKS                 nvapi_GPU_GetAllClocks;
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETDYNAMICPSTATESINFOEX)      (NvPhysicalGpuHandle, NV_GPU_DYNAMIC_PSTATES_INFO_EX*);
    QNVAPI_GPU_GETDYNAMICPSTATESINFOEX      nvapi_GPU_GetDynamicPstatesInfoEx;
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETMEMORYINFO)                (NvPhysicalGpuHandle, NV_DISPLAY_DRIVER_MEMORY_INFO*);
    QNVAPI_GPU_GETMEMORYINFO                nvapi_GPU_GetMemoryInfo;
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETPCIIDENTIFIERS)            (NvPhysicalGpuHandle, NvU32*, NvU32*, NvU32*, NvU32*);
    QNVAPI_GPU_GETPCIIDENTIFIERS            nvapi_GPU_GetPCIIdentifiers;

    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETUSAGES)                    (NvPhysicalGpuHandle, NV_GPU_USAGES*);
    QNVAPI_GPU_GETUSAGES                    nvapi_GPU_GetUsages;

    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_GETCOOLERSETTINGS)            (NvPhysicalGpuHandle, NvU32, NV_GPU_COOLER_SETTINGS*);
    QNVAPI_GPU_GETCOOLERSETTINGS            nvapi_GPU_GetCoolerSettings;
    typedef NvAPI_Status (__cdecl * QNVAPI_GPU_SETCOOLERLEVELS)              (NvPhysicalGpuHandle, NvU32, NV_GPU_COOLER_LEVELS*);
    QNVAPI_GPU_SETCOOLERLEVELS              nvapi_GPU_SetCoolerLevels;
};

#endif // QNVAPI_H
