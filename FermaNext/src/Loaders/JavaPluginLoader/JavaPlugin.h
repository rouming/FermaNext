
#ifndef JAVAPLUGIN_H
#define JAVAPLUGIN_H

#include "Plugin.h"

/** 
 * Java plugin class. Is a bridge between Ferma calls
 * and real Java plugins. See #Plugin for detailed 
 * description of whole plugin interface.
 */    
class JavaPlugin : public Plugin
{
public:
    JavaPlugin ( PluginManager& mng, const QString& path );
    virtual ~JavaPlugin ();

    virtual const PluginInfo& pluginInfo () const;
    virtual Status pluginStatusCode () const;
    virtual QString pluginStatusMsg () const;

    virtual const QStringList& requiredPluginTypes () const;
    virtual ResolvingMode resolvingMode () const;
protected:
    virtual void startWizardSetup ();
};

#endif //JAVAPLUGIN_H
