#include "nvapi_glue.h"

#include <QDebug>
//#include <QMutex>

//static QMutex* nvapi_lock = new QMutex(QMutex::Recursive);

#if defined(Q_PROCESSOR_X86_64)
    #define NVAPI_NAME "nvapi64.dll"
#elif defined(Q_PROCESSOR_X86_32)
    #define NVAPI_NAME "nvapi.dll"
#else
    #error "Uknown architecture"
#endif

QNvAPIGlue::QNvAPIGlue(QObject *parent) : QLibrary(parent)
{
//    QMutexLocker locker(nvapi_lock);

    setFileName(NVAPI_NAME);

    nvapi_QueryInterface = reinterpret_cast<QNVAPI_QUERYINTERFACE>(resolve("nvapi_QueryInterface"));

    if(!nvapi_QueryInterface) {
        NvAPI_Status status = NVAPI_LIBRARY_NOT_FOUND;
        qDebug("NvAPI_QueryInterface() failed with status %d", status);
    } else {
        nvapi_Initialize                        = reinterpret_cast<QNVAPI_INITIALIZE>                       (nvapi_QueryInterface(0x0150E828));
        nvapi_Unload                            = reinterpret_cast<QNVAPI_UNLOAD>                           (nvapi_QueryInterface(0xD22BDD7E));

        nvapi_EnumNvidiaDisplayHandle           = reinterpret_cast<QNVAPI_ENUMNVIDIADISPLAYHANDLE>          (nvapi_QueryInterface(0x9ABDD40D));
        nvapi_EnumPhysicalGPUs                  = reinterpret_cast<QNVAPI_ENUMPHYSICALGPUS>                 (nvapi_QueryInterface(0xE5AC921F));


        nvapi_GetInterfaceVersionString         = reinterpret_cast<QNVAPI_GETINTERFACEVERSIONSTRING>        (nvapi_QueryInterface(0x01053FA5));
        nvapi_GetDisplayDriverVersion           = reinterpret_cast<QNVAPI_GETDISPLAYDRIVERVERSION>          (nvapi_QueryInterface(0xF951A4D1));
        nvapi_GetPhysicalGPUsFromDisplay        = reinterpret_cast<QNVAPI_GETPHYSICALGPUSFROMDISPLAY>       (nvapi_QueryInterface(0x34EF9506));
        nvapi_GetPhysicalGPUFromGPUID           = reinterpret_cast<QNVAPI_GETPHYSICALGPUFROMGPUID>          (nvapi_QueryInterface(0x5380AD1A));
        nvapi_GetAssociatedNvidiaDisplayHandle  = reinterpret_cast<QNVAPI_GETASSOCIATEDNVIDIADISPLAYHANDLE> (nvapi_QueryInterface(0x35C29134));
        nvapi_GetAssociatedDisplayOutputId      = reinterpret_cast<QNVAPI_GETASSOCIATEDDISPLAYOUTPUTID>     (nvapi_QueryInterface(0xD995937E));

        nvapi_GetGPUIDFromPhysicalGPU           = reinterpret_cast<QNVAPI_GETGPUIDFROMPHYSICALGPU>          (nvapi_QueryInterface(0x6533EA3E));

        nvapi_GPU_GetThermalSettings            = reinterpret_cast<QNVAPI_GPU_GETTHERMALSETTINGS>           (nvapi_QueryInterface(0xE3640A56));
        nvapi_GPU_GetFullName                   = reinterpret_cast<QNVAPI_GPU_GETFULLNAME>                  (nvapi_QueryInterface(0xCEEE8E9F));
        nvapi_GPU_GetEDID                       = reinterpret_cast<QNVAPI_GPU_GETEDID>                      (nvapi_QueryInterface(0x37D32E69));
        nvapi_GPU_GetTachReading                = reinterpret_cast<QNVAPI_GPU_GETTACHREADING>               (nvapi_QueryInterface(0x5F608315));
        nvapi_GPU_GetAllClocks                  = reinterpret_cast<QNVAPI_GPU_GETALLCLOCKS>                 (nvapi_QueryInterface(0x1BD69F49));
        nvapi_GPU_GetAllClockFrequencies        = reinterpret_cast<QNVAPI_GPU_GETALLCLOCKFREQUENCIES>       (nvapi_QueryInterface(0xDCB616C3));
        nvapi_GPU_GetDynamicPstatesInfoEx       = reinterpret_cast<QNVAPI_GPU_GETDYNAMICPSTATESINFOEX>      (nvapi_QueryInterface(0x60DED2ED));
        nvapi_GPU_GetMemoryInfo                 = reinterpret_cast<QNVAPI_GPU_GETMEMORYINFO>                (nvapi_QueryInterface(0x774AA982));
        nvapi_GPU_GetPCIIdentifiers             = reinterpret_cast<QNVAPI_GPU_GETPCIIDENTIFIERS>            (nvapi_QueryInterface(0x2DDFB66E));

        nvapi_GPU_GetUsages                     = reinterpret_cast<QNVAPI_GPU_GETUSAGES>                    (nvapi_QueryInterface(0x189A1FDF));
        // Non-RTX
        nvapi_GPU_GetCoolerSettings             = reinterpret_cast<QNVAPI_GPU_GETCOOLERSETTINGS>            (nvapi_QueryInterface(0xDA141340));
        nvapi_GPU_SetCoolerLevels               = reinterpret_cast<QNVAPI_GPU_SETCOOLERLEVELS>              (nvapi_QueryInterface(0x891FA0AE));
        // RTX
        nvapi_GPU_GetClientFanCoolersInfo       = reinterpret_cast<QNVAPI_GPU_GETCLIENTFANCOOLERSINFO>      (nvapi_QueryInterface(0xFB85B01E));
        nvapi_GPU_GetClientFanCoolersStatus     = reinterpret_cast<QNVAPI_GPU_GETCLIENTFANCOOLERSSTATUS>    (nvapi_QueryInterface(0x35AED5E8));
        nvapi_GPU_GetClientFanCoolersControl    = reinterpret_cast<QNVAPI_GPU_GETCLIENTFANCOOLERSCONTROL>   (nvapi_QueryInterface(0x814B209F));
        nvapi_GPU_SetClientFanCoolersControl    = reinterpret_cast<QNVAPI_GPU_SETCLIENTFANCOOLERSCONTROL>   (nvapi_QueryInterface(0xA58971A5));
    }
}

