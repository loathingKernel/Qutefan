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
    explicit GpuTab(QNvAPI*, QNvAPI::NvGPU*, QWidget* parent = 0);
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

    QNvAPI* nvapi;
    QNvAPI::NvGPU* nvgpu;
    NV_GPU_COOLER_LEVELS nvDefaultCoolerLevels;
    NvS32 nvMaxTemp;
    NvS32 nvMaxFan;
};

#endif // GPUTAB_H
