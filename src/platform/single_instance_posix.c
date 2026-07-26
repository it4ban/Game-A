#include "single_instance.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdbool.h>

static int fileHandle = -1;

SingleInstanceResult singleInstanceAcquire(void)
{
#ifdef __APPLE__
    size_t runtimeDirSize = confstr(_CS_DARWIN_USER_TEMP_DIR, NULL, 0);
    if (runtimeDirSize == 0)
    {
        return SINGLE_INSTANCE_ERROR;
    }

    char* runtimeDir = malloc(runtimeDirSize);
    if (runtimeDir == NULL)
    {
        return SINGLE_INSTANCE_ERROR;
    }

    size_t runtimeWritten = confstr(_CS_DARWIN_USER_TEMP_DIR, runtimeDir, runtimeDirSize);
    if (runtimeWritten == 0 || runtimeWritten > runtimeDirSize || runtimeDir[0] == '\0')
    {
        free(runtimeDir);
        return SINGLE_INSTANCE_ERROR;
    }

    size_t dirLength = strlen(runtimeDir);
    size_t fileLength = strlen(GAME_INSTANCE_ID);

    bool hasSlash = dirLength > 0 && runtimeDir[dirLength - 1] == '/';
    size_t lockPathSize = dirLength + (hasSlash ? 0 : 1) + fileLength + strlen(".lock") + 1;

    char* lockPath = malloc(lockPathSize);
    if (lockPath == NULL)
    {
        free(runtimeDir);
        return SINGLE_INSTANCE_ERROR;
    }

    int lockWritten =
        snprintf(lockPath, lockPathSize, hasSlash ? "%s%s.lock" : "%s/%s.lock", runtimeDir, GAME_INSTANCE_ID);
    if (lockWritten < 0 || (size_t)lockWritten >= lockPathSize)
    {
        free(lockPath);
        free(runtimeDir);
        return SINGLE_INSTANCE_ERROR;
    }

    free(runtimeDir);
    runtimeDir = NULL;
#else
    const char* runtimeDir = getenv("XDG_RUNTIME_DIR");
    if (runtimeDir != NULL && runtimeDir[0] != '\0')
    {
        size_t bufSize = strlen(runtimeDir) + strlen(GAME_INSTANCE_ID) + 2;
        char lockPath[bufSize];

        int statusConcat = snprintf(lockPath, bufSize, "%s/%s", runtimeDir, GAME_INSTANCE_ID);
        if (statusConcat < 0 || (size_t)statusConcat >= bufSize)
        {
            return SINGLE_INSTANCE_ERROR;
        }
    }
    else
    {
        return SINGLE_INSTANCE_ERROR;
    }
#endif

    fileHandle = open(lockPath, O_RDWR | O_CREAT | O_CLOEXEC, 0600);
    free(lockPath);

    if (fileHandle == -1)
    {
        return SINGLE_INSTANCE_ERROR;
    }

    struct flock lock = {0};
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fileHandle, F_SETLK, &lock) == -1)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            singleInstanceRelease();

            return SINGLE_INSTANCE_ALREADY_RUNNING;
        }

        singleInstanceRelease();

        return SINGLE_INSTANCE_ERROR;
    }

    return SINGLE_INSTANCE_ACQUIRED;
}

void singleInstanceRelease(void)
{
    if (fileHandle != -1)
    {
        close(fileHandle);
        fileHandle = -1;
    }
}