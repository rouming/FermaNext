
#ifndef PRIVATECOMMON_H
#define PRIVATECOMMON_H

class PrivateSingleInstance
{
private:
    static PrivateSingleInstance* singleInstance;

    PrivateSingleInstance () {}

public:
    static PrivateSingleInstance& instance ()
    {
        if ( singleInstance == 0 )
            singleInstance = new PrivateSingleInstance;
        return *singleInstance;
    }    
};

PrivateSingleInstance* PrivateSingleInstance::singleInstance = 0;

#endif //PRIVATECOMMON_H
