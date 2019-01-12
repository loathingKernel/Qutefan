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

    nvapi->EnumPhysicalGPUs(handle, &num_gpus);
    for(int i = 0; i < num_gpus; i++) {
        gpu[i].handle = handle[i];
        nvapi->GPU_GetFullName(gpu[i].handle, gpu[i].name);
    }
    qDebug("Total number of GPU's = %lu", num_gpus);
}
