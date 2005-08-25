
#ifndef DYNALOADER_H
#define DYNALOADER_H

#if defined WINDOWS || defined WIN32  
  #include <windows.h>
  typedef HMODULE LibraryHandle;
  typedef FARPROC ProcessAddress;
  #define dl_open(filename) (LibraryHandle)LoadLibrary(filename)
  #define dl_sym(handler, funcname) GetProcAddress(handler, funcname)
  #define dl_close(handler) FreeLibrary(handler)
#else
  #include <dlfcn.h>
  typedef void* LibraryHandle;
  typedef void* ProcessAddress;
  #define dl_open(filename) dlopen(filename, RTLD_LAZY)
  #define dl_sym(handler, funcname) dlsym(handler, funcname)
  #define dl_close(handler) dlclose(handler)
#endif

#include <qstring.h>


class DynaLoader
{
public:
    // Excceptions
    class LibraryLoadException {};
    class AddressException {};

    DynaLoader ();
    DynaLoader ( const QString& fileName ) throw (LibraryLoadException);
    ~DynaLoader ();

    bool loadLibrary ( const QString& fileName );

    template <class AddressPointer>
    AddressPointer getAddress ( const QString& funcName ) 
                                                      throw (AddressException)
    { return (AddressPointer)getAddressHandler(funcName); }

    bool freeLibrary ();

protected:
    ProcessAddress getAddressHandler ( const QString& funcName ) 
                                                     throw (AddressException);

private:
    LibraryHandle handle;
};

#endif //DYNALOADER_H
