#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

typedef struct Lightswitch_
{
    int counter;
    sem_t mutex;
}
Lightswitch;

extern sem_t noReaders;
extern sem_t noWriters;
extern Lightswitch readSwitch;
extern Lightswitch writeSwitch;

void wasteTime()
{
    int i, j, x, randNum = rand() % 10000;
    for (i = 0; i < randNum; i++) {
        for (j = 0; j < randNum; j++) {
            x = i * j;
        }
    }
}

Lightswitch initLightswitch()
{
    Lightswitch light;

    light.counter = 0;
    sem_init(&light.mutex, 0, 1);

    return light;
}

void lockLightswitch(Lightswitch *light, sem_t *semaphore)
{
    sem_wait(&light->mutex);

    light->counter++;
    if (light->counter == 1) {
        sem_wait(semaphore);
    }

    sem_post(&light->mutex);
}

void unlockLightswitch(Lightswitch *light, sem_t *semaphore)
{
    sem_wait(&light->mutex);

    light->counter--;
    if (light->counter == 0) {
        sem_post(semaphore);
    }

    sem_post(&light->mutex);
}

void myRead(int threadNum)
{
    printf("%-4d beginning read  ...\n", threadNum);
    wasteTime();
    printf("%-4d complete read   ...\n", threadNum);
}

void myWrite(int threadNum)
{
    printf("%-4d beginning write ...\n", threadNum);
    wasteTime();
    printf("%-4d complete write  ...\n", threadNum);
}

void *reader(void *threadNum)
{
    sem_wait(&noReaders);
    lockLightswitch(&readSwitch, &noWriters);
    sem_post(&noReaders);

    myRead(*(int*)threadNum);

    unlockLightswitch(&readSwitch, &noWriters);

    return NULL;
}

void *writer(void *threadNum)
{
    lockLightswitch(&writeSwitch, &noReaders);
    sem_wait(&noWriters);

    myWrite(*(int*)threadNum);

    sem_post(&noWriters);
    unlockLightswitch(&writeSwitch, &noReaders);

    return NULL;
}