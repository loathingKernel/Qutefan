#include "qnvapi.h"

QMutex* nvapi_lock = new QMutex(QMutex::Recursive);

QNvAPI::QNvAPI()
    : QLibrary("nvapi"),
      nvapi_QueryInterface(NULL),
      nvapi_Initialize(NULL),
      nvapi_EnumNvidiaDisplayHandle(NULL),
      nvapi_EnumPhysicalGPUs(NULL),
      nvapi_GetInterfaceVersionString(NULL),
      nvapi_GetDisplayDriverVersion(NULL),
      nvapi_GetPhysicalGPUsFromDisplay(NULL),
      nvapi_GetAssociatedNvidiaDisplayHandle(NULL),
      nvapi_GetAssociatedDisplayOutputId(NULL),
      nvapi_GPU_GetThermalSettings(NULL),
      nvapi_GPU_GetFullName(NULL),
      nvapi_GPU_GetEDID(NULL),
      nvapi_GPU_GetTachReading(NULL),
      nvapi_GPU_GetAllClockFrequencies(NULL),
      nvapi_GPU_GetPstates20(NULL),
      nvapi_GPU_GetMemoryInfo(NULL),
      nvapi_GPU_GetPCIIdentifiers(NULL),
      nvapi_GPU_GetUsages(NULL),
      nvapi_GPU_GetCoolerSettings(NULL),
      nvapi_GPU_SetCoolerLevels(NULL),
      nvapi_initialized(false)
{
    QMutexLocker locker(nvapi_lock);

    nvapi_QueryInterface = (QNVAPI_QUERYINTERFACE)resolve("nvapi_QueryInterface");

    if(!nvapi_QueryInterface) {
        status = NVAPI_LIBRARY_NOT_FOUND;
        qDebug("QNvAPI failed to initialize NvAPI_QueryInterface()");
    } else {
        nvapi_Initialize                        = (QNVAPI_INITIALIZE)                       nvapi_QueryInterface(0x0150E828);

        nvapi_EnumNvidiaDisplayHandle           = (QNVAPI_ENUMNVIDIADISPLAYHANDLE)          nvapi_QueryInterface(0x9ABDD40D);
        nvapi_EnumPhysicalGPUs                  = (QNVAPI_ENUMPHYSICALGPUS)                 nvapi_QueryInterface(0xE5AC921F);

        nvapi_GetInterfaceVersionString         = (QNVAPI_GETINTERFACEVERSIONSTRING)        nvapi_QueryInterface(0x01053FA5);
        nvapi_GetDisplayDriverVersion           = (QNVAPI_GETDISPLAYDRIVERVERSION)          nvapi_QueryInterface(0xF951A4D1);
        nvapi_GetPhysicalGPUsFromDisplay        = (QNVAPI_GETPHYSICALGPUSFROMDISPLAY)       nvapi_QueryInterface(0x34EF9506);
        nvapi_GetAssociatedNvidiaDisplayHandle  = (QNVAPI_GETASSOCIATEDNVIDIADISPLAYHANDLE) nvapi_QueryInterface(0x35C29134);
        nvapi_GetAssociatedDisplayOutputId      = (QNVAPI_GETASSOCIATEDDISPLAYOUTPUTID)     nvapi_QueryInterface(0xD995937E);

        nvapi_GPU_GetThermalSettings            = (QNVAPI_GPU_GETTHERMALSETTINGS)           nvapi_QueryInterface(0xE3640A56);
        nvapi_GPU_GetFullName                   = (QNVAPI_GPU_GETFULLNAME)                  nvapi_QueryInterface(0xCEEE8E9F);
        nvapi_GPU_GetEDID                       = (QNVAPI_GPU_GETEDID)                      nvapi_QueryInterface(0x37D32E69);
        nvapi_GPU_GetTachReading                = (QNVAPI_GPU_GETTACHREADING)               nvapi_QueryInterface(0x5F608315);
        nvapi_GPU_GetAllClockFrequencies        = (QNVAPI_GPU_GETALLCLOCKFREQUENCIES)       nvapi_QueryInterface(0x1BD69F49);
        nvapi_GPU_GetPstates20                  = (QNVAPI_GPU_GETPSTATES20)                 nvapi_QueryInterface(0x60DED2ED);
        nvapi_GPU_GetMemoryInfo                 = (QNVAPI_GPU_GETMEMORYINFO)                nvapi_QueryInterface(0x774AA982);
        nvapi_GPU_GetPCIIdentifiers             = (QNVAPI_GPU_GETPCIIDENTIFIERS)            nvapi_QueryInterface(0x2DDFB66E);

        nvapi_GPU_GetUsages                     = (QNVAPI_GPU_GETUSAGES)                    nvapi_QueryInterface(0x189A1FDF);
        nvapi_GPU_GetCoolerSettings             = (QNVAPI_GPU_GETCOOLERSETTINGS)            nvapi_QueryInterface(0xDA141340);

        nvapi_GPU_SetCoolerLevels               = (QNVAPI_GPU_SETCOOLERLEVELS)              nvapi_QueryInterface(0x891FA0AE);
    }
}

