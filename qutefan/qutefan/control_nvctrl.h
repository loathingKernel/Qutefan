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
    Display* dpy;
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
        int maximum_level;
    } NvCooler;

    typedef struct {
        bool status;
        uint32_t handle;
        int cooler_count;
        QVector<NvCooler> cooler;
        char name[64];
        char uuid[64];
        int current_temp;
        int maximum_temp;
        int default_control;
        int current_control;
    } NvGPU;
    QVector<NvGPU> m_gpu;

    ControlNVCtrl();
    ~ControlNVCtrl();

    bool available();
    void initialize();

    const QString name(NvGPU*);
    const QString uuid(NvGPU*);

    NvGPU *getGpuByIndex(int index);
    int  getCoolerCount(NvGPU*);
    void setCoolerManualControl(NvGPU*, bool);
    void setCoolerLevel(NvGPU*, int);

    CoolerLevels getCoolerLevels(NvGPU*);
    CoolerLimits getCoolerLimits(NvGPU*);
    Temperature  getGpuTemperatures(NvGPU*);
    Frequency    getCurrentClockFrequencies(NvGPU*);

private:
    int getNvXScreen(Display*);
};

#endif // CONTROL_NVCTRL_H
