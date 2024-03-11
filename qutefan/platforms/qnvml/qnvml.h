#pragma once

#include <QtGlobal>
#include <QVector>

#include <X11/Xlib.h>

#include "control.h"
#include "include/nvml.h"

class ControlNvml : public Control
{
public:
    Display *dpy;
    int screen;

    nvmlReturn_t status;

    char version[NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE] = {};
    char driver_version[NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE] = {};

    typedef struct {
        unsigned int handle;
        unsigned int default_level;
        unsigned int current_level;
    } NvCooler;

    unsigned int gpu_count = 0;
    typedef struct _NvGPU : NvGPUBase {
        nvmlReturn_t status;
        nvmlDevice_t handle;
        QVector<NvCooler> cooler;
    } NvGPU;
    QVector<NvGPU> m_gpu;

    ControlNvml();
    ~ControlNvml();

    bool available();
    void initialize();

    const QString name(NvGPU *);
    const QString uuid(NvGPU *);

    NvGPU *getGpuByIndex(int index);
    void setCoolerManualControl(NvGPU *, bool);
    void setCoolerLevels(NvGPU *, int);
    void setPowerLimit(NvGPU *, int);

    int          getCoolerCount(NvGPU *);
    CoolerLimits getCoolerLimits(NvGPU *);
    CoolerLevels getCoolerLevels(NvGPU *);
    Temperatures getGpuTemperatures(NvGPU *);
    Frequencies  getCurrentClockFrequencies(NvGPU *);
    Limits       getPowerLimits(NvGPU *);
    Limits       getCoreClockLimits(NvGPU *);
    Limits       getMemClockLimits(NvGPU *);
};
