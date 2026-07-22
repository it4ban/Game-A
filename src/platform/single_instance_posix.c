#include "single_instance.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

static int fileHandle = -1;

SingleInstanceResult singleInstanceAcquire(void)
{
#ifdef __APPLE__

#else
    const char* runtimeDir = getenv("XDG_RUNTIME_DIR");
    if (runtimeDir != NULL && runtimeDir[0] != '\0')
    {
        int bufSize = strlen(runtimeDir) + strlen(GAME_INSTANCE_ID) + 2;
        char lockPath[bufSize];

        int statusConcat = snprintf(lockPath, bufSize, "%s/%s", runtimeDir, GAME_INSTANCE_ID);
        if (statusConcat < 0 || statusConcat >= bufSize)
        {
            return SINGLE_INSTANCE_ERROR;
        }
    }
    else
    {
        return SINGLE_INSTANCE_ERROR;
    }
#endif

    // fileHandle = open();
    return SINGLE_INSTANCE_ACQUIRED;
}

void singleInstanceRelease(void)
{
}