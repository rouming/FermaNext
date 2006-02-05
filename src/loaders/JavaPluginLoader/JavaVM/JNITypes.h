/***************************************************************************** 
 * This is jast a mapping of the original Java Native Interface
 ****************************************************************************/

#ifndef JNITYPES_H
#define JNITYPES_H

#if defined _WIN32 || defined WIN32
typedef __int32 int32_t;
typedef __int64 int64_t;
#else
#include <inttypes.h>
#endif

#ifdef _LP64 /* 64-bit Solaris */
typedef long JLong;
#else
typedef int64_t JLong;
#endif

typedef signed char     JByte;
typedef unsigned char   JBoolean;
typedef unsigned short  JChar;
typedef short           JShort;
typedef float           JFloat;
typedef double          JDouble;
typedef int32_t         JInt;
typedef JInt            JSize;

class _JObject {};
class _JClass : public _JObject {};
class _JThrowable : public _JObject {};
class _JString : public _JObject {};
class _JArray : public _JObject {};
class _JBooleanArray : public _JArray {};
class _JByteArray : public _JArray {};
class _JCharArray : public _JArray {};
class _JShortArray : public _JArray {};
class _JIntArray : public _JArray {};
class _JLongArray : public _JArray {};
class _JFloatArray : public _JArray {};
class _JDoubleArray : public _JArray {};
class _JObjectArray : public _JArray {};

typedef _JObject *JObject;
typedef _JClass *JClass;
typedef _JThrowable *JThrowable;
typedef _JString *JString;
typedef _JArray *JArray;
typedef _JBooleanArray *JBooleanArray;
typedef _JByteArray *JByteArray;
typedef _JCharArray *JCharArray;
typedef _JShortArray *JShortArray;
typedef _JIntArray *JIntArray;
typedef _JLongArray *JLongArray;
typedef _JFloatArray *JFloatArray;
typedef _JDoubleArray *JDoubleArray;
typedef _JObjectArray *JObjectArray;

typedef JObject JWeak;

typedef union JValue {
    JBoolean z;
    JByte    b;
    JChar    c;
    JShort   s;
    JInt     i;
    JLong    j;
    JFloat   f;
    JDouble  d;
    JObject  l;
} JValue;

struct _JFieldID;
typedef struct _JFieldID *JFieldID;

struct _JMethodID;
typedef struct _JMethodID *JMethodID;

/*
 * used in RegisterNatives to describe native method name, signature,
 * and function pointer.
 */

typedef struct {
    char *name;
    char *signature;
    void *fnPtr;
} JavaNativeMethod;

#endif //JNITYPES_H
