#include "single_instance.h"
#include <Windows.h>

static HANDLE instanceMutex = NULL;

SingleInstanceResult singleInstanceAcquire(void)
{
    instanceMutex = CreateMutexW(NULL, FALSE, GAME_INSTANCE_ID_W);
    if (instanceMutex == NULL)
    {
        return SINGLE_INSTANCE_ERROR;
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        singleInstanceRelease();

        return SINGLE_INSTANCE_ALREADY_RUNNING;
    }

    return SINGLE_INSTANCE_ACQUIRED;
}

void singleInstanceRelease(void)
{
    if (instanceMutex != NULL)
    {
        CloseHandle(instanceMutex);
        instanceMutex = NULL;
    }
}