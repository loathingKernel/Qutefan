#ifndef QUTEFAN_NVCTRL_H
#define QUTEFAN_NVCTRL_H

#include <vector>
#include <X11/Xlib.h>
#include "NVCtrl/NVCtrl.h"
#include "NVCtrl/NVCtrlLib.h"

#include <QtGlobal>

class QuteFanNVCtrl
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

    int gpu_count;
    typedef struct {
        bool status;
        uint32_t handle;
        char* name;
        char* uuid;
        int current_temp;
        int maximum_temp;
        int default_control;
        int current_control;
        int cooler_count;
        std::vector<NvCooler> cooler;
    } NvGPU;
    std::vector<NvGPU> gpu;

    QuteFanNVCtrl();
    ~QuteFanNVCtrl();

    bool available();
    void initialize();

private:
    int GetNvXScreen(Display*);
};

#endif // QUTEFAN_NVCTRL_H
