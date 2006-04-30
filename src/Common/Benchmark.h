
#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vector>
#include <iostream>
#include <string>
#include <QMutex>

#if defined _WIN32 || defined WIN32
 #include <time.h>
#else
 #include <sys/times.h>
 #include <unistd.h>
#endif

clock_t timer ();


/*****************************************************************************
 * Benchmark
 *****************************************************************************/

struct Mark
{
    clock_t mark;
    std::string name;
};

typedef std::vector<Mark> Marks;
typedef std::vector<Mark>::iterator MarksIter;

enum Status { Empty = 0, Begin, End };

class Benchmark
{
public:
    // Get Benchmark instance.
    static Benchmark& benchmark ();

    // Add new mark to marks tree with specified name.
    // Results are printed immediatelly.
    virtual void beginMark ( const std::string& name );

    // Close last mark.
    // Results are printed immediatelly.
    virtual void closeMark ();

    // Close all marks. Often called for quick closing.
    virtual void closeAllMarks ();

    // Print last mark like a node in the marks tree.
    virtual void printLastMark () const;

protected:
    virtual ~Benchmark ();

private:    
    Benchmark ();

    static Benchmark* instance;
    static QMutex mutex;
    Marks marks;
    Status status;
    bool markIsNode;
};

#ifdef BENCHMARK_ON

#define BEGIN_BENCHMARK(mark_name) Benchmark::benchmark().beginMark(mark_name);
#define CLOSE_BENCHMARK Benchmark::benchmark().closeMark();
#define CLOSE_ALL_BENCHMARKS Benchmark::benchmark().closeAllMarks();

#else

#define BEGIN_BENCHMARK(mark_name)
#define CLOSE_BENCHMARK
#define CLOSE_ALL_BENCHMARKS

#endif //BENCHMARK_ON


#endif //BENCHMARK_H
