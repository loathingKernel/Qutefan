#ifndef CONTROL_NVCTRL_H
#define CONTROL_NVCTRL_H

#include <QtGlobal>
#include <QVector>

#include <X11/Xlib.h>
#include <NVCtrl/NVCtrl.h>
#include <NVCtrl/NVCtrlLib.h>

#include "control.h"

class ControlNVCtrl : public Control
{
public:
    Display *dpy;
    int screen;

    bool status;

    typedef struct {
        int major;
        int minor;
    } NvVersion;
    NvVersion version;

    typedef struct {
        uint32_t handle;
        int default_level;
        int current_level;
    } NvCooler;

    int gpu_count = 0;
    typedef struct {
        bool status;
        uint32_t handle;
        int cooler_count;
        QVector<NvCooler> cooler;
        int current_temp;
        int default_control;
        int current_control;
    } NvGPU;
    QVector<NvGPU> m_gpu;

    ControlNVCtrl();
    ~ControlNVCtrl();

    bool available();
    void initialize();

    const QString name(NvGPU *);
    const QString uuid(NvGPU *);

    NvGPU *getGpuByIndex(int index);
    int  getCoolerCount(NvGPU *);
    void setCoolerManualControl(NvGPU *, bool);
    void setCoolerLevels(NvGPU *, int);

    CoolerLevels getCoolerLevels(NvGPU *);
    CoolerLimits getCoolerLimits(NvGPU *);
    Temperatures getGpuTemperatures(NvGPU *);
    Frequencies  getCurrentClockFrequencies(NvGPU *);

private:
    typedef struct _NV_THERMAL_SENSOR_DATA {
        int count;
        int handle[8];
    } NV_THERMAL_SENSOR_DATA;

    typedef struct _NV_COOLER_DATA {
        int count;
        int handle[8];
    } NV_COOLER_DATA;

    typedef struct _NV_CLOCK_FREQS {
        int memory : 16;
        int core : 16;
    } NV_CLOCK_FREQS;

    int getNvXScreen(Display*);
};

#endif // CONTROL_NVCTRL_H
