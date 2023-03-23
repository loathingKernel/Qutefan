#include "control_nvapi.h"

ControlNvAPI::ControlNvAPI()
{
    m_nvapi = new QNvAPI();
}

ControlNvAPI::~ControlNvAPI()
{
    delete m_nvapi;
}

bool ControlNvAPI::available()
{
    return m_nvapi->isAvailable();
}

void ControlNvAPI::initialize()
{
    NvPhysicalGpuHandle handle[NVAPI_MAX_PHYSICAL_GPUS];

    m_nvapi->Initialize();

    m_nvapi->GetInterfaceVersionString(version);
    qDebug("NvAPI version: %s", version);

    m_nvapi->EnumPhysicalGPUs(handle, &gpu_count);
    for(unsigned long i = 0; i < gpu_count; i++) {
        NvGPU gpu = NvGPU();
        gpu.handle = handle[i];
        gpu.rtx = m_nvapi->isRtx(gpu.handle);
        if (gpu.rtx) {
            NV_GPU_FAN_COOLERS_INFO info = {};
            gpu.status = m_nvapi->GPU_GetClientFanCoolersInfo(gpu.handle, &info);
            gpu.cooler_count = info.count;
        } else {
            NV_GPU_COOLER_SETTINGS settings = {};
            gpu.status = m_nvapi->GPU_GetCoolerSettings(gpu.handle, NV_COOLER_TARGET_ALL, &settings);
            gpu.cooler_count = settings.count;
        }
        qDebug("Number of coolers = %u", gpu.cooler_count);
        m_gpu.push_back(gpu);
    }
    qDebug("Number of GPUs = %lu", gpu_count);
}

const QString ControlNvAPI::name(NvGPU *gpu)
{
    NvAPI_ShortString name;
    gpu->status = m_nvapi->GPU_GetFullName(gpu->handle, name);
    return QString(name);
}

const QString ControlNvAPI::uuid(NvGPU *gpu)
{
    NvU32 device_id, sub_system_id, revision_id, ext_device_id;
    gpu->status = m_nvapi->GPU_GetPCIIdentifiers(gpu->handle, &device_id, &sub_system_id, &revision_id, &ext_device_id);
    QStringList id_list = {
        QStringLiteral("%1").arg(device_id, 8, 16, QLatin1Char('0')),
        QStringLiteral("%1").arg(sub_system_id, 8, 16, QLatin1Char('0')),
        QStringLiteral("%1").arg(revision_id, 8, 16, QLatin1Char('0')),
        QStringLiteral("%1").arg(ext_device_id, 8, 16, QLatin1Char('0'))
    };
    return id_list.join("-");
}

ControlNvAPI::NvGPU *ControlNvAPI::getGpuByIndex(int index)
{
    return &m_gpu[index];
}

int ControlNvAPI::getCoolerCount(NvGPU *gpu)
{
    return gpu->cooler_count;
}

Control::CoolerLimits ControlNvAPI::getCoolerLimits(NvGPU *gpu)
{
    CoolerLimits limits = {};
    if (gpu->rtx) {
        NV_GPU_FAN_COOLERS_STATUS status = {};
        gpu->status = m_nvapi->GPU_GetClientFanCoolersStatus(gpu->handle, &status);
        limits.minimum = status.cooler[0].minimum_level;
        limits.maximum = status.cooler[0].maximum_level;
        limits.current = status.cooler[0].current_level;
    } else {
        NV_GPU_COOLER_SETTINGS settings = {};
        gpu->status = m_nvapi->GPU_GetCoolerSettings(gpu->handle, NV_COOLER_TARGET_ALL, &settings);
        limits.minimum = settings.cooler[0].default_level_min;
        limits.maximum = settings.cooler[0].default_level_max;
        limits.current = settings.cooler[0].current_level;
    }
    return limits;
}

void ControlNvAPI::setCoolerManualControl(NvGPU * gpu, bool enable)
{
    if (gpu->rtx) {
        NV_GPU_FAN_COOLERS_CONTROL control;
        gpu->status = m_nvapi->GPU_GetClientFanCoolersControl(gpu->handle, &control);
        for (unsigned int c = 0; c < control.count; ++c) {
            control.cooler[c].mode = enable ? NV_COOLER_CONTROL_MODE_MANUAL : NV_COOLER_CONTROL_MODE_AUTO;
        }
        gpu->status = m_nvapi->GPU_SetClientFanCoolersControl(gpu->handle, &control);
    } else {
        NV_GPU_COOLER_LEVELS new_level;
        for (int c = 0; c < getCoolerCount(gpu); ++c) {
            new_level.cooler[c].policy = enable ? NV_COOLER_POLICY_MANUAL : NV_COOLER_POLICY_DEFAULT;
        }
        gpu->status = m_nvapi->GPU_SetCoolerLevels(gpu->handle, NV_COOLER_TARGET_ALL, &new_level);
    }
}

