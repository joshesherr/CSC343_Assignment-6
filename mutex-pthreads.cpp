#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char sharedData[100];
int dataReady = 0;

pthread_mutex_t lock;

void* writerThread(void* arg) {
    pthread_mutex_lock(&lock);
    strcpy(sharedData, "Message from Thread 1");
    dataReady = 1;
    printf("Thread 1 writing data to shared variable.\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* readerThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        if (dataReady) {
            printf("Thread 2 reading data: %s\n", sharedData);
            pthread_mutex_unlock(&lock);
            break;
        }
        pthread_mutex_unlock(&lock);
        usleep(1000);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, writerThread, NULL);
    sleep(1);
    pthread_create(&t2, NULL, readerThread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}
