/*****************************************************************************
** Mapping from the original jni.h file
**
** Created: by jniparser.pl
**
******************************************************************************/

#ifndef JAVAVIRTUALMACHINE_H
#define JAVAVIRTUALMACHINE_H

#include <stdio.h>
#include <stdarg.h>
#include <QStringList>

#include "JNITypes.h"
#include "DynaLoader.h"

class JavaVirtualMachine
{
public:
    enum JavaVMVersion {
                         v1_1 = 0x00010001,
                         v1_2 = 0x00010002,
                         v1_4 = 0x00010004
                       };

    enum JavaVMErrorCode {
                        // Mapping from original jni.h
                        OK        =  0, //success
                        ERR       = -1, //unknown error
                        DETACHED  = -2, //thread detached from the VM
                        VERSION   = -3, //version error
                        NOMEM     = -4, //not enough memory
                        EXIST     = -5, //VM already created
                        INVAL     = -6, //invalid arguments
                        // Local extensions
                        NOTLOADED = -1000, //JVM is not loaded
                        WRONGJVM  = -1001  //Wrong JVM has been loaded
                         };

    //Exceptions
    struct JavaVMException { JavaVMErrorCode errCode; };
    class  JavaVMDynaLoadException {};

private:
    JavaVirtualMachine ( const JavaVirtualMachine& );
    JavaVirtualMachine& operator= ( const JavaVirtualMachine& );

    void createJavaVM ( const QString& sharedLibName,
                        JavaVMVersion, const QStringList& options )
        throw (JavaVMException, JavaVMDynaLoadException);
    void createJavaVM ( JavaVMVersion, const QStringList& options )
        throw (JavaVMException);

    void destroyJavaVM ();

public:
    JavaVirtualMachine ();
    JavaVirtualMachine ( const QString& sharedLibName,
                         JavaVMVersion, const QStringList& options )
        throw (JavaVMException, JavaVMDynaLoadException);
    ~JavaVirtualMachine ();

    bool isJVMLoaded () const;

    /////////////////////////////////////////
    // Methods mapping from original jni.h
    /////////////////////////////////////////
    JInt getVersion ();

    JClass defineClass ( const char *name, JObject loader, const JByte *buf, JSize len );

    JClass findClass ( const char *name );

    JMethodID fromReflectedMethod ( JObject method );

    JFieldID fromReflectedField ( JObject field );

    JObject toReflectedMethod ( JClass cls, JMethodID methodID, JBoolean isStatic );

    JClass getSuperclass ( JClass sub );

    JBoolean isAssignableFrom ( JClass sub, JClass sup );

    JObject toReflectedField ( JClass cls, JFieldID fieldID, JBoolean isStatic );

    JInt throwException ( JThrowable obj );

    JInt throwNew ( JClass clazz, const char *msg );

    JThrowable exceptionOccurred ();

    void exceptionDescribe ();

    void exceptionClear ();

    void fatalError ( const char *msg );

    JInt pushLocalFrame ( JInt capacity );

    JObject popLocalFrame ( JObject result );

    JObject newGlobalRef ( JObject lobj );

    void deleteGlobalRef ( JObject gref );

    void deleteLocalRef ( JObject obj );

    JBoolean isSameObject ( JObject obj1, JObject obj2 );

    JObject newLocalRef ( JObject ref );

    JInt ensureLocalCapacity ( JInt capacity );

    JObject allocObject ( JClass clazz );

    JObject newObject ( JClass clazz, JMethodID methodID, ... );

    JObject newObjectV ( JClass clazz, JMethodID methodID, va_list args );

    JObject newObjectA ( JClass clazz, JMethodID methodID, const JValue *args );

    JClass getObjectClass ( JObject obj );

    JBoolean isInstanceOf ( JObject obj, JClass clazz );

    JMethodID getMethodID ( JClass clazz, const char *name, const char *sig );

    JObject callObjectMethod ( JObject obj, JMethodID methodID, ... );

    JObject callObjectMethodV ( JObject obj, JMethodID methodID, va_list args );

