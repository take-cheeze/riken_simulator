#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1024

double get_dtime(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ((double)(ts.tv_sec) + (double)(ts.tv_nsec) * 1e-9);
}

int main (void)
{
  int i, j, iteration = 100;
  double x[N], y[N], time = 0.0, gflops;
  const double alpha = 3.0;

  for (i = 0; i < N; i++) {
    y[i] = 0.0;
    x[i] = (double)(i);
  }

  __asm(".word 0xff420000"); // dump0
  time = get_dtime() - time;

#pragma omp parallel
  {
    fprintf(stdout, "I'm %d of %d.\n", omp_get_thread_num(), omp_get_num_threads());
    fflush(stdout);

    for (i = 0; i < iteration; i++) {
      for (j = 0; j < N; j++) {
	y[j] += alpha * x[j];
      }
    }
  }

  __asm(".word 0xff420000"); // dump1
  time = get_dtime() - time;

  gflops = (double)(N * iteration * 2) / time / 1e9;

  printf("time = %lf ms, %lf GFLOPS, dummy = %lf\n", 
	 time * 1000.0, gflops, y[N-1]);

  return 0;
}
