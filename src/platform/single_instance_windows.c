#include "single_instance.h"
#include <Windows.h>

SingleInstanceResult singleInstanceAcquire(void)
{
    SingleInstanceResult result;

    HANDLE handle = CreateMutexW(NULL, TRUE, L"MY_GAME_MUTEX");
}