    JObject callObjectMethodA ( JObject obj, JMethodID methodID, const JValue * args );

    JBoolean callBooleanMethod ( JObject obj, JMethodID methodID, ... );

    JBoolean callBooleanMethodV ( JObject obj, JMethodID methodID, va_list args );

    JBoolean callBooleanMethodA ( JObject obj, JMethodID methodID, const JValue * args );

    JByte callByteMethod ( JObject obj, JMethodID methodID, ... );

    JByte callByteMethodV ( JObject obj, JMethodID methodID, va_list args );

    JByte callByteMethodA ( JObject obj, JMethodID methodID, const JValue * args );

    JChar callCharMethod ( JObject obj, JMethodID methodID, ... );

    JChar callCharMethodV ( JObject obj, JMethodID methodID, va_list args );

    JChar callCharMethodA ( JObject obj, JMethodID methodID, const JValue * args );

    JShort callShortMethod ( JObject obj, JMethodID methodID, ... );

    JShort callShortMethodV ( JObject obj, JMethodID methodID, va_list args );

    JShort callShortMethodA ( JObject obj, JMethodID methodID, const JValue * args );

    JInt callIntMethod ( JObject obj, JMethodID methodID, ... );

    JInt callIntMethodV ( JObject obj, JMethodID methodID, va_list args );

    JInt callIntMethodA ( JObject obj, JMethodID methodID, const JValue * args );

    JLong callLongMethod ( JObject obj, JMethodID methodID, ... );

    JLong callLongMethodV ( JObject obj, JMethodID methodID, va_list args );

    JLong callLongMethodA ( JObject obj, JMethodID methodID, const JValue * args );

    JFloat callFloatMethod ( JObject obj, JMethodID methodID, ... );

    JFloat callFloatMethodV ( JObject obj, JMethodID methodID, va_list args );

    JFloat callFloatMethodA ( JObject obj, JMethodID methodID, const JValue * args );

    JDouble callDoubleMethod ( JObject obj, JMethodID methodID, ... );

    JDouble callDoubleMethodV ( JObject obj, JMethodID methodID, va_list args );

    JDouble callDoubleMethodA ( JObject obj, JMethodID methodID, const JValue * args );

    void callVoidMethod ( JObject obj, JMethodID methodID, ... );

    void callVoidMethodV ( JObject obj, JMethodID methodID, va_list args );

    void callVoidMethodA ( JObject obj, JMethodID methodID, const JValue * args );

    JObject callNonvirtualObjectMethod ( JObject obj, JClass clazz, JMethodID methodID, ... );

