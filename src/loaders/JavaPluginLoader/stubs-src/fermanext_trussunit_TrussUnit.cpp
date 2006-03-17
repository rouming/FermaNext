
#include "include/fermanext_trussunit_TrussUnit.h"

#include <iostream>


jint JNICALL Java_fermanext_trussunit_TrussUnit_countNodes
  (JNIEnv *, jobject)
{
    std::cout << "I'm 'fermanext.trussunit.TrussUnit.countNodes\n";
    return 666;
}

