
#ifndef UNDOREDOLISTBOX_H
#define UNDOREDOLISTBOX_H

#include "ObjectStateManager.h"
#include <qlistbox.h>

class UndoRedoListBox : public QListBox
{
    Q_OBJECT
public:
    UndoRedoListBox ( QWidget* parent = 0, const char* name = 0, WFlags f = 0 );
    virtual ~UndoRedoListBox ();

    virtual ObjectStateManager* getStateManager () const;
    virtual void setStateManager ( ObjectStateManager* );

private:
    ObjectStateManager* stateManager;    
};

#endif //UNDOREDOLISTBOX_H