Control::CoolerLevels ControlNvAPI::getCoolerLevels(NvGPU *gpu)
{
    CoolerLevels levels;
    if (gpu->rtx) {
        NV_GPU_FAN_COOLERS_STATUS status = {};
        gpu->status = m_nvapi->GPU_GetClientFanCoolersStatus(gpu->handle, &status);
        for (unsigned int c = 0; c < status.count; ++c)
            levels.current.append(status.cooler[c].current_level);
        levels.count = status.count;
    } else {
        NV_GPU_COOLER_SETTINGS settings = {};
        gpu->status = m_nvapi->GPU_GetCoolerSettings(gpu->handle, NV_COOLER_TARGET_ALL, &settings);
        for (unsigned int c = 0; c < settings.count; ++c)
            levels.current.append(settings.cooler[c].current_level);
        levels.count = settings.count;
    }
    return levels;
}

void ControlNvAPI::setCoolerLevels(NvGPU *gpu, int level)
{
    if (gpu->rtx) {
        NV_GPU_FAN_COOLERS_CONTROL control;
        gpu->status = m_nvapi->GPU_GetClientFanCoolersControl(gpu->handle, &control);
        for (unsigned int c = 0; c < control.count; ++c) {
            control.cooler[c].level = level;
        }
        gpu->status = m_nvapi->GPU_SetClientFanCoolersControl(gpu->handle, &control);
    } else {
        NV_GPU_COOLER_LEVELS new_level;
        for (int c = 0; c < getCoolerCount(gpu); ++c) {
            new_level.cooler[c].level = level;
        }
        gpu->status = m_nvapi->GPU_SetCoolerLevels(gpu->handle, NV_COOLER_TARGET_ALL, &new_level);
    }
}

Control::Temperatures ControlNvAPI::getGpuTemperatures(NvGPU *gpu)
{
    NV_GPU_THERMAL_SETTINGS nv_thermal_settings = {};
    gpu->status = m_nvapi->GPU_GetThermalSettings(gpu->handle, NVAPI_THERMAL_TARGET_ALL, &nv_thermal_settings);

    Temperatures temps;
    for (unsigned int t = 0; t < nv_thermal_settings.count; ++t) {
        if (nv_thermal_settings.sensor[t].target == NVAPI_THERMAL_TARGET_GPU) {
            temps.gpu = nv_thermal_settings.sensor[t].currentTemp;
        } else if (nv_thermal_settings.sensor[t].target == NVAPI_THERMAL_TARGET_MEMORY) {
           temps.memory = nv_thermal_settings.sensor[t].currentTemp;
        } else if (nv_thermal_settings.sensor[t].target == NVAPI_THERMAL_TARGET_POWER_SUPPLY) {
            temps.power_supply = nv_thermal_settings.sensor[t].currentTemp;
        } else if (nv_thermal_settings.sensor[t].target == NVAPI_THERMAL_TARGET_BOARD) {
            temps.board = nv_thermal_settings.sensor[t].currentTemp;
        } else {
            qDebug("%s uknown target %d", __PRETTY_FUNCTION__, nv_thermal_settings.sensor[t].target);
        }
    }
    return temps;
}

Control::Frequencies ControlNvAPI::getCurrentClockFrequencies(NvGPU *gpu)
{
    Frequencies freqs = {};

    if (gpu->rtx) {
        NV_GPU_CLOCK_FREQUENCIES nv_clock_frequencies = {};
        gpu->status = m_nvapi->GPU_GetAllClockFrequencies(gpu->handle, &nv_clock_frequencies);
        freqs.core   = nv_clock_frequencies.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency/1000;
        freqs.memory = nv_clock_frequencies.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency/1000;
        freqs.shader = nv_clock_frequencies.domain[NVAPI_GPU_PUBLIC_CLOCK_PROCESSOR].frequency/1000;
    } else {
        NV_GPU_CLOCKS nv_clocks = {};
        gpu->status = m_nvapi->GPU_GetAllClocks(gpu->handle, &nv_clocks);
        if (freqs.core == 0)
            freqs.core   = nv_clocks.clock[0] / 1000.0f;
        if (freqs.memory == 0)
            freqs.memory = nv_clocks.clock[1] / 1000.0f;
        if (freqs.shader == 0)
            freqs.shader = nv_clocks.clock[2] / 1000.0f;

        if(nv_clocks.clock[30] != 0) {
            if (freqs.core == 0)
                freqs.core   = nv_clocks.clock[30] / 2000.0f;
            if (freqs.shader == 0)
                freqs.shader = nv_clocks.clock[30] / 1000.0f;
        }
    }
    return freqs;
}
