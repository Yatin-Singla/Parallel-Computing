#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <assert.h>

int landed(double x, double y);
double piEstimator(long int n, int p);

int main(int argc, char *argv[])
{
    long int iterations;
    int p, debug = 0;
    if (argc < 2)
    {
        printf("Usage: piEstimator {iterations} [number of threads]\n");
        exit(1);
    }

    iterations = atoll(argv[1]);

    if (argc == 3)
    {
        p = atoi(argv[2]);
        assert(p >= 1);
	if (debug) {
        	printf("number of requested threads = %d\n", p);
        	printf("number of requested n = %ld\n", iterations);
	}
    }

    double time = omp_get_wtime();
    double pi = piEstimator(iterations, p);
    time = omp_get_wtime() - time;

    printf("%d,%ld,%.20lf,%lf\n", p, iterations, pi, time);
    return 0;
}

int landed(double x, double y)
{
    if ((double)(x * x) + (double)(y * y) <= 1.0)
        return 1;
    return 0;
}

double piEstimator(long int n, int p)
{
    omp_set_num_threads(p);

    double hits = 0, x, y;
    long int i;
#pragma omp parallel for reduction(+ \
                                   : hits) private(i, x, y)
    for (i = 0; i < n; i++)
    {
        struct drand48_data *buffer;
        int rank = omp_get_thread_num();
        int seed = rank + 1;
        seed = seed * i;
        //to initilize the struct
        srand48_r(seed, &buffer);
        drand48_r(&buffer, &x);
        drand48_r(&buffer, &y);
        // printf("x_rand=%2f\ny_rand=%2f\n", x_rand, y_rand);
        if (landed(x, y) == 1)
            hits += 1;
    }
    return 4.0 * (double)hits / (double)n;
}
