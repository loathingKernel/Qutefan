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
    nvapi->Initialize();

    nvapi->GetInterfaceVersionString(version);
    qDebug("NvAPI version: %s", version);

    NvPhysicalGpuHandle handle[NVAPI_MAX_PHYSICAL_GPUS];
    nvapi->EnumPhysicalGPUs(handle, &gpuCount);
    for(unsigned int i = 0; i < gpuCount; i++)
        gpu[i].handle = handle[i];
    qDebug("Total number of GPU's = %lu", gpuCount);

    for(unsigned int i = 0; i < gpuCount; i++)
        nvapi->GPU_GetFullName(gpu[i].handle, gpu[i].name);
}
