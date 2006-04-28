
#include <QThread>

#include "Config.h"

class ConfigTestThread : public QThread
{
    Q_OBJECT
public:
    ConfigTestThread ();    
    
    void run ();

    void set


public slots:
    void nodeChanged ( Config::Node );
    void nodeCreated ( Config::Node );
    void nodeRemoved ( Config::Node );

private:
    Config* config;
};

