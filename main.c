#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX_NUM_THREADS 128

typedef struct Lightswitch_
{
    int counter;
    sem_t mutex;
}
Lightswitch;

Lightswitch readSwitch;
Lightswitch writeSwitch;
sem_t noReaders;
sem_t noWriters;

Lightswitch initLightswitch();
void *reader(void *threadNum);
void *writer(void *threadNum);

int main(int argc, char *argv[])
{
    srand(time(NULL));

    FILE *scenarios = fopen("scenarios.txt", "r");
    if (scenarios == NULL) {
        fprintf(stderr, "Error: scenarios.txt failed to open.\n");
    }

    readSwitch = initLightswitch();
    writeSwitch = initLightswitch();
    sem_init(&noReaders, 0, 1);
    sem_init(&noWriters, 0, 1);

    pthread_t *threads = NULL;
    char scenarioStr[MAX_NUM_THREADS + 1];
    int i = 0, scenarioNum = 1, strLen = 0, *threadNums = NULL;
    while (fscanf(scenarios, "%s ", scenarioStr) != EOF) {
        strLen = strlen(scenarioStr);

        if (strLen > MAX_NUM_THREADS) {
            fprintf(stderr, "\nError: SCENARIO %d creates too many threads. Max: %d\n", 
                scenarioNum++, MAX_NUM_THREADS);
            continue;
        }

        threads = malloc(strLen * sizeof(pthread_t));
        threadNums = calloc(strLen, sizeof(int));

        if (threads == NULL) {
            fprintf(stderr, "Error: malloc failed for threads.\n");
        }

        if (threadNums == NULL) {
            fprintf(stderr, "Error: calloc failed for threadNums.\n");
        }

        printf("\nSCENARIO %d: %s\n", scenarioNum++, scenarioStr);

        for (i = 0; i < strLen; i++) {
            threadNums[i] = i;
            if (scenarioStr[i] == 'W' || scenarioStr[i] == 'w') {
                pthread_create(&threads[i], NULL, writer, &threadNums[i]);
            }
            else if (scenarioStr[i] == 'R' || scenarioStr[i] == 'r') {
                pthread_create(&threads[i], NULL, reader, &threadNums[i]);
            }
        }

        for (i = 0; i < strLen; i++) {
            pthread_join(threads[i], NULL);
        }

        free(threads);
        free(threadNums);
    }

    fclose(scenarios);

    return 0;
}