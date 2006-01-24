
#ifndef DYNALOADER_H
#define DYNALOADER_H

#if defined _WIN32 || defined WIN32  
  #include <windows.h>
  typedef HMODULE LibraryHandle;
  typedef FARPROC ProcAddress;
  #define dl_open(filename) (LibraryHandle)LoadLibrary(filename)
  #define dl_sym(handler, funcname) GetProcAddress(handler, funcname)
  #define dl_close(handler) FreeLibrary(handler)
#else
  #include <dlfcn.h>
  typedef void* LibraryHandle;
  typedef void* ProcAddress;
  #define dl_open(filename) dlopen(filename, RTLD_NOW | RTLD_GLOBAL)
  #define dl_sym(handler, funcname) dlsym(handler, funcname)
  #define dl_close(handler) dlclose(handler)
#endif

#include <qstring.h>

class DynaLoader
{
public:
    // OS dependent dynamic lib extension and prefix.
    static const QString& libExtension ();
    static const QString& libPrefix ();

public:
    // Excceptions
    class LibraryLoadException {};
    class AddressException {};

    DynaLoader ();
    DynaLoader ( const QString& fileName ) throw (LibraryLoadException);
    ~DynaLoader ();

    void loadLibrary ( const QString& fileName ) throw (LibraryLoadException);

    ProcAddress getProcAddress ( const QString& funcName ) const
                                                     throw (AddressException);

    template <class AddressPointer>
    ProcAddress getAddress ( const QString& funcName ) const
        throw (AddressException)
    { return (AddressPointer)getProcAddress(funcName); }


    bool freeLibrary ();
    bool isLoaded () const;

private:
    LibraryHandle handle;
};

#endif //DYNALOADER_H
