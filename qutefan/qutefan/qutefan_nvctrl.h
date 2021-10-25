#ifndef QUTEFAN_NVCTRL_H
#define QUTEFAN_NVCTRL_H

#include <QtGlobal>
#include <X11/Xlib.h>
#include "NVCtrl/NVCtrl.h"
#include "NVCtrl/NVCtrlLib.h"

#define NVCTRL_MAX_PHYSICAL_GPUS 64

class QuteFanNVCtrl
{
public:
    Display* dpy;
    int screen;

    bool status;
    int gpu_count;

    typedef struct {
        int major;
        int minor;
    } NvVersion;
    NvVersion version;

    typedef struct {
        bool status;
        int handle;
        char* name;
        char* uuid;
    } NvGPU;
    NvGPU gpu[NVCTRL_MAX_PHYSICAL_GPUS] = {};

    QuteFanNVCtrl();
    ~QuteFanNVCtrl();

    bool available();
    void initialize();

private:
    int GetNvXScreen(Display*);
};

#endif // QUTEFAN_NVCTRL_H
