#pragma once

#include <QDateTime>

#include "gputab.h"
#include "qnvctrl.h"

namespace Ui {
class GpuTabNVCtrl;
}

class GpuTabNVCtrl : public GpuTab
{
public:
    explicit GpuTabNVCtrl(ControlNVCtrl*, ControlNVCtrl::NvGPU*, QSettings*, QWidget* parent = nullptr);
    ~GpuTabNVCtrl();

    void saveGpuSettings();
    void setGPUDefaults();
    void regulateFans();
    void displayFrequencies();

private slots:
    void applyOverclock() { return; }
    void resetOverclock() { return; }
    void resetMaximums();

private:
    ControlNVCtrl           *m_api;
    ControlNVCtrl::NvGPU    *m_gpu;
};
