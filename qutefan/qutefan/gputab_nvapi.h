#ifndef GPUTABNVAPI_H
#define GPUTABNVAPI_H

#include "gputab.h"
#include "qutefan_nvapi.h"

namespace Ui {
class GpuTabNvAPI;
}

class GpuTabNvAPI : public GpuTab
{
public:
    explicit GpuTabNvAPI(QuteFanNvAPI*, QuteFanNvAPI::NvGPU*, QSettings*, QWidget* parent = nullptr);
    ~GpuTabNvAPI();

    void saveGpuSettings();
    void setGPUDefaults();
    void regulateFan();
    void displayStatus();

private slots:
    void resetMax();
#if USE_CHARTS
    void showChart();
#endif

private:
    QuteFanNvAPI* api;
    QuteFanNvAPI::NvGPU* gpu;
    QNvAPI* nvapi;

    QString* uuid;

    NV_GPU_COOLER_LEVELS default_cooler_levels;
    NvS32 max_temp;
    NvS32 max_level;
};

#endif // GPUTABNVAPI_H
