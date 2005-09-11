
#ifndef OLD_SIMPLECALC_H
#define OLD_SIMPLECALC_H

#include <qthread.h>

class QFile;
class QSocketDevice;

// Temp file
QFile* tempFile = 0;

const char* fermaResExt = ".vyv";

// Manages temp file
QFile* createTempFile ();
void destroyTempFile ( QFile*& );


#endif //OLD_SIMPLECALC_H
