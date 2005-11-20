
#include "Benchmark.h"

clock_t timer ()
{
#if defined _WIN32 || defined WIN32
  return clock();
#else
  struct tms tms_tmp;
  return times(&tms_tmp);
#endif
}

/*****************************************************************************
 * Benchmark
 *****************************************************************************/

Benchmark* Benchmark::instance = 0;
QMutex Benchmark::mutex;

Benchmark::Benchmark () : 
    status(Empty),
    markIsNode(false)
{}

Benchmark::~Benchmark ()
{}

Benchmark& Benchmark::benchmark ()
{
    if ( instance == 0 ) {
        mutex.lock();
        if ( instance == 0 ) 
            instance = new Benchmark;
        mutex.unlock();
    }
    return *instance;
}

void Benchmark::beginMark ( const std::string& name )
{
    Mark mark;
    mark.name = name;
    mark.mark = timer();
    marks.push_back(mark);
    status = Begin;
    printLastMark();
}

void Benchmark::closeMark ()
{
    if ( status == Empty ) return;
    markIsNode = (status == Begin);
    status = End;
    printLastMark();
    marks.erase( marks.end() - 1 );
    if ( marks.size() == 0 )
        status = Empty;
}

void Benchmark::closeAllMarks ()
{
    while ( status != Empty )
        closeMark();        
}

void Benchmark::printLastMark () const
{
    if ( status == Empty ) return;
    Mark lastMark = marks.back();
    uint num = marks.size();
    std::string spaces;

    for ( uint i = 0; i < num -1; ++i )
        spaces += "-";

    if ( status == Begin ) {
        std::cout  << std::endl << spaces << lastMark.name;
    } else if ( status == End ) { 
        clock_t now = timer();
        double diff = double(now - lastMark.mark) / CLOCKS_PER_SEC;
        if ( markIsNode )
            std::cout << " (" << diff << " sec)";
        else 
            std::cout << std::endl << spaces << lastMark.name  << " (" << 
                         diff << " sec)";
        if ( num == 1 )
            std::cout << std::endl;
    }
}

/*****************************************************************************/
