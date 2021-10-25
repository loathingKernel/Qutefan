#include "qutefan_nvapi.h"

QuteFanNvAPI::QuteFanNvAPI()
{
    nvapi = new QNvAPI();
}

QuteFanNvAPI::~QuteFanNvAPI()
{
    delete nvapi;
}

bool QuteFanNvAPI::available()
{
    return nvapi->isAvailable();
}

void QuteFanNvAPI::initialize()
{
    NvPhysicalGpuHandle handle[NVAPI_MAX_PHYSICAL_GPUS];

    nvapi->Initialize();

    nvapi->GetInterfaceVersionString(version);
    qDebug("NvAPI version: %s", version);

    nvapi->EnumPhysicalGPUs(handle, &gpu_count);
    for(unsigned long i = 0; i < gpu_count; i++) {
        gpu[i].handle = handle[i];
        nvapi->GPU_GetFullName(gpu[i].handle, gpu[i].name);
    }
    qDebug("Total number of GPUs = %lu", gpu_count);
}
