#include "single_instance.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

static int fileHandle = -1;

static bool endsWithSlash(const char* dirPath);

static char* createLockPath(void);

static char* buildLockPath(const char* runtimeDir);

static SingleInstanceResult tryLockFile(int descriptor);

SingleInstanceResult singleInstanceAcquire(void)
{
    if (fileHandle != -1)
    {
        return SINGLE_INSTANCE_ERROR;
    }

    char* lockPath = createLockPath();
    if (lockPath == NULL)
    {
        return SINGLE_INSTANCE_ERROR;
    }

    fileHandle = open(lockPath, O_RDWR | O_CREAT | O_CLOEXEC, 0600);
    free(lockPath);

    if (fileHandle == -1)
    {
        return SINGLE_INSTANCE_ERROR;
    }

    SingleInstanceResult result = tryLockFile(fileHandle);
    if (result != SINGLE_INSTANCE_ACQUIRED)
    {
        singleInstanceRelease();
    }

    return result;
}

static bool endsWithSlash(const char* dirPath)
{
    size_t length = strlen(dirPath);
    return length > 0 && dirPath[length - 1] == '/';
}

static char* createLockPath(void)
{
    char* lockPath = NULL;

#ifdef __APPLE__
    size_t runtimeDirSize = confstr(_CS_DARWIN_USER_TEMP_DIR, NULL, 0);
    if (runtimeDirSize == 0)
    {
        return NULL;
    }

    char* runtimeDir = malloc(runtimeDirSize);
    if (runtimeDir == NULL)
    {
        return NULL;
    }

    size_t runtimeWritten = confstr(_CS_DARWIN_USER_TEMP_DIR, runtimeDir, runtimeDirSize);
    if (runtimeWritten == 0 || runtimeWritten > runtimeDirSize || runtimeDir[0] == '\0')
    {
        free(runtimeDir);
        return NULL;
    }

    lockPath = buildLockPath(runtimeDir);
    free(runtimeDir);
#else
    const char* runtimeDir = getenv("XDG_RUNTIME_DIR");
    lockPath = buildLockPath(runtimeDir);
#endif

    return lockPath;
}

static char* buildLockPath(const char* runtimeDir)
{
    if (runtimeDir == NULL || runtimeDir[0] == '\0')
    {
        return NULL;
    }

    bool runtimeDirHasSlash = endsWithSlash(runtimeDir);

    size_t lockPathSize =
        strlen(runtimeDir) + (runtimeDirHasSlash ? 0 : 1) + strlen(GAME_INSTANCE_ID) + strlen(".lock") + 1;

    char* lockPath = malloc(lockPathSize);
    if (lockPath == NULL)
    {
        return NULL;
    }

    int lockWritten =
        snprintf(lockPath, lockPathSize, runtimeDirHasSlash ? "%s%s.lock" : "%s/%s.lock", runtimeDir, GAME_INSTANCE_ID);
    if (lockWritten < 0 || (size_t)lockWritten >= lockPathSize)
    {
        free(lockPath);
        return NULL;
    }

    return lockPath;
}

static SingleInstanceResult tryLockFile(int descriptor)
{
    struct flock lock = {0};
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(descriptor, F_SETLK, &lock) == -1)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            return SINGLE_INSTANCE_ALREADY_RUNNING;
        }

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
