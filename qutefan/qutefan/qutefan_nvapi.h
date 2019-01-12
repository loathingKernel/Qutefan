#ifndef QUTEFAN_NVAPI_H
#define QUTEFAN_NVAPI_H

#include "qnvapi.h"

class QuteFanNvAPI
{
public:
    NvAPI_Status status = NVAPI_OK;
    NvAPI_ShortString version = {};

    NV_DISPLAY_DRIVER_VERSION driver_version = {};

    NvU32 display_count = 0;
    NvDisplayHandle display_handle[NVAPI_MAX_PHYSICAL_GPUS*NVAPI_MAX_DISPLAY_HEADS] = {};

    NvU32 gpu_count = 0;
    typedef struct {
        NvAPI_Status status;
        NvPhysicalGpuHandle handle;
        NvAPI_ShortString name;
        NvU32 coolerTach;
        NV_GPU_THERMAL_SETTINGS thermalSettings;
        NV_GPU_COOLER_SETTINGS coolerSettings;
        NV_GPU_COOLER_LEVELS coolerLevels;
        NV_GPU_CLOCKS clocks;
    } NvGPU;
    NvGPU gpu[NVAPI_MAX_PHYSICAL_GPUS] = {};

    QNvAPI* nvapi;

    QuteFanNvAPI();
    ~QuteFanNvAPI();

    bool available();
    void initialize();
};

#endif // QUTEFAN_NVAPI_H
