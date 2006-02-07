/*****************************************************************************
** Mapping from the original jni.h file
**
** Created: by jniparser.pl
**
******************************************************************************/

#include "JavaVirtualMachine.h"
#include <jni.h>

/*****************************************************************************
 * Pimpl idiom
 *****************************************************************************/

class JavaVMImpl
{
public:
    JavaVM* jvm;
    JNIEnv* env;
};

/*****************************************************************************
 * Java Virtual Machine
 *****************************************************************************/

JavaVirtualMachine::JavaVirtualMachine () :
    pimpl_(0)
{}

JavaVirtualMachine::JavaVirtualMachine ( const QString& sharedLibName,
                                         JavaVMVersion version,
                                         const QStringList& options )
    throw (JavaVMException, JavaVMDynaLoadException) :
    pimpl_(0)
{
    createJavaVM( sharedLibName, version, options );
}

JavaVirtualMachine::~JavaVirtualMachine ()
{
    destroyJavaVM();
}

bool JavaVirtualMachine::isJVMLoaded () const
{
    return pimpl_ != 0;
}

void JavaVirtualMachine::createJavaVM ( const QString& sharedLibName,
                                        JavaVMVersion version,
                                        const QStringList& optionsList )
    throw (JavaVMException, JavaVMDynaLoadException)
{
    if ( pimpl_ != 0 )
        destroyJavaVM();
    try { dynaLoader.loadLibrary( sharedLibName ); }
    catch ( DynaLoader::LibraryLoadException& ) {
        throw JavaVMDynaLoadException();
    }
    createJavaVM( version, optionsList );
}

void JavaVirtualMachine::createJavaVM ( JavaVMVersion version,
                                        const QStringList& optionsList )
    throw (JavaVMException)
{
    JavaVMException excpt;

    if ( pimpl_ != 0 ) {
        excpt.errCode = EXIST;
        throw excpt;
    }
    else if ( ! dynaLoader.isLoaded() ) {
        excpt.errCode = NOTLOADED;
        throw excpt;
    }

    typedef jint (* JNIJavaVMCreator)( JavaVM**, void**, void* );
    JNIJavaVMCreator jvmCreator = 0;

    try {
        jvmCreator = (JNIJavaVMCreator)dynaLoader.
                                 getProcAddress( "JNI_CreateJavaVM" );

    }
    catch ( DynaLoader::AddressException& ) {
        excpt.errCode = WRONGJVM;
        throw excpt;
    }

    JavaVMOption* options = new JavaVMOption[optionsList.size()];

    uint indx = 0;
    QStringList::ConstIterator optIt = optionsList.begin();
    for ( ; optIt != optionsList.end(); ++optIt, ++indx ) {
        QString optStr = *optIt;
        // Safe copy
        options[indx].optionString =
            qstrcpy( new char[optStr.length() + 1], optStr.ascii() );
        options[indx].extraInfo = 0;
    }

    JavaVMInitArgs vm_args;
    vm_args.version = (jint)version;
    vm_args.options = options;
    vm_args.nOptions = optionsList.size();
    vm_args.ignoreUnrecognized = 1;

    pimpl_ = new JavaVMImpl;

    jint res = jvmCreator( &pimpl_->jvm, (void**)&pimpl_->env, &vm_args );

    for ( indx = 0; indx != optionsList.size(); ++indx ) {
        delete [] options[indx].optionString;
    }
    delete [] options;

    if ( res < 0 ) {
        JavaVMException excp;
        excp.errCode = (JavaVMErrorCode)res;
        delete pimpl_;
        pimpl_ = 0;
        throw excp;
    }
}

void JavaVirtualMachine::destroyJavaVM ()
{
    if ( pimpl_ == 0 )
        return;
    pimpl_->jvm->DestroyJavaVM();
    delete pimpl_;
    pimpl_ = 0;
}

JInt JavaVirtualMachine::getVersion ()
{
    return (JInt)pimpl_->env->GetVersion();
}

JClass JavaVirtualMachine::defineClass ( const char *name, JObject loader, const JByte *buf, JSize len )
{
    return (JClass)pimpl_->env->DefineClass( name, (jobject )loader, (const jbyte *)buf, (jsize )len );
}

JClass JavaVirtualMachine::findClass ( const char *name )
{
    return (JClass)pimpl_->env->FindClass( name );
}

JMethodID JavaVirtualMachine::fromReflectedMethod ( JObject method )
{
    return (JMethodID)pimpl_->env->FromReflectedMethod( (jobject )method );
}

JFieldID JavaVirtualMachine::fromReflectedField ( JObject field )
{
    return (JFieldID)pimpl_->env->FromReflectedField( (jobject )field );
}

JObject JavaVirtualMachine::toReflectedMethod ( JClass cls, JMethodID methodID, JBoolean isStatic )
{
    return (JObject)pimpl_->env->ToReflectedMethod( (jclass )cls, (jmethodID )methodID, (jboolean )isStatic );
}

JClass JavaVirtualMachine::getSuperclass ( JClass sub )
{
    return (JClass)pimpl_->env->GetSuperclass( (jclass )sub );
}

JBoolean JavaVirtualMachine::isAssignableFrom ( JClass sub, JClass sup )
{
    return (JBoolean)pimpl_->env->IsAssignableFrom( (jclass )sub, (jclass )sup );
}

JObject JavaVirtualMachine::toReflectedField ( JClass cls, JFieldID fieldID, JBoolean isStatic )
{
    return (JObject)pimpl_->env->ToReflectedField( (jclass )cls, (jfieldID )fieldID, (jboolean )isStatic );
}

JInt JavaVirtualMachine::throwException ( JThrowable obj )
{
    return (JInt)pimpl_->env->Throw( (jthrowable )obj );
}

JInt JavaVirtualMachine::throwNew ( JClass clazz, const char *msg )
{
    return (JInt)pimpl_->env->ThrowNew( (jclass )clazz, msg );
}

JThrowable JavaVirtualMachine::exceptionOccurred ()
{
    return (JThrowable)pimpl_->env->ExceptionOccurred();
}

void JavaVirtualMachine::exceptionDescribe ()
{
    pimpl_->env->ExceptionDescribe();
}

void JavaVirtualMachine::exceptionClear ()
{
    pimpl_->env->ExceptionClear();
}

void JavaVirtualMachine::fatalError ( const char *msg )
{
    pimpl_->env->FatalError( msg );
}

JInt JavaVirtualMachine::pushLocalFrame ( JInt capacity )
{
    return (JInt)pimpl_->env->PushLocalFrame( (jint )capacity );
}

JObject JavaVirtualMachine::popLocalFrame ( JObject result )
{
    return (JObject)pimpl_->env->PopLocalFrame( (jobject )result );
}

JObject JavaVirtualMachine::newGlobalRef ( JObject lobj )
{
    return (JObject)pimpl_->env->NewGlobalRef( (jobject )lobj );
}

