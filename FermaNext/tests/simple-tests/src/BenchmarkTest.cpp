
#define BENCHMARK_ON

#include "Benchmark.h"

#include <string>
#include <iostream>

void sleep ( clock_t wait )
{
   clock_t goal = wait + timer()/CLOCKS_PER_SEC;
   while( goal > (timer()/CLOCKS_PER_SEC) );
}

int main ()
{
    const clock_t SLEEP1 = 1;
    const clock_t SLEEP2 = 2;
    const clock_t SLEEP3 = 1;
    const clock_t SLEEP4 = 0;
    const clock_t SLEEP5 = 1;

   
    BEGIN_BENCHMARK("WindowRender");
    sleep(SLEEP1);

    BEGIN_BENCHMARK("TrussUnitRender");
    sleep(SLEEP2);

    BEGIN_BENCHMARK("NodeRender");
    sleep(SLEEP3);
    CLOSE_BENCHMARK;

    BEGIN_BENCHMARK("PivotRender");
    sleep(SLEEP4);
    CLOSE_BENCHMARK;

    CLOSE_ALL_BENCHMARKS

    BEGIN_BENCHMARK("BufferBlending");
    sleep(SLEEP5);
    CLOSE_ALL_BENCHMARKS;

    return 0;
}
