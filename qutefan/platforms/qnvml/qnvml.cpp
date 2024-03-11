#include <QDebug>

#include "qnvml.h"

ControlNvml::ControlNvml()
{

}

ControlNvml::~ControlNvml()
{
    nvmlShutdown();
}

bool ControlNvml::available()
{
    status = nvmlInit_v2();
    if(status != NVML_SUCCESS) {
        qDebug("Failed to initialize NVML; aborting");
        return false;
    }

    return true;
}

void ControlNvml::initialize()
{
    status = nvmlSystemGetNVMLVersion(version, NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE);
    if(status != NVML_SUCCESS) {
        qDebug("Failed to get NVML Version.");
        return;
    }

    status = nvmlDeviceGetCount_v2(&gpu_count);
    if (status != NVML_SUCCESS) {
        qDebug("Failed to query number of gpus");
        return;
    }

    for(unsigned int i = 0; i < gpu_count; i++) {
        NvGPU gpu = NvGPU();
        status = nvmlDeviceGetHandleByIndex_v2(i, &(gpu.handle));
        gpu.status = nvmlDeviceGetNumFans(gpu.handle, &(gpu.cooler_count));
        for(unsigned int c = 0; c < gpu.cooler_count; ++c) {
            NvCooler cooler = NvCooler();
            cooler.handle = c;
            gpu.cooler.push_back(cooler);
        }
        qDebug("GPU %i: Number of coolers = %i", gpu_count, gpu.cooler_count);
        m_gpu.push_back(gpu);
    }
    qDebug("Number of GPUs = %i", gpu_count);
}

const QString ControlNvml::name(NvGPU *gpu)
{
    char nv_name[NVML_DEVICE_NAME_V2_BUFFER_SIZE];
    gpu->status = nvmlDeviceGetName(gpu->handle, nv_name, NVML_DEVICE_NAME_V2_BUFFER_SIZE);
    QString name(nv_name);
    return name;
}

const QString ControlNvml::uuid(NvGPU *gpu)
{
    char nv_uuid[NVML_DEVICE_UUID_V2_BUFFER_SIZE];
    gpu->status = nvmlDeviceGetUUID(gpu->handle, nv_uuid, NVML_DEVICE_UUID_V2_BUFFER_SIZE);
    QString uuid(nv_uuid);
    return uuid;
}

ControlNvml::NvGPU *ControlNvml::getGpuByIndex(int index)
{
    return &m_gpu[index];
}

int ControlNvml::getCoolerCount(NvGPU *gpu)
{
    return gpu->cooler_count;
}

Control::CoolerLimits ControlNvml::getCoolerLimits(NvGPU *gpu)
{
    CoolerLimits limits = {};
    gpu->status = nvmlDeviceGetMinMaxFanSpeed(
        gpu->handle, &(limits.minimum), &(limits.maximum));
    gpu->status = nvmlDeviceGetFanSpeed(
        gpu->handle, &(limits.current));
    return limits;
}

void ControlNvml::setCoolerManualControl(NvGPU *gpu, bool enable)
{
    if (enable) {
        for (unsigned int c = 0; c < gpu->cooler_count; ++c) {
            unsigned int level;
            gpu->status = nvmlDeviceGetTargetFanSpeed(gpu->handle, gpu->cooler[c].handle, &level);
            gpu->status = nvmlDeviceSetFanSpeed_v2(gpu->handle, gpu->cooler[c].handle, level);
        }
    } else {
        for (unsigned int c = 0; c < gpu->cooler_count; ++c) {
            gpu->status = nvmlDeviceSetDefaultFanSpeed_v2(gpu->handle, gpu->cooler[c].handle);
        }
    }
}

Control::CoolerLevels ControlNvml::getCoolerLevels(NvGPU *gpu)
{
    CoolerLevels levels = {};
    for (unsigned int c = 0; c < gpu->cooler_count; ++c) {
        unsigned int level;
        gpu->status = nvmlDeviceGetFanSpeed_v2(gpu->handle, gpu->cooler[c].handle, &level);
        levels.current.append(level);
    }
    levels.count = gpu->cooler_count;
    return levels;
}

void ControlNvml::setCoolerLevels(NvGPU *gpu, int level)
{
    for (unsigned int c = 0; c < gpu->cooler_count; ++c) {
        gpu->status = nvmlDeviceSetFanSpeed_v2(gpu->handle, gpu->cooler[c].handle, level);
    }
}

void ControlNvml::setPowerLimit(NvGPU *gpu, int limit)
{
    gpu->status = nvmlDeviceSetPowerManagementLimit(gpu->handle, limit);
}

Control::Temperatures ControlNvml::getGpuTemperatures(NvGPU *gpu)
{
    Temperatures temps = {};
    gpu->status = nvmlDeviceGetTemperature(gpu->handle, NVML_TEMPERATURE_GPU, &(temps.gpu));
    return temps;
}

Control::Frequencies ControlNvml::getCurrentClockFrequencies(NvGPU *gpu)
{
    Frequencies freqs = {};
    gpu->status = nvmlDeviceGetClockInfo(gpu->handle, NVML_CLOCK_GRAPHICS, &(freqs.core));
    gpu->status = nvmlDeviceGetClockInfo(gpu->handle, NVML_CLOCK_MEM,      &(freqs.memory));
    gpu->status = nvmlDeviceGetClockInfo(gpu->handle, NVML_CLOCK_SM,       &(freqs.shader));
    gpu->status = nvmlDeviceGetClockInfo(gpu->handle, NVML_CLOCK_VIDEO,    &(freqs.video));
    gpu->status = nvmlDeviceGetClockInfo(gpu->handle, NVML_CLOCK_VIDEO,    &(freqs.video));
    gpu->status = nvmlDeviceGetPowerUsage(gpu->handle, &(freqs.power));
    return freqs;
}

Control::Limits ControlNvml::getPowerLimits(NvGPU *gpu)
{
    Limits limits = {};
    limits.denominator = 1000;
    limits.type = Control::LimitType::CONTROL_LIMIT_WATTAGE;
    gpu->status = nvmlDeviceGetPowerManagementLimitConstraints(gpu->handle, &(limits.minimum), &(limits.maximum));
    gpu->status = nvmlDeviceGetPowerManagementLimit(gpu->handle, &(limits.current));
    return limits;
}

Control::Limits ControlNvml::getCoreClockLimits(NvGPU *gpu)
{
    Limits limits = {};
    limits.denominator = 1000;
    limits.type = Control::LimitType::CONTROL_LIMIT_FREQUENCY;
    // gpu->status = nvmlDeviceGetPowerManagementLimitConstraints(gpu->handle, &(limits.minimum), &(limits.maximum));
    // gpu->status = nvmlDeviceGetPowerManagementLimit(gpu->handle, &(limits.current));
    return limits;
}

Control::Limits ControlNvml::getMemClockLimits(NvGPU *gpu)
{
    Limits limits = {};
    limits.denominator = 1000;
    limits.type = Control::LimitType::CONTROL_LIMIT_FREQUENCY;
    // gpu->status = nvmlDeviceGetPowerManagementLimitConstraints(gpu->handle, &(limits.minimum), &(limits.maximum));
    // gpu->status = nvmlDeviceGetPowerManagementLimit(gpu->handle, &(limits.current));
    return limits;
}
