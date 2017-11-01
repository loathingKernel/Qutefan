#ifndef GPUTAB_H
#define GPUTAB_H

#include <QWidget>

#include "qnvapi.h"
#include "qnvctrl.h"

namespace Ui {
class GpuTab;
}

class GpuTab : public QWidget
{
    Q_OBJECT

public:
    explicit GpuTab(QWidget* parent = 0);
#if defined(Q_OS_WIN)
    explicit GpuTab(QNvAPI*, QNvAPI::NvGPU*, QWidget* parent = 0);
#elif defined(Q_OS_LINUX)
    explicit GpuTab(QNVCtrl*, QNVCtrl::NvGPU*, QWidget* parent = 0);
#endif
    ~GpuTab();

    enum class AccessMode {
        nvapi,
        nvctrl,
        amd
    };

    enum class FanMode {
        Off = 0,
        Quiet,
        Fixed,
        Linear,
        Graph
    };

    void setGPUDefaults();
    void regulateFan();
    void displayStatus();

private:
    GpuTab::FanMode getMode();

    Ui::GpuTab* ui;
    AccessMode mode;
    GpuTab::FanMode lastMode = FanMode::Off;

#if defined(Q_OS_WIN)
    QNvAPI* nvapi;
    QNvAPI::NvGPU* nvgpu;
    NV_GPU_COOLER_LEVELS nvDefaultCoolerLevels;
    NvS32 nvMaxTemp;
    NvS32 nvMaxLevel;
#elif defined(Q_OS_LINUX)
    QNVCtrl* nvapi;
    QNVCtrl::NvGPU* nvgpu;
#endif
};

#endif // GPUTAB_H
