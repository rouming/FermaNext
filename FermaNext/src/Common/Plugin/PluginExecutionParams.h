
#ifndef PLUGINEXECUTIONPARAMS_H
#define PLUGINEXECUTIONPARAMS_H

#include <QHash>
#include <QPair>
#include <QVariant>

/** 
 * Container class, that uses multi hash to store 
 * params names and their values.
 */
class PluginExecutionParams
{
public:
    /** Adds param. */
    void addParam ( const QString& name, const QVariant& value );
    /** Removes params. */
    void removeParam ( const QString& name );

    /** Returns whole map of params */
    const QHash<QString, QVariant>& getParams () const;

private:
    QHash<QString, QVariant> params;
};

#endif //PLUGINEXECUTIONPARAMS_H