    JObject callNonvirtualObjectMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args );

    JObject callNonvirtualObjectMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args );

    JBoolean callNonvirtualBooleanMethod ( JObject obj, JClass clazz, JMethodID methodID, ... );

    JBoolean callNonvirtualBooleanMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args );

    JBoolean callNonvirtualBooleanMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args );

    JByte callNonvirtualByteMethod ( JObject obj, JClass clazz, JMethodID methodID, ... );

    JByte callNonvirtualByteMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args );

    JByte callNonvirtualByteMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args );

    JChar callNonvirtualCharMethod ( JObject obj, JClass clazz, JMethodID methodID, ... );

    JChar callNonvirtualCharMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args );

    JChar callNonvirtualCharMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args );

    JShort callNonvirtualShortMethod ( JObject obj, JClass clazz, JMethodID methodID, ... );

    JShort callNonvirtualShortMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args );

    JShort callNonvirtualShortMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args );

    JInt callNonvirtualIntMethod ( JObject obj, JClass clazz, JMethodID methodID, ... );

    JInt callNonvirtualIntMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args );

    JInt callNonvirtualIntMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args );

    JLong callNonvirtualLongMethod ( JObject obj, JClass clazz, JMethodID methodID, ... );

    JLong callNonvirtualLongMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args );

    JLong callNonvirtualLongMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args );

    JFloat callNonvirtualFloatMethod ( JObject obj, JClass clazz, JMethodID methodID, ... );

    JFloat callNonvirtualFloatMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args );

    JFloat callNonvirtualFloatMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args );

    JDouble callNonvirtualDoubleMethod ( JObject obj, JClass clazz, JMethodID methodID, ... );

    JDouble callNonvirtualDoubleMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args );

    JDouble callNonvirtualDoubleMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args );

    void callNonvirtualVoidMethod ( JObject obj, JClass clazz, JMethodID methodID, ... );

    void callNonvirtualVoidMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args );

    void callNonvirtualVoidMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args );

    JFieldID getFieldID ( JClass clazz, const char *name, const char *sig );

    JObject getObjectField ( JObject obj, JFieldID fieldID );

    JBoolean getBooleanField ( JObject obj, JFieldID fieldID );

    JByte getByteField ( JObject obj, JFieldID fieldID );

    JChar getCharField ( JObject obj, JFieldID fieldID );

    JShort getShortField ( JObject obj, JFieldID fieldID );

    JInt getIntField ( JObject obj, JFieldID fieldID );

    JLong getLongField ( JObject obj, JFieldID fieldID );

    JFloat getFloatField ( JObject obj, JFieldID fieldID );

    JDouble getDoubleField ( JObject obj, JFieldID fieldID );

    void setObjectField ( JObject obj, JFieldID fieldID, JObject val );

    void setBooleanField ( JObject obj, JFieldID fieldID, JBoolean val );

    void setByteField ( JObject obj, JFieldID fieldID, JByte val );

    void setCharField ( JObject obj, JFieldID fieldID, JChar val );

    void setShortField ( JObject obj, JFieldID fieldID, JShort val );

    void setIntField ( JObject obj, JFieldID fieldID, JInt val );

    void setLongField ( JObject obj, JFieldID fieldID, JLong val );

    void setFloatField ( JObject obj, JFieldID fieldID, JFloat val );

    void setDoubleField ( JObject obj, JFieldID fieldID, JDouble val );

    JMethodID getStaticMethodID ( JClass clazz, const char *name, const char *sig );

    JObject callStaticObjectMethod ( JClass clazz, JMethodID methodID, ... );

    JObject callStaticObjectMethodV ( JClass clazz, JMethodID methodID, va_list args );

    JObject callStaticObjectMethodA ( JClass clazz, JMethodID methodID, const JValue *args );

    JBoolean callStaticBooleanMethod ( JClass clazz, JMethodID methodID, ... );

    JBoolean callStaticBooleanMethodV ( JClass clazz, JMethodID methodID, va_list args );

    JBoolean callStaticBooleanMethodA ( JClass clazz, JMethodID methodID, const JValue *args );

    JByte callStaticByteMethod ( JClass clazz, JMethodID methodID, ... );

    JByte callStaticByteMethodV ( JClass clazz, JMethodID methodID, va_list args );

    JByte callStaticByteMethodA ( JClass clazz, JMethodID methodID, const JValue *args );

    JChar callStaticCharMethod ( JClass clazz, JMethodID methodID, ... );

    JChar callStaticCharMethodV ( JClass clazz, JMethodID methodID, va_list args );

    JChar callStaticCharMethodA ( JClass clazz, JMethodID methodID, const JValue *args );

    JShort callStaticShortMethod ( JClass clazz, JMethodID methodID, ... );

    JShort callStaticShortMethodV ( JClass clazz, JMethodID methodID, va_list args );

    JShort callStaticShortMethodA ( JClass clazz, JMethodID methodID, const JValue *args );

    JInt callStaticIntMethod ( JClass clazz, JMethodID methodID, ... );

    JInt callStaticIntMethodV ( JClass clazz, JMethodID methodID, va_list args );

    JInt callStaticIntMethodA ( JClass clazz, JMethodID methodID, const JValue *args );

    JLong callStaticLongMethod ( JClass clazz, JMethodID methodID, ... );

    JLong callStaticLongMethodV ( JClass clazz, JMethodID methodID, va_list args );

    JLong callStaticLongMethodA ( JClass clazz, JMethodID methodID, const JValue *args );

    JFloat callStaticFloatMethod ( JClass clazz, JMethodID methodID, ... );

    JFloat callStaticFloatMethodV ( JClass clazz, JMethodID methodID, va_list args );

    JFloat callStaticFloatMethodA ( JClass clazz, JMethodID methodID, const JValue *args );

    JDouble callStaticDoubleMethod ( JClass clazz, JMethodID methodID, ... );

    JDouble callStaticDoubleMethodV ( JClass clazz, JMethodID methodID, va_list args );

    JDouble callStaticDoubleMethodA ( JClass clazz, JMethodID methodID, const JValue *args );

    void callStaticVoidMethod ( JClass cls, JMethodID methodID, ... );

    void callStaticVoidMethodV ( JClass cls, JMethodID methodID, va_list args );

    void callStaticVoidMethodA ( JClass cls, JMethodID methodID, const JValue * args );

    JFieldID getStaticFieldID ( JClass clazz, const char *name, const char *sig );

    JObject getStaticObjectField ( JClass clazz, JFieldID fieldID );

    JBoolean getStaticBooleanField ( JClass clazz, JFieldID fieldID );

    JByte getStaticByteField ( JClass clazz, JFieldID fieldID );

    JChar getStaticCharField ( JClass clazz, JFieldID fieldID );

    JShort getStaticShortField ( JClass clazz, JFieldID fieldID );

    JInt getStaticIntField ( JClass clazz, JFieldID fieldID );

    JLong getStaticLongField ( JClass clazz, JFieldID fieldID );

    JFloat getStaticFloatField ( JClass clazz, JFieldID fieldID );

    JDouble getStaticDoubleField ( JClass clazz, JFieldID fieldID );

    void setStaticObjectField ( JClass clazz, JFieldID fieldID, JObject value );

    void setStaticBooleanField ( JClass clazz, JFieldID fieldID, JBoolean value );

    void setStaticByteField ( JClass clazz, JFieldID fieldID, JByte value );

    void setStaticCharField ( JClass clazz, JFieldID fieldID, JChar value );

    void setStaticShortField ( JClass clazz, JFieldID fieldID, JShort value );

    void setStaticIntField ( JClass clazz, JFieldID fieldID, JInt value );

    void setStaticLongField ( JClass clazz, JFieldID fieldID, JLong value );

    void setStaticFloatField ( JClass clazz, JFieldID fieldID, JFloat value );

    void setStaticDoubleField ( JClass clazz, JFieldID fieldID, JDouble value );

    JString newString ( const JChar *unicode, JSize len );

    JSize getStringLength ( JString str );

    const JChar * getStringChars ( JString str, JBoolean *isCopy );

    void releaseStringChars ( JString str, const JChar *chars );

    JString newStringUTF ( const char *utf );

    JSize getStringUTFLength ( JString str );

    const char* getStringUTFChars ( JString str, JBoolean *isCopy );

    void releaseStringUTFChars ( JString str, const char* chars );

    JSize getArrayLength ( JArray array );

    JObjectArray newObjectArray ( JSize len, JClass clazz, JObject init );

    JObject getObjectArrayElement ( JObjectArray array, JSize index );

    void setObjectArrayElement ( JObjectArray array, JSize index, JObject val );

    JBooleanArray newBooleanArray ( JSize len );

    JByteArray newByteArray ( JSize len );

    JCharArray newCharArray ( JSize len );

    JShortArray newShortArray ( JSize len );

    JIntArray newIntArray ( JSize len );

    JLongArray newLongArray ( JSize len );

    JFloatArray newFloatArray ( JSize len );

    JDoubleArray newDoubleArray ( JSize len );

    JBoolean * getBooleanArrayElements ( JBooleanArray array, JBoolean *isCopy );

    JByte * getByteArrayElements ( JByteArray array, JBoolean *isCopy );

    JChar * getCharArrayElements ( JCharArray array, JBoolean *isCopy );

    JShort * getShortArrayElements ( JShortArray array, JBoolean *isCopy );

    JInt * getIntArrayElements ( JIntArray array, JBoolean *isCopy );

    JLong * getLongArrayElements ( JLongArray array, JBoolean *isCopy );

    JFloat * getFloatArrayElements ( JFloatArray array, JBoolean *isCopy );

    JDouble * getDoubleArrayElements ( JDoubleArray array, JBoolean *isCopy );

    void releaseBooleanArrayElements ( JBooleanArray array, JBoolean *elems, JInt mode );

    void releaseByteArrayElements ( JByteArray array, JByte *elems, JInt mode );

    void releaseCharArrayElements ( JCharArray array, JChar *elems, JInt mode );

    void releaseShortArrayElements ( JShortArray array, JShort *elems, JInt mode );

    void releaseIntArrayElements ( JIntArray array, JInt *elems, JInt mode );

    void releaseLongArrayElements ( JLongArray array, JLong *elems, JInt mode );

    void releaseFloatArrayElements ( JFloatArray array, JFloat *elems, JInt mode );

    void releaseDoubleArrayElements ( JDoubleArray array, JDouble *elems, JInt mode );

    void getBooleanArrayRegion ( JBooleanArray array, JSize start, JSize len, JBoolean *buf );

    void getByteArrayRegion ( JByteArray array, JSize start, JSize len, JByte *buf );

    void getCharArrayRegion ( JCharArray array, JSize start, JSize len, JChar *buf );

    void getShortArrayRegion ( JShortArray array, JSize start, JSize len, JShort *buf );

    void getIntArrayRegion ( JIntArray array, JSize start, JSize len, JInt *buf );

    void getLongArrayRegion ( JLongArray array, JSize start, JSize len, JLong *buf );

    void getFloatArrayRegion ( JFloatArray array, JSize start, JSize len, JFloat *buf );

    void getDoubleArrayRegion ( JDoubleArray array, JSize start, JSize len, JDouble *buf );

    void setBooleanArrayRegion ( JBooleanArray array, JSize start, JSize len, const JBoolean *buf );

    void setByteArrayRegion ( JByteArray array, JSize start, JSize len, const JByte *buf );

    void setCharArrayRegion ( JCharArray array, JSize start, JSize len, const JChar *buf );

    void setShortArrayRegion ( JShortArray array, JSize start, JSize len, const JShort *buf );

    void setIntArrayRegion ( JIntArray array, JSize start, JSize len, const JInt *buf );

    void setLongArrayRegion ( JLongArray array, JSize start, JSize len, const JLong *buf );

    void setFloatArrayRegion ( JFloatArray array, JSize start, JSize len, const JFloat *buf );

    void setDoubleArrayRegion ( JDoubleArray array, JSize start, JSize len, const JDouble *buf );

    JInt registerNatives ( JClass clazz, const JavaNativeMethod *methods, JInt nMethods );

    JInt unregisterNatives ( JClass clazz );

    JInt monitorEnter ( JObject obj );

    JInt monitorExit ( JObject obj );

    void getStringRegion ( JString str, JSize start, JSize len, JChar *buf );

    void getStringUTFRegion ( JString str, JSize start, JSize len, char *buf );

    void * getPrimitiveArrayCritical ( JArray array, JBoolean *isCopy );

    void releasePrimitiveArrayCritical ( JArray array, void *carray, JInt mode );

    const JChar * getStringCritical ( JString string, JBoolean *isCopy );

    void releaseStringCritical ( JString string, const JChar *cstring );

    JWeak newWeakGlobalRef ( JObject obj );

    void deleteWeakGlobalRef ( JWeak ref );

    JBoolean exceptionCheck ();

    JObject newDirectByteBuffer ( void* address, JLong capacity );

    void* getDirectBufferAddress ( JObject buf );

    JLong getDirectBufferCapacity ( JObject buf );


private:
    class JavaVMImpl* pimpl_;
    DynaLoader dynaLoader;
};

#endif //JAVAVIRTUALMACHINE_H
