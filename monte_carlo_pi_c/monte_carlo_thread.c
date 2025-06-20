#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long incircle = 0;
long points_per_thread;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *runner() {
    long incircle_thread = 0;

    unsigned int rand_state = rand();
    long i;
    for (i = 0; i < points_per_thread; i++) {

        double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;

        if (x * x + y * y < 1) {
            incircle_thread++;
        }
    }

    pthread_mutex_lock(&mutex);
    incircle += incircle_thread;
    pthread_mutex_unlock(&mutex);
}

int main(int argc, const char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "usage: ./monte_carlo_threads <total points> <threads>\n");
        exit(1);
    }

    long iters = atol(argv[1]);
    long threads = atoi(argv[2]);
    points_per_thread = iters / threads;

    time_t start = time(NULL);

    srand((unsigned)time(NULL));

    pthread_t *thread = malloc(threads * sizeof(pthread_t));
    if(thread == NULL) {
        perror("Failed to allocate memory for threads");
        exit(1);
    }

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int i;
    for (i = 0; i < threads; i++) {
        pthread_create(&thread[i], &attr, runner, (void *) NULL);
    }

    for (i = 0; i < threads; i++) {
        pthread_join(thread[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    free(thread);
    
    double pi = (4. * (double)incircle) / ((double)points_per_thread * threads);
    double taken_time = time(NULL) - start;
    printf("Points: %ld\n", iters);
    printf("Threads: %ld\n", threads);
    printf("Pi: %10f\n", pi);
    printf("Time: %f sec\n", taken_time);

    FILE *file = fopen("result_monte_carlo_threads.txt", "a");
    if(file == NULL){
        perror("Failed to open result_monte_carlo_threads.txt");
        return 1;
    }

    fprintf(file, "Points: %ld | Threads: %ld | Time: %.5f sec | Pi: %.10f\n",
            iters, threads, taken_time, pi);
    fclose(file);

    return 0;
}
