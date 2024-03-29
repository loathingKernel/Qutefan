#pragma once

#include <QtGlobal>
#include <QVector>

#include "nvapi_glue.h"
#include "control.h"

class ControlNvAPI : public Control
{
public:
    NvDisplayHandle display_handle[NVAPI_MAX_PHYSICAL_GPUS*NVAPI_MAX_DISPLAY_HEADS] = {};
    NvU32 display_count = 0;

    NvAPI_Status status = NVAPI_OK;
    NvAPI_ShortString version = {};
    NV_DISPLAY_DRIVER_VERSION driver_version = {};

    long unsigned int gpu_count = 0;
    typedef struct _NvGPU : NvGPUBase {
        NvAPI_Status status;
        NvPhysicalGpuHandle handle;
        bool rtx;
    } NvGPU;
    QVector<NvGPU> m_gpu;

    QNvAPIGlue *m_nvapi;

    ControlNvAPI();
    ~ControlNvAPI();

    bool available();
    void initialize();

    const QString name(NvGPU *);
    const QString uuid(NvGPU *);

    NvGPU *getGpuByIndex(int index);
    int  getCoolerCount(NvGPU *);
    void setCoolerManualControl(NvGPU *, bool);
    void setCoolerLevels(NvGPU *, int);

    CoolerLimits getCoolerLimits(NvGPU *);
    CoolerLevels getCoolerLevels(NvGPU *);
    Temperatures getGpuTemperatures(NvGPU *);
    Frequencies  getCurrentClockFrequencies(NvGPU *);
};

