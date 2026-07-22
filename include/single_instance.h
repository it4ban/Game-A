#pragma once

typedef enum SingleInstanceResult
{
    SINGLE_INSTANCE_ACQUIRED,
    SINGLE_INSTANCE_ALREADY_RUNNING,
    SINGLE_INSTANCE_ERROR
} SingleInstanceResult;

#define GAME_INSTANCE_ID "com.it4ban.gamea"
#define GAME_INSTANCE_ID_W L"com.it4ban.gamea"

SingleInstanceResult singleInstanceAcquire(void);

void singleInstanceRelease(void);