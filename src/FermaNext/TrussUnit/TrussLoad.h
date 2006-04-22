
#ifndef TRUSSLOAD_H
#define TRUSSLOAD_H

#include <vector>
#include <QObject>
#include <QMap>

#include "StatefulObject.h"

class Node;

/*****************************************************************************
 * Truss Load
 *****************************************************************************/

class TrussLoad : public QObject
{
    Q_OBJECT
public:
    TrussLoad ();
    TrussLoad ( double x, double y );
    TrussLoad ( const TrussLoad& );
    TrussLoad& operator= ( const TrussLoad& );

    double getXForce () const ;
    double getYForce () const;

    void setXForce ( double );
    void setYForce ( double );
    void setForces ( double, double );

    bool isEnabled () const;
    bool isRemoved () const;

public slots:
    void disable ();
    void enable ();
    void remove ();

signals:
    void onForceChange ();
    void onEnableChange ( bool );

private:
    bool enabled;
    bool removed;
    double x;
    double y;
};

/*****************************************************************************
 * Truss Load Case Emitter
 *****************************************************************************/

class TrussLoadCaseEmitter : public QObject
{
    Q_OBJECT
public:
    TrussLoadCaseEmitter () {}

protected slots:
    virtual void loadIsChanged () = 0;

signals:
    void onAfterLoadCreation ( const Node& );
    void onAfterLoadRemoval ( const Node& );
    void onLoadChange ( const Node& );
};

/*****************************************************************************
 * Truss Load Case
 *****************************************************************************/

template <class N>
class TrussLoadCase : public TrussLoadCaseEmitter
{
public:
    // Basic typedefs
    typedef QMap<const N*, TrussLoad*> TrussLoadMap;
    typedef typename TrussLoadMap::Iterator TrussLoadMapIter;
    typedef typename TrussLoadMap::ConstIterator TrussLoadMapConstIter;

    TrussLoadCase ()
    {}

    virtual ~TrussLoadCase ()
    { 
        TrussLoadMapIter iter = loads.begin();
        for ( ; iter != loads.end(); )
            iter = removeLoad( iter );
    }

    // Add new node load
    virtual void addLoad ( const N& node, double x, double y )
    {
        // Do suspended clean
        suspendedClean();

        TrussLoad* load = 0;
        if ( loads.contains(&node) ) {
            load = loads[&node];
            load->setForces( x, y );
        }
        else {
            load = new TrussLoad( x, y );
            // Catch life time changes
            connect( &node, SIGNAL(onBeforeDesist(StatefulObject&)),
                      load, SLOT(disable()) );
            connect( &node, SIGNAL(onAfterRevive(StatefulObject&)),
                      load, SLOT(enable()) );
            connect( &node, SIGNAL(destroyed()),
                      load, SLOT(remove()) );

            connect( load, SIGNAL(onForceChange()),
                           SLOT(loadIsChanged()) );
            connect( load, SIGNAL(onEnableChange(bool)),
                           SLOT(loadIsChanged()) );
            
            loads[&node] = load;

            emit onAfterLoadCreation( node );
        }
    }

    // Remove node load
    virtual TrussLoadMapIter removeLoad ( TrussLoadMapIter iter )
    {
        if ( iter == loads.end() )
            return loads.end();

        TrussLoad* load = iter.value();
        const N* node = iter.key();
        QObject::disconnect( node, 0, load, 0 );
        delete load;
        iter = loads.erase( iter );
        emit onAfterLoadRemoval( *node );
        return iter;
    }

    virtual bool removeLoad ( const N& node ) 
    {
        // Do suspended clean
        suspendedClean();

        TrussLoadMapIter iter = removeLoad( loads.find(&node) );
        if ( iter == loads.end() )
            return false;
        else
            return true;       
    }

    // Returns load, 0 if fails
    virtual TrussLoad* findLoad ( const N& node ) const
    {
        if ( loads.contains(&node) ) {
            TrussLoad* load = loads[&node];
            if ( load->isEnabled() )
                return load;
            return 0;
        }
        return 0;
    }

    virtual TrussLoadMap getTrussLoadMap () const
    {
        return loads;
    }

    virtual size_t countLoads () const
    {
        size_t loadsNum = 0;
        TrussLoadMapConstIter iter = loads.begin();
        for ( ; iter != loads.end(); ++iter ) {
            if ( iter.value()->isEnabled() ) 
                ++loadsNum;
        }
        return loadsNum;
    }

protected:
    void suspendedClean ()
    {
        TrussLoadMapIter iter = loads.begin();
        for ( ; iter != loads.end(); ) {
            TrussLoad* load = iter.value();
            if ( load->isRemoved() )
                iter = removeLoad( iter );
        }
    }

