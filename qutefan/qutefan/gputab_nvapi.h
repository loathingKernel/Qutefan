#ifndef GPUTABNVAPI_H
#define GPUTABNVAPI_H

#include <QLabel>
#include <QDateTime>

#include "doublelabel.h"
#include "gputab.h"
#include "control_nvapi.h"

namespace Ui {
class GpuTabNvAPI;
}

class GpuTabNvAPI : public GpuTab
{
public:
    explicit GpuTabNvAPI(ControlNvAPI*, ControlNvAPI::NvGPU*, QSettings*, QWidget* parent = nullptr);
    ~GpuTabNvAPI();

    void saveGpuSettings();
    void setGPUDefaults();
    void regulateFans();
    void displayFrequencies();

private slots:
    void resetMaximums();
#if USE_CHARTS
    void showChart();
#endif

private:
    ControlNvAPI        *m_api;
    ControlNvAPI::NvGPU *m_gpu;
};

#endif // GPUTABNVAPI_H
