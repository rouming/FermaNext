
#ifndef UNDOREDOLISTBOX_H
#define UNDOREDOLISTBOX_H

// Qt3 Support classes
#include <Q3ListBox>

#include "ObjectStateManager.h"

class UndoRedoListBox : public Q3ListBox
{
    Q_OBJECT
public:
    UndoRedoListBox ( QWidget* parent = 0, Qt::WFlags f = 0 );
    virtual ~UndoRedoListBox ();

    virtual ObjectStateManager* getStateManager () const;
    virtual void setStateManager ( ObjectStateManager* );

protected slots:
    virtual void clickOnItem ( Q3ListBoxItem* );

private:
    ObjectStateManager* stateManager;    
};

#endif //UNDOREDOLISTBOX_H