NvAPI_Status QNvAPI::Initialize(void)
{
    nvapi_status = nvapi_Initialize();
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_Initialize() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::EnumNvidiaDisplayHandle(NvU32 thisEnum, NvDisplayHandle* pNvDispHandle)
{
    nvapi_status = nvapi_EnumNvidiaDisplayHandle(thisEnum, pNvDispHandle);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_EnumNvidiaDisplayHandle() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::EnumPhysicalGPUs(NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32* pGpuCount)
{
    nvapi_status = nvapi_EnumPhysicalGPUs(nvGPUHandle, pGpuCount);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_EnumPhysicalGPUs() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GetInterfaceVersionString(NvAPI_ShortString szDesc)
{
    nvapi_status = nvapi_GetInterfaceVersionString(szDesc);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GetInterfaceVersionString() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GetDisplayDriverVersion(NvDisplayHandle hNvDisplay, NV_DISPLAY_DRIVER_VERSION* pVersion)
{
    pVersion->version = NV_DISPLAY_DRIVER_VERSION_VER;
    nvapi_status = nvapi_GetDisplayDriverVersion(hNvDisplay, pVersion);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GetDisplayDriverVersion() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GetPhysicalGPUsFromDisplay(NvDisplayHandle hNvDisp, NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32* pGpuCount)
{
    nvapi_status = nvapi_GetPhysicalGPUsFromDisplay(hNvDisp, nvGPUHandle, pGpuCount);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GetPhysicalGPUsFromDisplay() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GetAssociatedNvidiaDisplayHandle(const char* szDisplayName, NvDisplayHandle* pNvDispHandle)
{
    nvapi_status = nvapi_GetAssociatedNvidiaDisplayHandle(szDisplayName, pNvDispHandle);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GetAssociatedNvidiaDisplayHandle() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GetAssociatedDisplayOutputId(NvDisplayHandle hNvDisplay, NvU32* pOutputId)
{
    nvapi_status = nvapi_GetAssociatedDisplayOutputId(hNvDisplay, pOutputId);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GetAssociatedDisplayOutputId() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GPU_GetThermalSettings(NvPhysicalGpuHandle hPhysicalGpu, NvU32 sensorIndex, NV_GPU_THERMAL_SETTINGS* pThermalSettings)
{
    pThermalSettings->version = NV_GPU_THERMAL_SETTINGS_VER;
    nvapi_status = nvapi_GPU_GetThermalSettings(hPhysicalGpu, sensorIndex, pThermalSettings);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetThermalSettings() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GPU_GetFullName(NvPhysicalGpuHandle hPhysicalGpu, NvAPI_ShortString szName)
{
    nvapi_status = nvapi_GPU_GetFullName(hPhysicalGpu, szName);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetFullName() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GPU_GetEDID(NvPhysicalGpuHandle hPhysicalGpu, NvU32 displayOutputId, NV_EDID* pEDID)
{
    nvapi_status = nvapi_GPU_GetEDID(hPhysicalGpu, displayOutputId, pEDID);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetEDID() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GPU_GetTachReading(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pValue)
{
    nvapi_status = nvapi_GPU_GetTachReading(hPhysicalGpu, pValue);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetTachReading() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GPU_GetAllClockFrequencies(__in NvPhysicalGpuHandle hPhysicalGpu, __inout NV_GPU_CLOCK_FREQUENCIES* pClkFreqs)
{
    pClkFreqs->version = NV_GPU_CLOCK_FREQUENCIES_VER;
    nvapi_status = nvapi_GPU_GetAllClockFrequencies(hPhysicalGpu, pClkFreqs);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetAllClockFrequencies() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GPU_GetPstates20(__in NvPhysicalGpuHandle hPhysicalGpu, __inout NV_GPU_PERF_PSTATES20_INFO* pPstatesInfo)
{
    pPstatesInfo->version = NV_GPU_PERF_PSTATES20_INFO_VER;
    nvapi_status = nvapi_GPU_GetPstates20(hPhysicalGpu, pPstatesInfo);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetPstates20() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GPU_GetMemoryInfo(NvPhysicalGpuHandle hPhysicalGpu, NV_DISPLAY_DRIVER_MEMORY_INFO* pMemoryInfo)
{
    pMemoryInfo->version = NV_DISPLAY_DRIVER_MEMORY_INFO_VER;
    nvapi_status = nvapi_GPU_GetMemoryInfo(hPhysicalGpu, pMemoryInfo);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetMemoryInfo() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GPU_GetPCIIdentifiers(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pDeviceId, NvU32* pSubSystemId, NvU32* pRevisionId, NvU32* pExtDeviceId)
{
    nvapi_status = nvapi_GPU_GetPCIIdentifiers(hPhysicalGpu, pDeviceId, pSubSystemId, pRevisionId, pExtDeviceId);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetPCIIdentifiers() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GPU_GetUsages(NvPhysicalGpuHandle hPhysicalGpu, NV_GPU_USAGES* pGpuUsages)
{
    pGpuUsages->version = NV_GPU_USAGES_VER;
    nvapi_status = nvapi_GPU_GetUsages(hPhysicalGpu, pGpuUsages);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetUsages() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GPU_GetCoolerSettings(NvPhysicalGpuHandle hPhysicalGpu, NvU32 coolerIndex, NV_GPU_COOLER_SETTINGS* pCoolerSettings)
{
    pCoolerSettings->version = NV_GPU_COOLER_SETTINGS_VER;
    nvapi_status = nvapi_GPU_GetCoolerSettings(hPhysicalGpu, coolerIndex, pCoolerSettings);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetCoolerSettings() failed with status %d", nvapi_status);
    return nvapi_status;
}

NvAPI_Status QNvAPI::GPU_SetCoolerLevels(NvPhysicalGpuHandle hPhysicalGpu, NvU32 coolerIndex, NV_GPU_COOLER_LEVELS* pCoolerLevels)
{
    pCoolerLevels->version = NV_GPU_COOLER_LEVELS_VER;
    nvapi_status = nvapi_GPU_SetCoolerLevels(hPhysicalGpu, coolerIndex, pCoolerLevels);
    if(nvapi_status != NVAPI_OK)
        qDebug("NvAPI_GPU_GetCoolerLevels() failed with status %d", nvapi_status);
    return nvapi_status;
}

bool QNvAPI::Available(void)
{
    return  nvapi_QueryInterface &&
            nvapi_Initialize &&
            nvapi_EnumNvidiaDisplayHandle &&
            nvapi_EnumPhysicalGPUs &&
            nvapi_GetInterfaceVersionString &&
            nvapi_GetDisplayDriverVersion &&
            nvapi_GetPhysicalGPUsFromDisplay &&
            nvapi_GetAssociatedNvidiaDisplayHandle &&
            nvapi_GetAssociatedDisplayOutputId &&
            nvapi_GPU_GetThermalSettings &&
            nvapi_GPU_GetFullName &&
            nvapi_GPU_GetEDID &&
            nvapi_GPU_GetTachReading &&
            nvapi_GPU_GetAllClockFrequencies &&
            nvapi_GPU_GetPstates20 &&
            nvapi_GPU_GetMemoryInfo &&
            nvapi_GPU_GetPCIIdentifiers &&
            nvapi_GPU_GetUsages &&
            nvapi_GPU_GetCoolerSettings &&
            nvapi_GPU_SetCoolerLevels;
}
