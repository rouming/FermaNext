
#ifndef UNDOREDOLISTBOX_H
#define UNDOREDOLISTBOX_H

#include <QListWidget>

#include "ObjectStateManager.h"

class UndoRedoListBox : public QListWidget
{
    Q_OBJECT
public:
    UndoRedoListBox ( QWidget* parent = 0 );
    virtual ~UndoRedoListBox ();

    virtual ObjectStateManager* getStateManager () const;
    virtual void setStateManager ( ObjectStateManager* );

protected slots:
    virtual void clickOnItem ( QListWidgetItem* );

private:
    ObjectStateManager* stateManager;    
};

#endif //UNDOREDOLISTBOX_H