    void loadIsChanged ()
    {
        TrussLoad* load = dynamic_cast<TrussLoad*>(sender());
        if ( ! load )
            return;

        const Node* node = loads.key( load );
        if ( ! node )
            return;    
        emit onLoadChange( *node );
    }

private:
    TrussLoadCase ( const TrussLoadCase& );
    TrussLoadCase& operator= ( const TrussLoadCase& );

private:
    TrussLoadMap loads;
};

/*****************************************************************************
 * Truss Load Case Array
 *****************************************************************************/

template <class N>
class TrussLoadCaseArray
{
public:
    TrussLoadCaseArray () :
        currentLoadCase(0)
    {}

    virtual ~TrussLoadCaseArray ()
    {
        clean();
    }

    // Create new load case
    virtual TrussLoadCase<N>& createLoadCase ()
    {
        TrussLoadCase<N>* loadCase = new TrussLoadCase<N>;
        loadCases.push_back( loadCase );
        return *loadCase;
    }

    // Remove specified load case
    virtual bool removeLoadCase ( TrussLoadCase<N>& loadCase )
    {
        TrussLoadCaseListIter iter = std::find( loadCases.begin(),
                                                loadCases.end(),
                                                &loadCase );
        if ( iter == loadCases.end() )
            return false;

        // Zero current
        if ( *iter == currentLoadCase )
            currentLoadCase = 0;
        delete *iter;
        loadCases.erase(iter);
        return true;
    }

    // Remove load case by index
    virtual bool removeLoadCase ( int indx )
    {
        // Load cases index starts with 1, not 0
        if ( indx == 0 || indx > loadCases.size() )
            return false;
        int index = indx - 1;
        
        try {
            return removeLoadCase( *loadCases.at(index) );
        } 
        catch ( ... ) {
            return false;
        }
    }

    // Set current load case by index
    virtual bool setCurrentLoadCase ( int indx )
    {
        // Load cases index starts with 1, not 0
        if ( indx == 0 || indx > loadCases.size() )
            return false;
        int index = indx - 1;
        return setCurrentLoadCase( *loadCases.at(index) );
    }

    // Set current specified load case if it is in vector
    virtual bool setCurrentLoadCase ( TrussLoadCase<N>& loadCase ) 
    {
        // Make sure load case is in vector
        TrussLoadCaseListIter iter = std::find( loadCases.begin(),
                                                loadCases.end(),
                                                &loadCase );
        if ( iter == loadCases.end() )
            return false;

        currentLoadCase = &loadCase;
        return true;
    }

    virtual TrussLoadCase<N>* getCurrentLoadCase () const
    {
        return currentLoadCase;
    }

    virtual int getLoadCaseIndex ( TrussLoadCase<N>& loadCase ) const
    {
        // Load cases index starts with 1, not 0
        int indx = 1;
        TrussLoadCaseListConstIter iter = loadCases.begin();
        for ( ; iter != loadCases.end(); ++iter ) {
            if ( *iter == &loadCase )
                return indx;
            ++indx;
        }
        return 0;
    }

    // Try to find load case by index. 
    virtual TrussLoadCase<N>* findLoadCase ( int indx ) const
    {
        // Load cases index starts with 1, not 0
        if ( indx == 0 || indx > loadCases.size() )
            return 0;

        int index = indx - 1;
        try {
            return loadCases.at(index);
        }
        catch ( ... ) {
            return 0;
        }
    }

    virtual size_t countLoadCases () const
    {
        return loadCases.size();
    }

protected:
    void clean ()
    {
        TrussLoadCaseListIter iter = loadCases.begin();
        for ( ; iter != loadCases.end(); ++iter )
            delete *iter;
        loadCases.clear();
    }

private:
    TrussLoadCaseArray ( const TrussLoadCaseArray& );
    TrussLoadCaseArray& operator= ( const TrussLoadCaseArray& );
    
private:
    typedef std::vector<TrussLoadCase<N>*> TrussLoadCaseList;
    typedef typename TrussLoadCaseList::iterator TrussLoadCaseListIter;
    typedef typename TrussLoadCaseList::const_iterator TrussLoadCaseListConstIter;

    TrussLoadCaseList loadCases;        
    TrussLoadCase<N>* currentLoadCase;
};

#endif //TRUSSLOAD_H
