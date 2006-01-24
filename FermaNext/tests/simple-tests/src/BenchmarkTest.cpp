
#define BENCHMARK_ON

#include "Benchmark.h"

#include <string>
#include <iostream>

void sleep ( clock_t wait )
{
   clock_t goal;
   goal = wait + timer();
   while( goal > timer() );
}

int main ()
{
    const clock_t SLEEP1 = 1000;
    const clock_t SLEEP2 = 666;
    const clock_t SLEEP3 = 1234;
    const clock_t SLEEP4 = 200;
    const clock_t SLEEP5 = 3000;

   
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
