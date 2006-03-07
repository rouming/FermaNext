#!/usr/bin/perl -w

###
#
# TODO: remove referencies to JavaVM methods;
#       add feature to parse whole jni.h
#       add vivid comments

use strict;

my $ClassName = "JavaVirtualMachine";

print("Usage: <path_to_jni.h> header|src\n"), exit() if ( scalar @ARGV != 2 );

open( FILE, $ARGV[0] ) or print( "Can't open file '" . $ARGV[1] . "'\n"), 
                          exit(1);
chomp(my @file = <FILE>);
my $file = join "\n", @file;

my %cwords = ( throw => 'throwException' );

my %jtypes = map { my $v = $_; $v =~ s/(\w\w)(\w+)/uc($1) . $2/e;
                   ($_ => $v) } qw( jint
                                    jboolean
                                    jlong
                                    jsize
                                    jbyte
                                    jchar
                                    jshort
                                    jfloat
                                    jdouble

                                    jobject
                                    jclass
                                    jthrowable
                                    jstring
                                    jarray
                                    jbooleanArray
                                    jbyteArray
                                    jcharArray
                                    jshortArray
                                    jintArray
                                    jlongArray
                                    jfloatArray
                                    jdoubleArray
                                    jobjectArray
                                    jweak
                                    jvalue
                                    jfieldID
                                   jmethodID );

$jtypes{JNINativeMethod} = "JavaNativeMethod";

my $MethodName = undef;
my $ReturnType = undef;
my @ParamVals = ();
my @ParamTypes = ();


sub replaceJType {
    my $type = shift;
    for my $jtype ( keys %jtypes ) {
        return $type if $type =~ s/$jtype/$jtypes{$jtype}/eg;
    }
    return $type;
}

sub replaceCWords {
    my $word = shift;
    return  defined $cwords{$word} ? $cwords{$word} : $word;
}


sub parseArgs {
    my $args = shift;
    @ParamVals = ();
    @ParamTypes = ();
    if ( defined $args && $args ne '' ) {
        my @args = split/\s*,\s*/, $args;
        map {
              if ( /\Q.../ ) {
                  push @ParamVals, 'args';
                  push @ParamTypes, $_;
              } else {
                  /^(\s*)(.*?)(\w+)(\s*)$/;
                  push @ParamVals, $3;
                  push @ParamTypes, $2;
              }
          } @args;
        $args = join ", ", map { replaceJType($_) } @args;
        $args = " " . $args . " ";
    }
    return $args;
}


sub parseMethodHead {
    my $method = shift;
    my $header = shift;

    my $classPref = $header ? "" : $ClassName . "::";

    $method =~ s/(\s+)(.+?)(\s*)(\w+)(\s*)\((.*)\)/replaceJType($2) . " " .
             $classPref . replaceCWords(lcfirst($4)) . " (" . parseArgs($6)/se;

    $MethodName = $4;
    $ReturnType = replaceJType($2);

    $method =~ s/(.*?)\s$/$1/;
    return $method;
}

sub parseMethodBody {
    my $specBody = { before => '', after => '' };
    my $returnType = "";
    my $methodPrefix = $ReturnType ne 'void' ? "    return ($ReturnType)" :
                                               "    ";

	  my @pureVals = ();

    for ( my $i=0; $i < scalar @ParamVals; ++$i ) {
        push @pureVals, $ParamVals[$i];
        for my $jtype ( keys %jtypes ) {
            if ( $ParamTypes[$i] =~ /\Q.../ ) {
                $specBody->{before} = "    va_list args;\n";
                $specBody->{after}  = "    va_end(args);\n";
                if ( $ReturnType ne 'void' ) {
                    $specBody->{before} .= "    $ReturnType res;\n";
                    $methodPrefix = "    res = ($ReturnType)";
                    $returnType = "    return res;\n";
                }
                $specBody->{before} .= "    va_start(args, " .
                                       $pureVals[$i-1] . ");\n";
            }
            elsif ( $ParamTypes[$i] =~ /\Q$jtype/ ) {
                $ParamVals[$i] = "(". $ParamTypes[$i] . ")" .
                                      $ParamVals[$i];
            }
        }
    }
    my $params = join ", ", @ParamVals;
    $params = " " . $params . " " if $params;
    return "{\n". $specBody->{before} .
                  $methodPrefix ."pimpl_->env->$MethodName($params);\n" .
                  $specBody->{after} .
                  $returnType . "}\n";
}


my $headerFile = $file;
my $srcFile = $file;

$headerFile =~ s/(.*?){(.*?)}/"    " . parseMethodHead($1, 1) . ");\n\n"/gse;
$srcFile =~ s/(.*?){(.*?)}/parseMethodHead($1, 0) . ")\n" . parseMethodBody() . "\n"/gse;

if ( $ARGV[1] eq 'header' ) {
    print <<HEADER;
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
HEADER

    print $headerFile;

    print <<FOOTER;

private:
    class JavaVMImpl* pimpl_;
    DynaLoader dynaLoader;
};

#endif //JAVAVIRTUALMACHINE_H
FOOTER

} elsif ( $ARGV[1] eq 'src' ) {
    print <<HEADER;
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

HEADER

    print $srcFile;

    print <<FOOTER;
/*****************************************************************************/
FOOTER

} else {
    print "Usage: header|src\n";
}

