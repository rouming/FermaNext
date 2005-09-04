
#ifndef OLD_SIMPLECALC_H
#define OLD_SIMPLECALC_H

class QFile;

// Temp file
QFile* tempFile = 0;

const char* fermaResExt = ".vyv";

// Manages temp file
QFile* createTempFile ();
void destroyTempFile ( QFile*& );


#endif //OLD_SIMPLECALC_H