QNvAPIGlue::~QNvAPIGlue()
{
    unload();
}

bool QNvAPIGlue::unload()
{
    NvAPI_Status status = Unload();
    return status == NVAPI_OK && QLibrary::unload();
}

NvAPI_Status QNvAPIGlue::Initialize(void)
{
    NvAPI_Status status = nvapi_Initialize();
    if(status != NVAPI_OK)
        qDebug("%s failed with status %d", __PRETTY_FUNCTION__, status);
    return status;
}

NvAPI_Status QNvAPIGlue::Unload(void)
{
    NvAPI_Status status = nvapi_Unload();
    if(status != NVAPI_OK)
        qDebug("%s failed with status %d", __PRETTY_FUNCTION__, status);
    return status;
}

NvAPI_Status QNvAPIGlue::EnumNvidiaDisplayHandle(NvU32 thisEnum, NvDisplayHandle* pNvDispHandle)
{
    NvAPI_Status status = nvapi_EnumNvidiaDisplayHandle(thisEnum, pNvDispHandle);
    if(status != NVAPI_OK)
        qDebug("NvAPI_EnumNvidiaDisplayHandle() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::EnumPhysicalGPUs(NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32* pGpuCount)
{
    NvAPI_Status status = nvapi_EnumPhysicalGPUs(nvGPUHandle, pGpuCount);
    if(status != NVAPI_OK)
        qDebug("NvAPI_EnumPhysicalGPUs() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GetInterfaceVersionString(NvAPI_ShortString szDesc)
{
    NvAPI_Status status = nvapi_GetInterfaceVersionString(szDesc);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GetInterfaceVersionString() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GetDisplayDriverVersion(NvDisplayHandle hNvDisplay, NV_DISPLAY_DRIVER_VERSION* pVersion)
{
    pVersion->version = NV_DISPLAY_DRIVER_VERSION_VER;
    NvAPI_Status status = nvapi_GetDisplayDriverVersion(hNvDisplay, pVersion);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GetDisplayDriverVersion() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GetPhysicalGPUsFromDisplay(NvDisplayHandle hNvDisp, NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32* pGpuCount)
{
    NvAPI_Status status = nvapi_GetPhysicalGPUsFromDisplay(hNvDisp, nvGPUHandle, pGpuCount);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GetPhysicalGPUsFromDisplay() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GetPhysicalGPUFromGPUID(NvU32 gpuId, NvPhysicalGpuHandle* nvGPUHandle)
{
    NvAPI_Status status = nvapi_GetPhysicalGPUFromGPUID(gpuId, nvGPUHandle);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GetGetPhysicalGPUFromGPUID() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GetAssociatedNvidiaDisplayHandle(const char* szDisplayName, NvDisplayHandle* pNvDispHandle)
{
    NvAPI_Status status = nvapi_GetAssociatedNvidiaDisplayHandle(szDisplayName, pNvDispHandle);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GetAssociatedNvidiaDisplayHandle() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GetAssociatedDisplayOutputId(NvDisplayHandle hNvDisplay, NvU32* pOutputId)
{
    NvAPI_Status  status = nvapi_GetAssociatedDisplayOutputId(hNvDisplay, pOutputId);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GetAssociatedDisplayOutputId() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GetGPUIDFromPhysicalGPU(NvPhysicalGpuHandle hPhysicalGpu, NvU32* gpuId)
{
    NvAPI_Status status = nvapi_GetGPUIDFromPhysicalGPU(hPhysicalGpu, gpuId);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GetGPUIDFromPhysicalGPU() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GPU_GetThermalSettings(NvPhysicalGpuHandle hPhysicalGpu, NvU32 sensorIndex, NV_GPU_THERMAL_SETTINGS* pThermalSettings)
{
    pThermalSettings->version = NV_GPU_THERMAL_SETTINGS_VER;
    NvAPI_Status status = nvapi_GPU_GetThermalSettings(hPhysicalGpu, sensorIndex, pThermalSettings);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetThermalSettings() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GPU_GetFullName(NvPhysicalGpuHandle hPhysicalGpu, NvAPI_ShortString szName)
{
    NvAPI_Status status = nvapi_GPU_GetFullName(hPhysicalGpu, szName);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetFullName() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GPU_GetEDID(NvPhysicalGpuHandle hPhysicalGpu, NvU32 displayOutputId, NV_EDID* pEDID)
{
    NvAPI_Status status = nvapi_GPU_GetEDID(hPhysicalGpu, displayOutputId, pEDID);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetEDID() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GPU_GetTachReading(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pValue)
{
    NvAPI_Status status = nvapi_GPU_GetTachReading(hPhysicalGpu, pValue);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetTachReading() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPI::GPU_GetAllClocks(__in NvPhysicalGpuHandle hPhysicalGpu, __inout NV_GPU_CLOCKS* pClocks)
{
    pClocks->version = NV_GPU_CLOCKS_VER;
    NvAPI_Status status = nvapi_GPU_GetAllClocks(hPhysicalGpu, pClocks);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetAllClocks() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPI::GPU_GetAllClockFrequencies(__in NvPhysicalGpuHandle hPhysicalGpu, __inout NV_GPU_CLOCK_FREQUENCIES* pClkFreqs)
{
    pClkFreqs->version = NV_GPU_CLOCK_FREQUENCIES_VER;
    NvAPI_Status status = nvapi_GPU_GetAllClockFrequencies(hPhysicalGpu, pClkFreqs);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetAllClockFrequencies() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPI::GPU_GetDynamicPstatesInfoEx(__in NvPhysicalGpuHandle hPhysicalGpu, __inout NV_GPU_DYNAMIC_PSTATES_INFO_EX* pDynamicPstatesInfoEx)
{
    pDynamicPstatesInfoEx->version = NV_GPU_PERF_PSTATES20_INFO_VER;
    NvAPI_Status status = nvapi_GPU_GetDynamicPstatesInfoEx(hPhysicalGpu, pDynamicPstatesInfoEx);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetDynamicPstatesInfoEx() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GPU_GetMemoryInfo(NvPhysicalGpuHandle hPhysicalGpu, NV_DISPLAY_DRIVER_MEMORY_INFO* pMemoryInfo)
{
    pMemoryInfo->version = NV_DISPLAY_DRIVER_MEMORY_INFO_VER;
    NvAPI_Status status = nvapi_GPU_GetMemoryInfo(hPhysicalGpu, pMemoryInfo);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetMemoryInfo() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GPU_GetPCIIdentifiers(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pDeviceId, NvU32* pSubSystemId, NvU32* pRevisionId, NvU32* pExtDeviceId)
{
    NvAPI_Status status = nvapi_GPU_GetPCIIdentifiers(hPhysicalGpu, pDeviceId, pSubSystemId, pRevisionId, pExtDeviceId);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetPCIIdentifiers() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GPU_GetUsages(NvPhysicalGpuHandle hPhysicalGpu, NV_GPU_USAGES* pGpuUsages)
{
    pGpuUsages->version = NV_GPU_USAGES_VER;
    NvAPI_Status status = nvapi_GPU_GetUsages(hPhysicalGpu, pGpuUsages);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetUsages() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPI::GPU_GetCoolerSettings(__in NvPhysicalGpuHandle hPhysicalGpu, __in NvU32 coolerIndex, __inout NV_GPU_COOLER_SETTINGS* pCoolerSettings)
{
    pCoolerSettings->version = NV_GPU_COOLER_SETTINGS_VER;
    NvAPI_Status status = nvapi_GPU_GetCoolerSettings(hPhysicalGpu, coolerIndex, pCoolerSettings);
    if(status != NVAPI_OK)
        qDebug("%s failed with status %d", __PRETTY_FUNCTION__, status);
    return status;
}

NvAPI_Status QNvAPIGlue::GPU_SetCoolerLevels(NvPhysicalGpuHandle hPhysicalGpu, NvU32 coolerIndex, NV_GPU_COOLER_LEVELS* pCoolerLevels)
{
    pCoolerLevels->version = NV_GPU_COOLER_LEVELS_VER;
    NvAPI_Status status = nvapi_GPU_SetCoolerLevels(hPhysicalGpu, coolerIndex, pCoolerLevels);
    if(status != NVAPI_OK)
        qDebug("NvAPI_GPU_SetCoolerLevels() failed with status %d", status);
    return status;
}

NvAPI_Status QNvAPIGlue::GPU_GetClientFanCoolersInfo(NvPhysicalGpuHandle hPhysicalGpu, NV_GPU_FAN_COOLERS_INFO *pCoolersInfo)
{
    pCoolersInfo->version = NV_GPU_FAN_COOLERS_INFO_VER;
    NvAPI_Status status = nvapi_GPU_GetClientFanCoolersInfo(hPhysicalGpu, pCoolersInfo);
    if(status != NVAPI_OK)
        qDebug("%s failed with status %d", __PRETTY_FUNCTION__, status);
    return status;
}

NvAPI_Status QNvAPIGlue::GPU_GetClientFanCoolersStatus(NvPhysicalGpuHandle hPhysicalGpu, NV_GPU_FAN_COOLERS_STATUS *pCoolersStatus)
{
    pCoolersStatus->version = NV_GPU_FAN_COOLERS_STATUS_VER;
    NvAPI_Status status = nvapi_GPU_GetClientFanCoolersStatus(hPhysicalGpu, pCoolersStatus);
    if(status != NVAPI_OK)
        qDebug("%s failed with status %d", __PRETTY_FUNCTION__, status);
    return status;
}

NvAPI_Status QNvAPIGlue::GPU_GetClientFanCoolersControl(NvPhysicalGpuHandle hPhysicalGpu, NV_GPU_FAN_COOLERS_CONTROL *pCoolersControl)
{
    pCoolersControl->version = NV_GPU_FAN_COOLERS_CONTROL_VER;
    NvAPI_Status status = nvapi_GPU_GetClientFanCoolersControl(hPhysicalGpu, pCoolersControl);
    if(status != NVAPI_OK)
        qDebug("%s failed with status %d", __PRETTY_FUNCTION__, status);
    return status;
}

NvAPI_Status QNvAPIGlue::GPU_SetClientFanCoolersControl(NvPhysicalGpuHandle hPhysicalGpu, NV_GPU_FAN_COOLERS_CONTROL *pCoolersControl)
{
    pCoolersControl->version = NV_GPU_FAN_COOLERS_CONTROL_VER;
    NvAPI_Status status = nvapi_GPU_SetClientFanCoolersControl(hPhysicalGpu, pCoolersControl);
    if(status != NVAPI_OK)
        qDebug("%s failed with status %d", __PRETTY_FUNCTION__, status);
    return status;
}

bool QNvAPIGlue::isAvailable(void)
{
    return  nvapi_QueryInterface &&
            nvapi_Initialize &&
            nvapi_Unload &&
            nvapi_EnumNvidiaDisplayHandle &&
            nvapi_EnumPhysicalGPUs &&
            nvapi_GetInterfaceVersionString &&
            nvapi_GetDisplayDriverVersion &&
            nvapi_GetPhysicalGPUsFromDisplay &&
            nvapi_GetPhysicalGPUFromGPUID &&
            nvapi_GetAssociatedNvidiaDisplayHandle &&
            nvapi_GetAssociatedDisplayOutputId &&
            nvapi_GetGPUIDFromPhysicalGPU &&
            nvapi_GPU_GetThermalSettings &&
            nvapi_GPU_GetFullName &&
            nvapi_GPU_GetEDID &&
            nvapi_GPU_GetTachReading &&
            nvapi_GPU_GetAllClocks &&
            nvapi_GPU_GetAllClockFrequencies &&
            nvapi_GPU_GetDynamicPstatesInfoEx &&
            nvapi_GPU_GetMemoryInfo &&
            nvapi_GPU_GetPCIIdentifiers &&
            nvapi_GPU_GetUsages &&
            nvapi_GPU_GetCoolerSettings &&
            nvapi_GPU_SetCoolerLevels &&
            nvapi_GPU_GetClientFanCoolersInfo &&
            nvapi_GPU_GetClientFanCoolersStatus &&
            nvapi_GPU_GetClientFanCoolersControl &&
            nvapi_GPU_SetClientFanCoolersControl
        ;
}

bool QNvAPIGlue::isRtx(NvPhysicalGpuHandle hPhysicalGpu)
{
    NV_GPU_COOLER_SETTINGS settings;
    settings.version = NV_GPU_COOLER_SETTINGS_VER;
    NvAPI_Status status = nvapi_GPU_GetCoolerSettings(hPhysicalGpu, NV_COOLER_TARGET_ALL, &settings);
    if (status == NVAPI_OK) {
        qDebug("%s succeded with status %d, not an RTX card", __PRETTY_FUNCTION__, status);
        return false;
    } else {
        qDebug("%s failed with status %d, it's an RTX card", __PRETTY_FUNCTION__, status);
        return true;
    }
}
