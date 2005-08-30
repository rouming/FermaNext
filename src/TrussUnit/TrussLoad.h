
#ifndef TRUSSLOAD_H
#define TRUSSLOAD_H

#include <vector>
#include <qmap.h>

/*****************************************************************************
 * Truss Load
 *****************************************************************************/

class TrussLoad 
{
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

private:
    double x;
    double y;
};

/*****************************************************************************
 * Truss Load Case
 *****************************************************************************/

template <class N>
class TrussLoadCase
{
public:
    virtual ~TrussLoadCase ()
    { 
        clean();
    }

    // Add new node load
    virtual void addLoad ( const N& node, double x, double y )
    {
        TrussLoad* load = 0;
        if ( loads.contains(&node) ) {
            load = loads[&node];
            load->setForces( x, y );
        }
        else {
            load = new TrussLoad( x, y );
            loads[&node] = load;
        }
    }

    // Remove node load
    virtual bool removeLoad ( const N& node )
    {
        if ( loads.contains(&node) ) {
            delete loads[&node];
            loads.remove(&node);
            return true;
        }
        return false;
    }

    // Returns load, 0 if fails
    virtual TrussLoad* findLoad ( const N& node ) const
    {
        if ( loads.contains(&node) ) {
            return loads[&node];
        }
        return 0;
    }

    virtual size_t countLoads () const
    {
        return loads.size();
    }

protected:
    void clean ()
    {
        TrussLoadMapIter iter = loads.begin();
        for ( ; iter != loads.end(); ++iter )
            delete iter.data();
        loads.clear();
    }

private:
    typedef QMap<const N*, TrussLoad*> TrussLoadMap;
    typedef typename TrussLoadMap::iterator TrussLoadMapIter;

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
    virtual bool removeLoadCase ( uint indx )
    {
        // Load cases index starts with 1, not 0
        if ( indx == 0 || indx > loadCases.size() )
            return false;
        uint index = indx - 1;
        
        try {
            return removeLoadCase( *loadCases.at(index) );
        } 
        catch ( ... ) {
            return false;
        }
    }

    // Set current load case by index
    virtual bool setCurrentLoadCase ( uint indx )
    {
        // Load cases index starts with 1, not 0
        if ( indx == 0 || indx > loadCases.size() )
            return false;
        uint index = indx - 1;
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

    // Try to find load case by index. 
    virtual TrussLoadCase<N>* findLoadCase ( uint indx ) const
    {
        // Load cases index starts with 1, not 0
        if ( indx == 0 || indx > loadCases.size() )
            return 0;

        uint index = indx - 1;
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
    typedef std::vector<TrussLoadCase<N>*> TrussLoadCaseList;
    typedef typename TrussLoadCaseList::iterator TrussLoadCaseListIter;

    TrussLoadCaseList loadCases;        
    TrussLoadCase<N>* currentLoadCase;
};

#endif //TRUSSLOAD_H