void JavaVirtualMachine::deleteGlobalRef ( JObject gref )
{
    pimpl_->env->DeleteGlobalRef( (jobject )gref );
}

void JavaVirtualMachine::deleteLocalRef ( JObject obj )
{
    pimpl_->env->DeleteLocalRef( (jobject )obj );
}

JBoolean JavaVirtualMachine::isSameObject ( JObject obj1, JObject obj2 )
{
    return (JBoolean)pimpl_->env->IsSameObject( (jobject )obj1, (jobject )obj2 );
}

JObject JavaVirtualMachine::newLocalRef ( JObject ref )
{
    return (JObject)pimpl_->env->NewLocalRef( (jobject )ref );
}

JInt JavaVirtualMachine::ensureLocalCapacity ( JInt capacity )
{
    return (JInt)pimpl_->env->EnsureLocalCapacity( (jint )capacity );
}

JObject JavaVirtualMachine::allocObject ( JClass clazz )
{
    return (JObject)pimpl_->env->AllocObject( (jclass )clazz );
}

JObject JavaVirtualMachine::newObject ( JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JObject res;
    va_start(args, methodID);
    res = (JObject)pimpl_->env->NewObject( (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JObject JavaVirtualMachine::newObjectV ( JClass clazz, JMethodID methodID, va_list args )
{
    return (JObject)pimpl_->env->NewObjectV( (jclass )clazz, (jmethodID )methodID, args );
}

JObject JavaVirtualMachine::newObjectA ( JClass clazz, JMethodID methodID, const JValue *args )
{
    return (JObject)pimpl_->env->NewObjectA( (jclass )clazz, (jmethodID )methodID, (const jvalue *)args );
}

JClass JavaVirtualMachine::getObjectClass ( JObject obj )
{
    return (JClass)pimpl_->env->GetObjectClass( (jobject )obj );
}

JBoolean JavaVirtualMachine::isInstanceOf ( JObject obj, JClass clazz )
{
    return (JBoolean)pimpl_->env->IsInstanceOf( (jobject )obj, (jclass )clazz );
}

JMethodID JavaVirtualMachine::getMethodID ( JClass clazz, const char *name, const char *sig )
{
    return (JMethodID)pimpl_->env->GetMethodID( (jclass )clazz, name, sig );
}

JObject JavaVirtualMachine::callObjectMethod ( JObject obj, JMethodID methodID, ... )
{
    va_list args;
    JObject res;
    va_start(args, methodID);
    res = (JObject)pimpl_->env->CallObjectMethod( (jobject )obj, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JObject JavaVirtualMachine::callObjectMethodV ( JObject obj, JMethodID methodID, va_list args )
{
    return (JObject)pimpl_->env->CallObjectMethodV( (jobject )obj, (jmethodID )methodID, args );
}

JObject JavaVirtualMachine::callObjectMethodA ( JObject obj, JMethodID methodID, const JValue * args )
{
    return (JObject)pimpl_->env->CallObjectMethodA( (jobject )obj, (jmethodID )methodID, (const jvalue * )args );
}

JBoolean JavaVirtualMachine::callBooleanMethod ( JObject obj, JMethodID methodID, ... )
{
    va_list args;
    JBoolean res;
    va_start(args, methodID);
    res = (JBoolean)pimpl_->env->CallBooleanMethod( (jobject )obj, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JBoolean JavaVirtualMachine::callBooleanMethodV ( JObject obj, JMethodID methodID, va_list args )
{
    return (JBoolean)pimpl_->env->CallBooleanMethodV( (jobject )obj, (jmethodID )methodID, args );
}

JBoolean JavaVirtualMachine::callBooleanMethodA ( JObject obj, JMethodID methodID, const JValue * args )
{
    return (JBoolean)pimpl_->env->CallBooleanMethodA( (jobject )obj, (jmethodID )methodID, (const jvalue * )args );
}

JByte JavaVirtualMachine::callByteMethod ( JObject obj, JMethodID methodID, ... )
{
    va_list args;
    JByte res;
    va_start(args, methodID);
    res = (JByte)pimpl_->env->CallByteMethod( (jobject )obj, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JByte JavaVirtualMachine::callByteMethodV ( JObject obj, JMethodID methodID, va_list args )
{
    return (JByte)pimpl_->env->CallByteMethodV( (jobject )obj, (jmethodID )methodID, args );
}

JByte JavaVirtualMachine::callByteMethodA ( JObject obj, JMethodID methodID, const JValue * args )
{
    return (JByte)pimpl_->env->CallByteMethodA( (jobject )obj, (jmethodID )methodID, (const jvalue * )args );
}

JChar JavaVirtualMachine::callCharMethod ( JObject obj, JMethodID methodID, ... )
{
    va_list args;
    JChar res;
    va_start(args, methodID);
    res = (JChar)pimpl_->env->CallCharMethod( (jobject )obj, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JChar JavaVirtualMachine::callCharMethodV ( JObject obj, JMethodID methodID, va_list args )
{
    return (JChar)pimpl_->env->CallCharMethodV( (jobject )obj, (jmethodID )methodID, args );
}

JChar JavaVirtualMachine::callCharMethodA ( JObject obj, JMethodID methodID, const JValue * args )
{
    return (JChar)pimpl_->env->CallCharMethodA( (jobject )obj, (jmethodID )methodID, (const jvalue * )args );
}

JShort JavaVirtualMachine::callShortMethod ( JObject obj, JMethodID methodID, ... )
{
    va_list args;
    JShort res;
    va_start(args, methodID);
    res = (JShort)pimpl_->env->CallShortMethod( (jobject )obj, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JShort JavaVirtualMachine::callShortMethodV ( JObject obj, JMethodID methodID, va_list args )
{
    return (JShort)pimpl_->env->CallShortMethodV( (jobject )obj, (jmethodID )methodID, args );
}

JShort JavaVirtualMachine::callShortMethodA ( JObject obj, JMethodID methodID, const JValue * args )
{
    return (JShort)pimpl_->env->CallShortMethodA( (jobject )obj, (jmethodID )methodID, (const jvalue * )args );
}

JInt JavaVirtualMachine::callIntMethod ( JObject obj, JMethodID methodID, ... )
{
    va_list args;
    JInt res;
    va_start(args, methodID);
    res = (JInt)pimpl_->env->CallIntMethod( (jobject )obj, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JInt JavaVirtualMachine::callIntMethodV ( JObject obj, JMethodID methodID, va_list args )
{
    return (JInt)pimpl_->env->CallIntMethodV( (jobject )obj, (jmethodID )methodID, args );
}

JInt JavaVirtualMachine::callIntMethodA ( JObject obj, JMethodID methodID, const JValue * args )
{
    return (JInt)pimpl_->env->CallIntMethodA( (jobject )obj, (jmethodID )methodID, (const jvalue * )args );
}

JLong JavaVirtualMachine::callLongMethod ( JObject obj, JMethodID methodID, ... )
{
    va_list args;
    JLong res;
    va_start(args, methodID);
    res = (JLong)pimpl_->env->CallLongMethod( (jobject )obj, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JLong JavaVirtualMachine::callLongMethodV ( JObject obj, JMethodID methodID, va_list args )
{
    return (JLong)pimpl_->env->CallLongMethodV( (jobject )obj, (jmethodID )methodID, args );
}

JLong JavaVirtualMachine::callLongMethodA ( JObject obj, JMethodID methodID, const JValue * args )
{
    return (JLong)pimpl_->env->CallLongMethodA( (jobject )obj, (jmethodID )methodID, (const jvalue * )args );
}

JFloat JavaVirtualMachine::callFloatMethod ( JObject obj, JMethodID methodID, ... )
{
    va_list args;
    JFloat res;
    va_start(args, methodID);
    res = (JFloat)pimpl_->env->CallFloatMethod( (jobject )obj, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JFloat JavaVirtualMachine::callFloatMethodV ( JObject obj, JMethodID methodID, va_list args )
{
    return (JFloat)pimpl_->env->CallFloatMethodV( (jobject )obj, (jmethodID )methodID, args );
}

JFloat JavaVirtualMachine::callFloatMethodA ( JObject obj, JMethodID methodID, const JValue * args )
{
    return (JFloat)pimpl_->env->CallFloatMethodA( (jobject )obj, (jmethodID )methodID, (const jvalue * )args );
}

JDouble JavaVirtualMachine::callDoubleMethod ( JObject obj, JMethodID methodID, ... )
{
    va_list args;
    JDouble res;
    va_start(args, methodID);
    res = (JDouble)pimpl_->env->CallDoubleMethod( (jobject )obj, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JDouble JavaVirtualMachine::callDoubleMethodV ( JObject obj, JMethodID methodID, va_list args )
{
    return (JDouble)pimpl_->env->CallDoubleMethodV( (jobject )obj, (jmethodID )methodID, args );
}

JDouble JavaVirtualMachine::callDoubleMethodA ( JObject obj, JMethodID methodID, const JValue * args )
{
    return (JDouble)pimpl_->env->CallDoubleMethodA( (jobject )obj, (jmethodID )methodID, (const jvalue * )args );
}

void JavaVirtualMachine::callVoidMethod ( JObject obj, JMethodID methodID, ... )
{
    va_list args;
    va_start(args, methodID);
    pimpl_->env->CallVoidMethod( (jobject )obj, (jmethodID )methodID, args );
    va_end(args);
}

void JavaVirtualMachine::callVoidMethodV ( JObject obj, JMethodID methodID, va_list args )
{
    pimpl_->env->CallVoidMethodV( (jobject )obj, (jmethodID )methodID, args );
}

void JavaVirtualMachine::callVoidMethodA ( JObject obj, JMethodID methodID, const JValue * args )
{
    pimpl_->env->CallVoidMethodA( (jobject )obj, (jmethodID )methodID, (const jvalue * )args );
}

JObject JavaVirtualMachine::callNonvirtualObjectMethod ( JObject obj, JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JObject res;
    va_start(args, methodID);
    res = (JObject)pimpl_->env->CallNonvirtualObjectMethod( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JObject JavaVirtualMachine::callNonvirtualObjectMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args )
{
    return (JObject)pimpl_->env->CallNonvirtualObjectMethodV( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
}

JObject JavaVirtualMachine::callNonvirtualObjectMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args )
{
    return (JObject)pimpl_->env->CallNonvirtualObjectMethodA( (jobject )obj, (jclass )clazz, (jmethodID )methodID, (const jvalue * )args );
}

JBoolean JavaVirtualMachine::callNonvirtualBooleanMethod ( JObject obj, JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JBoolean res;
    va_start(args, methodID);
    res = (JBoolean)pimpl_->env->CallNonvirtualBooleanMethod( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JBoolean JavaVirtualMachine::callNonvirtualBooleanMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args )
{
    return (JBoolean)pimpl_->env->CallNonvirtualBooleanMethodV( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
}

JBoolean JavaVirtualMachine::callNonvirtualBooleanMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args )
{
    return (JBoolean)pimpl_->env->CallNonvirtualBooleanMethodA( (jobject )obj, (jclass )clazz, (jmethodID )methodID, (const jvalue * )args );
}

JByte JavaVirtualMachine::callNonvirtualByteMethod ( JObject obj, JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JByte res;
    va_start(args, methodID);
    res = (JByte)pimpl_->env->CallNonvirtualByteMethod( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JByte JavaVirtualMachine::callNonvirtualByteMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args )
{
    return (JByte)pimpl_->env->CallNonvirtualByteMethodV( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
}

JByte JavaVirtualMachine::callNonvirtualByteMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args )
{
    return (JByte)pimpl_->env->CallNonvirtualByteMethodA( (jobject )obj, (jclass )clazz, (jmethodID )methodID, (const jvalue * )args );
}

JChar JavaVirtualMachine::callNonvirtualCharMethod ( JObject obj, JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JChar res;
    va_start(args, methodID);
    res = (JChar)pimpl_->env->CallNonvirtualCharMethod( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JChar JavaVirtualMachine::callNonvirtualCharMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args )
{
    return (JChar)pimpl_->env->CallNonvirtualCharMethodV( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
}

JChar JavaVirtualMachine::callNonvirtualCharMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args )
{
    return (JChar)pimpl_->env->CallNonvirtualCharMethodA( (jobject )obj, (jclass )clazz, (jmethodID )methodID, (const jvalue * )args );
}

JShort JavaVirtualMachine::callNonvirtualShortMethod ( JObject obj, JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JShort res;
    va_start(args, methodID);
    res = (JShort)pimpl_->env->CallNonvirtualShortMethod( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JShort JavaVirtualMachine::callNonvirtualShortMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args )
{
    return (JShort)pimpl_->env->CallNonvirtualShortMethodV( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
}

JShort JavaVirtualMachine::callNonvirtualShortMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args )
{
    return (JShort)pimpl_->env->CallNonvirtualShortMethodA( (jobject )obj, (jclass )clazz, (jmethodID )methodID, (const jvalue * )args );
}

JInt JavaVirtualMachine::callNonvirtualIntMethod ( JObject obj, JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JInt res;
    va_start(args, methodID);
    res = (JInt)pimpl_->env->CallNonvirtualIntMethod( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JInt JavaVirtualMachine::callNonvirtualIntMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args )
{
    return (JInt)pimpl_->env->CallNonvirtualIntMethodV( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
}

JInt JavaVirtualMachine::callNonvirtualIntMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args )
{
    return (JInt)pimpl_->env->CallNonvirtualIntMethodA( (jobject )obj, (jclass )clazz, (jmethodID )methodID, (const jvalue * )args );
}

JLong JavaVirtualMachine::callNonvirtualLongMethod ( JObject obj, JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JLong res;
    va_start(args, methodID);
    res = (JLong)pimpl_->env->CallNonvirtualLongMethod( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JLong JavaVirtualMachine::callNonvirtualLongMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args )
{
    return (JLong)pimpl_->env->CallNonvirtualLongMethodV( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
}

JLong JavaVirtualMachine::callNonvirtualLongMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args )
{
    return (JLong)pimpl_->env->CallNonvirtualLongMethodA( (jobject )obj, (jclass )clazz, (jmethodID )methodID, (const jvalue * )args );
}

JFloat JavaVirtualMachine::callNonvirtualFloatMethod ( JObject obj, JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JFloat res;
    va_start(args, methodID);
    res = (JFloat)pimpl_->env->CallNonvirtualFloatMethod( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JFloat JavaVirtualMachine::callNonvirtualFloatMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args )
{
    return (JFloat)pimpl_->env->CallNonvirtualFloatMethodV( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
}

JFloat JavaVirtualMachine::callNonvirtualFloatMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args )
{
    return (JFloat)pimpl_->env->CallNonvirtualFloatMethodA( (jobject )obj, (jclass )clazz, (jmethodID )methodID, (const jvalue * )args );
}

JDouble JavaVirtualMachine::callNonvirtualDoubleMethod ( JObject obj, JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JDouble res;
    va_start(args, methodID);
    res = (JDouble)pimpl_->env->CallNonvirtualDoubleMethod( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JDouble JavaVirtualMachine::callNonvirtualDoubleMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args )
{
    return (JDouble)pimpl_->env->CallNonvirtualDoubleMethodV( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
}

JDouble JavaVirtualMachine::callNonvirtualDoubleMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args )
{
    return (JDouble)pimpl_->env->CallNonvirtualDoubleMethodA( (jobject )obj, (jclass )clazz, (jmethodID )methodID, (const jvalue * )args );
}

void JavaVirtualMachine::callNonvirtualVoidMethod ( JObject obj, JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    va_start(args, methodID);
    pimpl_->env->CallNonvirtualVoidMethod( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
}

void JavaVirtualMachine::callNonvirtualVoidMethodV ( JObject obj, JClass clazz, JMethodID methodID, va_list args )
{
    pimpl_->env->CallNonvirtualVoidMethodV( (jobject )obj, (jclass )clazz, (jmethodID )methodID, args );
}

void JavaVirtualMachine::callNonvirtualVoidMethodA ( JObject obj, JClass clazz, JMethodID methodID, const JValue * args )
{
    pimpl_->env->CallNonvirtualVoidMethodA( (jobject )obj, (jclass )clazz, (jmethodID )methodID, (const jvalue * )args );
}

JFieldID JavaVirtualMachine::getFieldID ( JClass clazz, const char *name, const char *sig )
{
    return (JFieldID)pimpl_->env->GetFieldID( (jclass )clazz, name, sig );
}

JObject JavaVirtualMachine::getObjectField ( JObject obj, JFieldID fieldID )
{
    return (JObject)pimpl_->env->GetObjectField( (jobject )obj, (jfieldID )fieldID );
}

JBoolean JavaVirtualMachine::getBooleanField ( JObject obj, JFieldID fieldID )
{
    return (JBoolean)pimpl_->env->GetBooleanField( (jobject )obj, (jfieldID )fieldID );
}

JByte JavaVirtualMachine::getByteField ( JObject obj, JFieldID fieldID )
{
    return (JByte)pimpl_->env->GetByteField( (jobject )obj, (jfieldID )fieldID );
}

JChar JavaVirtualMachine::getCharField ( JObject obj, JFieldID fieldID )
{
    return (JChar)pimpl_->env->GetCharField( (jobject )obj, (jfieldID )fieldID );
}

JShort JavaVirtualMachine::getShortField ( JObject obj, JFieldID fieldID )
{
    return (JShort)pimpl_->env->GetShortField( (jobject )obj, (jfieldID )fieldID );
}

JInt JavaVirtualMachine::getIntField ( JObject obj, JFieldID fieldID )
{
    return (JInt)pimpl_->env->GetIntField( (jobject )obj, (jfieldID )fieldID );
}

JLong JavaVirtualMachine::getLongField ( JObject obj, JFieldID fieldID )
{
    return (JLong)pimpl_->env->GetLongField( (jobject )obj, (jfieldID )fieldID );
}

JFloat JavaVirtualMachine::getFloatField ( JObject obj, JFieldID fieldID )
{
    return (JFloat)pimpl_->env->GetFloatField( (jobject )obj, (jfieldID )fieldID );
}

JDouble JavaVirtualMachine::getDoubleField ( JObject obj, JFieldID fieldID )
{
    return (JDouble)pimpl_->env->GetDoubleField( (jobject )obj, (jfieldID )fieldID );
}

void JavaVirtualMachine::setObjectField ( JObject obj, JFieldID fieldID, JObject val )
{
    pimpl_->env->SetObjectField( (jobject )obj, (jfieldID )fieldID, (jobject )val );
}

void JavaVirtualMachine::setBooleanField ( JObject obj, JFieldID fieldID, JBoolean val )
{
    pimpl_->env->SetBooleanField( (jobject )obj, (jfieldID )fieldID, (jboolean )val );
}

void JavaVirtualMachine::setByteField ( JObject obj, JFieldID fieldID, JByte val )
{
    pimpl_->env->SetByteField( (jobject )obj, (jfieldID )fieldID, (jbyte )val );
}

void JavaVirtualMachine::setCharField ( JObject obj, JFieldID fieldID, JChar val )
{
    pimpl_->env->SetCharField( (jobject )obj, (jfieldID )fieldID, (jchar )val );
}

void JavaVirtualMachine::setShortField ( JObject obj, JFieldID fieldID, JShort val )
{
    pimpl_->env->SetShortField( (jobject )obj, (jfieldID )fieldID, (jshort )val );
}

void JavaVirtualMachine::setIntField ( JObject obj, JFieldID fieldID, JInt val )
{
    pimpl_->env->SetIntField( (jobject )obj, (jfieldID )fieldID, (jint )val );
}

void JavaVirtualMachine::setLongField ( JObject obj, JFieldID fieldID, JLong val )
{
    pimpl_->env->SetLongField( (jobject )obj, (jfieldID )fieldID, (jlong )val );
}

void JavaVirtualMachine::setFloatField ( JObject obj, JFieldID fieldID, JFloat val )
{
    pimpl_->env->SetFloatField( (jobject )obj, (jfieldID )fieldID, (jfloat )val );
}

void JavaVirtualMachine::setDoubleField ( JObject obj, JFieldID fieldID, JDouble val )
{
    pimpl_->env->SetDoubleField( (jobject )obj, (jfieldID )fieldID, (jdouble )val );
}

JMethodID JavaVirtualMachine::getStaticMethodID ( JClass clazz, const char *name, const char *sig )
{
    return (JMethodID)pimpl_->env->GetStaticMethodID( (jclass )clazz, name, sig );
}

JObject JavaVirtualMachine::callStaticObjectMethod ( JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JObject res;
    va_start(args, methodID);
    res = (JObject)pimpl_->env->CallStaticObjectMethod( (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JObject JavaVirtualMachine::callStaticObjectMethodV ( JClass clazz, JMethodID methodID, va_list args )
{
    return (JObject)pimpl_->env->CallStaticObjectMethodV( (jclass )clazz, (jmethodID )methodID, args );
}

JObject JavaVirtualMachine::callStaticObjectMethodA ( JClass clazz, JMethodID methodID, const JValue *args )
{
    return (JObject)pimpl_->env->CallStaticObjectMethodA( (jclass )clazz, (jmethodID )methodID, (const jvalue *)args );
}

JBoolean JavaVirtualMachine::callStaticBooleanMethod ( JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JBoolean res;
    va_start(args, methodID);
    res = (JBoolean)pimpl_->env->CallStaticBooleanMethod( (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JBoolean JavaVirtualMachine::callStaticBooleanMethodV ( JClass clazz, JMethodID methodID, va_list args )
{
    return (JBoolean)pimpl_->env->CallStaticBooleanMethodV( (jclass )clazz, (jmethodID )methodID, args );
}

JBoolean JavaVirtualMachine::callStaticBooleanMethodA ( JClass clazz, JMethodID methodID, const JValue *args )
{
    return (JBoolean)pimpl_->env->CallStaticBooleanMethodA( (jclass )clazz, (jmethodID )methodID, (const jvalue *)args );
}

JByte JavaVirtualMachine::callStaticByteMethod ( JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JByte res;
    va_start(args, methodID);
    res = (JByte)pimpl_->env->CallStaticByteMethod( (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JByte JavaVirtualMachine::callStaticByteMethodV ( JClass clazz, JMethodID methodID, va_list args )
{
    return (JByte)pimpl_->env->CallStaticByteMethodV( (jclass )clazz, (jmethodID )methodID, args );
}

JByte JavaVirtualMachine::callStaticByteMethodA ( JClass clazz, JMethodID methodID, const JValue *args )
{
    return (JByte)pimpl_->env->CallStaticByteMethodA( (jclass )clazz, (jmethodID )methodID, (const jvalue *)args );
}

JChar JavaVirtualMachine::callStaticCharMethod ( JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JChar res;
    va_start(args, methodID);
    res = (JChar)pimpl_->env->CallStaticCharMethod( (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JChar JavaVirtualMachine::callStaticCharMethodV ( JClass clazz, JMethodID methodID, va_list args )
{
    return (JChar)pimpl_->env->CallStaticCharMethodV( (jclass )clazz, (jmethodID )methodID, args );
}

JChar JavaVirtualMachine::callStaticCharMethodA ( JClass clazz, JMethodID methodID, const JValue *args )
{
    return (JChar)pimpl_->env->CallStaticCharMethodA( (jclass )clazz, (jmethodID )methodID, (const jvalue *)args );
}

JShort JavaVirtualMachine::callStaticShortMethod ( JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JShort res;
    va_start(args, methodID);
    res = (JShort)pimpl_->env->CallStaticShortMethod( (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JShort JavaVirtualMachine::callStaticShortMethodV ( JClass clazz, JMethodID methodID, va_list args )
{
    return (JShort)pimpl_->env->CallStaticShortMethodV( (jclass )clazz, (jmethodID )methodID, args );
}

JShort JavaVirtualMachine::callStaticShortMethodA ( JClass clazz, JMethodID methodID, const JValue *args )
{
    return (JShort)pimpl_->env->CallStaticShortMethodA( (jclass )clazz, (jmethodID )methodID, (const jvalue *)args );
}

JInt JavaVirtualMachine::callStaticIntMethod ( JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JInt res;
    va_start(args, methodID);
    res = (JInt)pimpl_->env->CallStaticIntMethod( (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JInt JavaVirtualMachine::callStaticIntMethodV ( JClass clazz, JMethodID methodID, va_list args )
{
    return (JInt)pimpl_->env->CallStaticIntMethodV( (jclass )clazz, (jmethodID )methodID, args );
}

JInt JavaVirtualMachine::callStaticIntMethodA ( JClass clazz, JMethodID methodID, const JValue *args )
{
    return (JInt)pimpl_->env->CallStaticIntMethodA( (jclass )clazz, (jmethodID )methodID, (const jvalue *)args );
}

JLong JavaVirtualMachine::callStaticLongMethod ( JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JLong res;
    va_start(args, methodID);
    res = (JLong)pimpl_->env->CallStaticLongMethod( (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JLong JavaVirtualMachine::callStaticLongMethodV ( JClass clazz, JMethodID methodID, va_list args )
{
    return (JLong)pimpl_->env->CallStaticLongMethodV( (jclass )clazz, (jmethodID )methodID, args );
}

JLong JavaVirtualMachine::callStaticLongMethodA ( JClass clazz, JMethodID methodID, const JValue *args )
{
    return (JLong)pimpl_->env->CallStaticLongMethodA( (jclass )clazz, (jmethodID )methodID, (const jvalue *)args );
}

JFloat JavaVirtualMachine::callStaticFloatMethod ( JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JFloat res;
    va_start(args, methodID);
    res = (JFloat)pimpl_->env->CallStaticFloatMethod( (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JFloat JavaVirtualMachine::callStaticFloatMethodV ( JClass clazz, JMethodID methodID, va_list args )
{
    return (JFloat)pimpl_->env->CallStaticFloatMethodV( (jclass )clazz, (jmethodID )methodID, args );
}

JFloat JavaVirtualMachine::callStaticFloatMethodA ( JClass clazz, JMethodID methodID, const JValue *args )
{
    return (JFloat)pimpl_->env->CallStaticFloatMethodA( (jclass )clazz, (jmethodID )methodID, (const jvalue *)args );
}

JDouble JavaVirtualMachine::callStaticDoubleMethod ( JClass clazz, JMethodID methodID, ... )
{
    va_list args;
    JDouble res;
    va_start(args, methodID);
    res = (JDouble)pimpl_->env->CallStaticDoubleMethod( (jclass )clazz, (jmethodID )methodID, args );
    va_end(args);
    return res;
}

JDouble JavaVirtualMachine::callStaticDoubleMethodV ( JClass clazz, JMethodID methodID, va_list args )
{
    return (JDouble)pimpl_->env->CallStaticDoubleMethodV( (jclass )clazz, (jmethodID )methodID, args );
}

JDouble JavaVirtualMachine::callStaticDoubleMethodA ( JClass clazz, JMethodID methodID, const JValue *args )
{
    return (JDouble)pimpl_->env->CallStaticDoubleMethodA( (jclass )clazz, (jmethodID )methodID, (const jvalue *)args );
}

void JavaVirtualMachine::callStaticVoidMethod ( JClass cls, JMethodID methodID, ... )
{
    va_list args;
    va_start(args, methodID);
    pimpl_->env->CallStaticVoidMethod( (jclass )cls, (jmethodID )methodID, args );
    va_end(args);
}

void JavaVirtualMachine::callStaticVoidMethodV ( JClass cls, JMethodID methodID, va_list args )
{
    pimpl_->env->CallStaticVoidMethodV( (jclass )cls, (jmethodID )methodID, args );
}

void JavaVirtualMachine::callStaticVoidMethodA ( JClass cls, JMethodID methodID, const JValue * args )
{
    pimpl_->env->CallStaticVoidMethodA( (jclass )cls, (jmethodID )methodID, (const jvalue * )args );
}

JFieldID JavaVirtualMachine::getStaticFieldID ( JClass clazz, const char *name, const char *sig )
{
    return (JFieldID)pimpl_->env->GetStaticFieldID( (jclass )clazz, name, sig );
}

JObject JavaVirtualMachine::getStaticObjectField ( JClass clazz, JFieldID fieldID )
{
    return (JObject)pimpl_->env->GetStaticObjectField( (jclass )clazz, (jfieldID )fieldID );
}

JBoolean JavaVirtualMachine::getStaticBooleanField ( JClass clazz, JFieldID fieldID )
{
    return (JBoolean)pimpl_->env->GetStaticBooleanField( (jclass )clazz, (jfieldID )fieldID );
}

JByte JavaVirtualMachine::getStaticByteField ( JClass clazz, JFieldID fieldID )
{
    return (JByte)pimpl_->env->GetStaticByteField( (jclass )clazz, (jfieldID )fieldID );
}

JChar JavaVirtualMachine::getStaticCharField ( JClass clazz, JFieldID fieldID )
{
    return (JChar)pimpl_->env->GetStaticCharField( (jclass )clazz, (jfieldID )fieldID );
}

JShort JavaVirtualMachine::getStaticShortField ( JClass clazz, JFieldID fieldID )
{
    return (JShort)pimpl_->env->GetStaticShortField( (jclass )clazz, (jfieldID )fieldID );
}

JInt JavaVirtualMachine::getStaticIntField ( JClass clazz, JFieldID fieldID )
{
    return (JInt)pimpl_->env->GetStaticIntField( (jclass )clazz, (jfieldID )fieldID );
}

JLong JavaVirtualMachine::getStaticLongField ( JClass clazz, JFieldID fieldID )
{
    return (JLong)pimpl_->env->GetStaticLongField( (jclass )clazz, (jfieldID )fieldID );
}

JFloat JavaVirtualMachine::getStaticFloatField ( JClass clazz, JFieldID fieldID )
{
    return (JFloat)pimpl_->env->GetStaticFloatField( (jclass )clazz, (jfieldID )fieldID );
}

JDouble JavaVirtualMachine::getStaticDoubleField ( JClass clazz, JFieldID fieldID )
{
    return (JDouble)pimpl_->env->GetStaticDoubleField( (jclass )clazz, (jfieldID )fieldID );
}

void JavaVirtualMachine::setStaticObjectField ( JClass clazz, JFieldID fieldID, JObject value )
{
    pimpl_->env->SetStaticObjectField( (jclass )clazz, (jfieldID )fieldID, (jobject )value );
}

void JavaVirtualMachine::setStaticBooleanField ( JClass clazz, JFieldID fieldID, JBoolean value )
{
    pimpl_->env->SetStaticBooleanField( (jclass )clazz, (jfieldID )fieldID, (jboolean )value );
}

void JavaVirtualMachine::setStaticByteField ( JClass clazz, JFieldID fieldID, JByte value )
{
    pimpl_->env->SetStaticByteField( (jclass )clazz, (jfieldID )fieldID, (jbyte )value );
}

void JavaVirtualMachine::setStaticCharField ( JClass clazz, JFieldID fieldID, JChar value )
{
    pimpl_->env->SetStaticCharField( (jclass )clazz, (jfieldID )fieldID, (jchar )value );
}

void JavaVirtualMachine::setStaticShortField ( JClass clazz, JFieldID fieldID, JShort value )
{
    pimpl_->env->SetStaticShortField( (jclass )clazz, (jfieldID )fieldID, (jshort )value );
}

void JavaVirtualMachine::setStaticIntField ( JClass clazz, JFieldID fieldID, JInt value )
{
    pimpl_->env->SetStaticIntField( (jclass )clazz, (jfieldID )fieldID, (jint )value );
}

void JavaVirtualMachine::setStaticLongField ( JClass clazz, JFieldID fieldID, JLong value )
{
    pimpl_->env->SetStaticLongField( (jclass )clazz, (jfieldID )fieldID, (jlong )value );
}

void JavaVirtualMachine::setStaticFloatField ( JClass clazz, JFieldID fieldID, JFloat value )
{
    pimpl_->env->SetStaticFloatField( (jclass )clazz, (jfieldID )fieldID, (jfloat )value );
}

void JavaVirtualMachine::setStaticDoubleField ( JClass clazz, JFieldID fieldID, JDouble value )
{
    pimpl_->env->SetStaticDoubleField( (jclass )clazz, (jfieldID )fieldID, (jdouble )value );
}

JString JavaVirtualMachine::newString ( const JChar *unicode, JSize len )
{
    return (JString)pimpl_->env->NewString( (const jchar *)unicode, (jsize )len );
}

JSize JavaVirtualMachine::getStringLength ( JString str )
{
    return (JSize)pimpl_->env->GetStringLength( (jstring )str );
}

const JChar * JavaVirtualMachine::getStringChars ( JString str, JBoolean *isCopy )
{
    return (const JChar *)pimpl_->env->GetStringChars( (jstring )str, (jboolean *)isCopy );
}

void JavaVirtualMachine::releaseStringChars ( JString str, const JChar *chars )
{
    pimpl_->env->ReleaseStringChars( (jstring )str, (const jchar *)chars );
}

JString JavaVirtualMachine::newStringUTF ( const char *utf )
{
    return (JString)pimpl_->env->NewStringUTF( utf );
}

JSize JavaVirtualMachine::getStringUTFLength ( JString str )
{
    return (JSize)pimpl_->env->GetStringUTFLength( (jstring )str );
}

const char* JavaVirtualMachine::getStringUTFChars ( JString str, JBoolean *isCopy )
{
    return (const char*)pimpl_->env->GetStringUTFChars( (jstring )str, (jboolean *)isCopy );
}

void JavaVirtualMachine::releaseStringUTFChars ( JString str, const char* chars )
{
    pimpl_->env->ReleaseStringUTFChars( (jstring )str, chars );
}

JSize JavaVirtualMachine::getArrayLength ( JArray array )
{
    return (JSize)pimpl_->env->GetArrayLength( (jarray )array );
}

JObjectArray JavaVirtualMachine::newObjectArray ( JSize len, JClass clazz, JObject init )
{
    return (JObjectArray)pimpl_->env->NewObjectArray( (jsize )len, (jclass )clazz, (jobject )init );
}

JObject JavaVirtualMachine::getObjectArrayElement ( JObjectArray array, JSize index )
{
    return (JObject)pimpl_->env->GetObjectArrayElement( (jobjectArray )(jobjectArray )array, (jsize )index );
}

void JavaVirtualMachine::setObjectArrayElement ( JObjectArray array, JSize index, JObject val )
{
    pimpl_->env->SetObjectArrayElement( (jobjectArray )(jobjectArray )array, (jsize )index, (jobject )val );
}

JBooleanArray JavaVirtualMachine::newBooleanArray ( JSize len )
{
    return (JBooleanArray)pimpl_->env->NewBooleanArray( (jsize )len );
}

JByteArray JavaVirtualMachine::newByteArray ( JSize len )
{
    return (JByteArray)pimpl_->env->NewByteArray( (jsize )len );
}

JCharArray JavaVirtualMachine::newCharArray ( JSize len )
{
    return (JCharArray)pimpl_->env->NewCharArray( (jsize )len );
}

JShortArray JavaVirtualMachine::newShortArray ( JSize len )
{
    return (JShortArray)pimpl_->env->NewShortArray( (jsize )len );
}

JIntArray JavaVirtualMachine::newIntArray ( JSize len )
{
    return (JIntArray)pimpl_->env->NewIntArray( (jsize )len );
}

JLongArray JavaVirtualMachine::newLongArray ( JSize len )
{
    return (JLongArray)pimpl_->env->NewLongArray( (jsize )len );
}

JFloatArray JavaVirtualMachine::newFloatArray ( JSize len )
{
    return (JFloatArray)pimpl_->env->NewFloatArray( (jsize )len );
}

JDoubleArray JavaVirtualMachine::newDoubleArray ( JSize len )
{
    return (JDoubleArray)pimpl_->env->NewDoubleArray( (jsize )len );
}

JBoolean * JavaVirtualMachine::getBooleanArrayElements ( JBooleanArray array, JBoolean *isCopy )
{
    return (JBoolean *)pimpl_->env->GetBooleanArrayElements( (jbooleanArray )(jbooleanArray )array, (jboolean *)isCopy );
}

JByte * JavaVirtualMachine::getByteArrayElements ( JByteArray array, JBoolean *isCopy )
{
    return (JByte *)pimpl_->env->GetByteArrayElements( (jbyteArray )(jbyteArray )array, (jboolean *)isCopy );
}

JChar * JavaVirtualMachine::getCharArrayElements ( JCharArray array, JBoolean *isCopy )
{
    return (JChar *)pimpl_->env->GetCharArrayElements( (jcharArray )(jcharArray )array, (jboolean *)isCopy );
}

JShort * JavaVirtualMachine::getShortArrayElements ( JShortArray array, JBoolean *isCopy )
{
    return (JShort *)pimpl_->env->GetShortArrayElements( (jshortArray )(jshortArray )array, (jboolean *)isCopy );
}

JInt * JavaVirtualMachine::getIntArrayElements ( JIntArray array, JBoolean *isCopy )
{
    return (JInt *)pimpl_->env->GetIntArrayElements( (jintArray )(jintArray )array, (jboolean *)isCopy );
}

JLong * JavaVirtualMachine::getLongArrayElements ( JLongArray array, JBoolean *isCopy )
{
    return (JLong *)pimpl_->env->GetLongArrayElements( (jlongArray )(jlongArray )array, (jboolean *)isCopy );
}

JFloat * JavaVirtualMachine::getFloatArrayElements ( JFloatArray array, JBoolean *isCopy )
{
    return (JFloat *)pimpl_->env->GetFloatArrayElements( (jfloatArray )(jfloatArray )array, (jboolean *)isCopy );
}

JDouble * JavaVirtualMachine::getDoubleArrayElements ( JDoubleArray array, JBoolean *isCopy )
{
    return (JDouble *)pimpl_->env->GetDoubleArrayElements( (jdoubleArray )(jdoubleArray )array, (jboolean *)isCopy );
}

void JavaVirtualMachine::releaseBooleanArrayElements ( JBooleanArray array, JBoolean *elems, JInt mode )
{
    pimpl_->env->ReleaseBooleanArrayElements( (jbooleanArray )(jbooleanArray )array, (jboolean *)elems, (jint )mode );
}

void JavaVirtualMachine::releaseByteArrayElements ( JByteArray array, JByte *elems, JInt mode )
{
    pimpl_->env->ReleaseByteArrayElements( (jbyteArray )(jbyteArray )array, (jbyte *)elems, (jint )mode );
}

void JavaVirtualMachine::releaseCharArrayElements ( JCharArray array, JChar *elems, JInt mode )
{
    pimpl_->env->ReleaseCharArrayElements( (jcharArray )(jcharArray )array, (jchar *)elems, (jint )mode );
}

void JavaVirtualMachine::releaseShortArrayElements ( JShortArray array, JShort *elems, JInt mode )
{
    pimpl_->env->ReleaseShortArrayElements( (jshortArray )(jshortArray )array, (jshort *)elems, (jint )mode );
}

void JavaVirtualMachine::releaseIntArrayElements ( JIntArray array, JInt *elems, JInt mode )
{
    pimpl_->env->ReleaseIntArrayElements( (jintArray )(jintArray )array, (jint *)elems, (jint )mode );
}

void JavaVirtualMachine::releaseLongArrayElements ( JLongArray array, JLong *elems, JInt mode )
{
    pimpl_->env->ReleaseLongArrayElements( (jlongArray )(jlongArray )array, (jlong *)elems, (jint )mode );
}

void JavaVirtualMachine::releaseFloatArrayElements ( JFloatArray array, JFloat *elems, JInt mode )
{
    pimpl_->env->ReleaseFloatArrayElements( (jfloatArray )(jfloatArray )array, (jfloat *)elems, (jint )mode );
}

void JavaVirtualMachine::releaseDoubleArrayElements ( JDoubleArray array, JDouble *elems, JInt mode )
{
    pimpl_->env->ReleaseDoubleArrayElements( (jdoubleArray )(jdoubleArray )array, (jdouble *)elems, (jint )mode );
}

void JavaVirtualMachine::getBooleanArrayRegion ( JBooleanArray array, JSize start, JSize len, JBoolean *buf )
{
    pimpl_->env->GetBooleanArrayRegion( (jbooleanArray )(jbooleanArray )array, (jsize )start, (jsize )len, (jboolean *)buf );
}

void JavaVirtualMachine::getByteArrayRegion ( JByteArray array, JSize start, JSize len, JByte *buf )
{
    pimpl_->env->GetByteArrayRegion( (jbyteArray )(jbyteArray )array, (jsize )start, (jsize )len, (jbyte *)buf );
}

void JavaVirtualMachine::getCharArrayRegion ( JCharArray array, JSize start, JSize len, JChar *buf )
{
    pimpl_->env->GetCharArrayRegion( (jcharArray )(jcharArray )array, (jsize )start, (jsize )len, (jchar *)buf );
}

void JavaVirtualMachine::getShortArrayRegion ( JShortArray array, JSize start, JSize len, JShort *buf )
{
    pimpl_->env->GetShortArrayRegion( (jshortArray )(jshortArray )array, (jsize )start, (jsize )len, (jshort *)buf );
}

void JavaVirtualMachine::getIntArrayRegion ( JIntArray array, JSize start, JSize len, JInt *buf )
{
    pimpl_->env->GetIntArrayRegion( (jintArray )(jintArray )array, (jsize )start, (jsize )len, (jint *)buf );
}

void JavaVirtualMachine::getLongArrayRegion ( JLongArray array, JSize start, JSize len, JLong *buf )
{
    pimpl_->env->GetLongArrayRegion( (jlongArray )(jlongArray )array, (jsize )start, (jsize )len, (jlong *)buf );
}

void JavaVirtualMachine::getFloatArrayRegion ( JFloatArray array, JSize start, JSize len, JFloat *buf )
{
    pimpl_->env->GetFloatArrayRegion( (jfloatArray )(jfloatArray )array, (jsize )start, (jsize )len, (jfloat *)buf );
}

void JavaVirtualMachine::getDoubleArrayRegion ( JDoubleArray array, JSize start, JSize len, JDouble *buf )
{
    pimpl_->env->GetDoubleArrayRegion( (jdoubleArray )(jdoubleArray )array, (jsize )start, (jsize )len, (jdouble *)buf );
}

void JavaVirtualMachine::setBooleanArrayRegion ( JBooleanArray array, JSize start, JSize len, const JBoolean *buf )
{
    pimpl_->env->SetBooleanArrayRegion( (jbooleanArray )(jbooleanArray )array, (jsize )start, (jsize )len, (const jboolean *)buf );
}

void JavaVirtualMachine::setByteArrayRegion ( JByteArray array, JSize start, JSize len, const JByte *buf )
{
    pimpl_->env->SetByteArrayRegion( (jbyteArray )(jbyteArray )array, (jsize )start, (jsize )len, (const jbyte *)buf );
}

void JavaVirtualMachine::setCharArrayRegion ( JCharArray array, JSize start, JSize len, const JChar *buf )
{
    pimpl_->env->SetCharArrayRegion( (jcharArray )(jcharArray )array, (jsize )start, (jsize )len, (const jchar *)buf );
}

void JavaVirtualMachine::setShortArrayRegion ( JShortArray array, JSize start, JSize len, const JShort *buf )
{
    pimpl_->env->SetShortArrayRegion( (jshortArray )(jshortArray )array, (jsize )start, (jsize )len, (const jshort *)buf );
}

void JavaVirtualMachine::setIntArrayRegion ( JIntArray array, JSize start, JSize len, const JInt *buf )
{
    pimpl_->env->SetIntArrayRegion( (jintArray )(jintArray )array, (jsize )start, (jsize )len, (const jint *)buf );
}

void JavaVirtualMachine::setLongArrayRegion ( JLongArray array, JSize start, JSize len, const JLong *buf )
{
    pimpl_->env->SetLongArrayRegion( (jlongArray )(jlongArray )array, (jsize )start, (jsize )len, (const jlong *)buf );
}

void JavaVirtualMachine::setFloatArrayRegion ( JFloatArray array, JSize start, JSize len, const JFloat *buf )
{
    pimpl_->env->SetFloatArrayRegion( (jfloatArray )(jfloatArray )array, (jsize )start, (jsize )len, (const jfloat *)buf );
}

void JavaVirtualMachine::setDoubleArrayRegion ( JDoubleArray array, JSize start, JSize len, const JDouble *buf )
{
    pimpl_->env->SetDoubleArrayRegion( (jdoubleArray )(jdoubleArray )array, (jsize )start, (jsize )len, (const jdouble *)buf );
}

JInt JavaVirtualMachine::registerNatives ( JClass clazz, const JavaNativeMethod *methods, JInt nMethods )
{
    return (JInt)pimpl_->env->RegisterNatives( (jclass )clazz, (const JNINativeMethod *)methods, (jint )nMethods );
}

JInt JavaVirtualMachine::unregisterNatives ( JClass clazz )
{
    return (JInt)pimpl_->env->UnregisterNatives( (jclass )clazz );
}

JInt JavaVirtualMachine::monitorEnter ( JObject obj )
{
    return (JInt)pimpl_->env->MonitorEnter( (jobject )obj );
}

JInt JavaVirtualMachine::monitorExit ( JObject obj )
{
    return (JInt)pimpl_->env->MonitorExit( (jobject )obj );
}

void JavaVirtualMachine::getStringRegion ( JString str, JSize start, JSize len, JChar *buf )
{
    pimpl_->env->GetStringRegion( (jstring )str, (jsize )start, (jsize )len, (jchar *)buf );
}

void JavaVirtualMachine::getStringUTFRegion ( JString str, JSize start, JSize len, char *buf )
{
    pimpl_->env->GetStringUTFRegion( (jstring )str, (jsize )start, (jsize )len, buf );
}

void * JavaVirtualMachine::getPrimitiveArrayCritical ( JArray array, JBoolean *isCopy )
{
    return (void *)pimpl_->env->GetPrimitiveArrayCritical( (jarray )array, (jboolean *)isCopy );
}

void JavaVirtualMachine::releasePrimitiveArrayCritical ( JArray array, void *carray, JInt mode )
{
    pimpl_->env->ReleasePrimitiveArrayCritical( (jarray )array, carray, (jint )mode );
}

const JChar * JavaVirtualMachine::getStringCritical ( JString string, JBoolean *isCopy )
{
    return (const JChar *)pimpl_->env->GetStringCritical( (jstring )string, (jboolean *)isCopy );
}

void JavaVirtualMachine::releaseStringCritical ( JString string, const JChar *cstring )
{
    pimpl_->env->ReleaseStringCritical( (jstring )string, (const jchar *)cstring );
}

JWeak JavaVirtualMachine::newWeakGlobalRef ( JObject obj )
{
    return (JWeak)pimpl_->env->NewWeakGlobalRef( (jobject )obj );
}

void JavaVirtualMachine::deleteWeakGlobalRef ( JWeak ref )
{
    pimpl_->env->DeleteWeakGlobalRef( (jweak )ref );
}

JBoolean JavaVirtualMachine::exceptionCheck ()
{
    return (JBoolean)pimpl_->env->ExceptionCheck();
}

JObject JavaVirtualMachine::newDirectByteBuffer ( void* address, JLong capacity )
{
    return (JObject)pimpl_->env->NewDirectByteBuffer( address, (jlong )capacity );
}

void* JavaVirtualMachine::getDirectBufferAddress ( JObject buf )
{
    return (void*)pimpl_->env->GetDirectBufferAddress( (jobject )buf );
}

JLong JavaVirtualMachine::getDirectBufferCapacity ( JObject buf )
{
    return (JLong)pimpl_->env->GetDirectBufferCapacity( (jobject )buf );
}

/*****************************************************************************/
