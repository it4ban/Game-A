#pragma once

typedef enum SingleInstanceResult
{
    SINGLE_INSTANCE_ACQUIRED,
    SINGLE_INSTANCE_ALREADY_RUNNING,
    SINGLE_INSTANCE_ERROR
} SingleInstanceResult;

SingleInstanceResult singleInstanceAcquire(void);

void singleInstanceRelease(void);