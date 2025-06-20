#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

long double fRand(long double fMin, long double fMax)
{
	long double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

int findPi(int meanIters)
{
	srand(time(NULL) + omp_get_thread_num());
	long double x, y;
	int score = 0;

	for (int i = 0; i < meanIters; i++)
	{
		x = fRand(-1.0, 1.0);
		y = fRand(-1.0, 1.0);

		if (x*x + y*y < 1.0)
			score++;
	}
	return score;
}

int main(int argc, char** argv)
{
    int iters = atoi(argv[1]);
    long int threads = atol(argv[2]);
	long double pi;
	int score = 0, mIters;

	mIters = iters / threads;
	double beginParallel = omp_get_wtime();

#pragma omp parallel for reduction(+:score) num_threads(threads)
	for (int i = 1; i <= threads; i++)
		score += findPi(mIters);

	double endParallel = omp_get_wtime();
	pi = 4.0 * ((long double)score / (long double)iters);

	double time_spent_parallel = endParallel - beginParallel;
	printf("\nCalculated pi : %.5Lf\n", pi);
    printf("Total points : %d\n", iters);
    printf("Threads used : %ld\n", threads);
	printf("Parallel Execution Time: %f\n", time_spent_parallel);

    FILE *file = fopen("result_monte_carlo_omp.txt", "a");
    if(file == NULL){
        perror("Failed to open result_monte_carlo_omp.txt");
        return 1;
    }

    fprintf(file, "Points: %d | Threads: %ld | Time: %.5f sec | Pi: %.10Lf\n",
            iters, threads, time_spent_parallel, pi);
    fclose(file);

	return 0;
}

