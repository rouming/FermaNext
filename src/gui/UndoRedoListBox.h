
#ifndef UNDOREDOLISTBOX_H
#define UNDOREDOLISTBOX_H

#include <QListWidget>

#include "ObjectStateManager.h"

class QListWidgetItem;

class UndoRedoListBox : public QListWidget
{
    Q_OBJECT
public:
    UndoRedoListBox ( QWidget* parent = 0, const char* name = 0,
                      Qt::WFlags f = 0 );
    virtual ~UndoRedoListBox ();

    virtual ObjectStateManager* getStateManager () const;
    virtual void setStateManager ( ObjectStateManager* );

protected slots:
    virtual void clickOnItem ( QListWidgetItem* ); 

private:
    ObjectStateManager* stateManager;    
};

#endif //UNDOREDOLISTBOX_H
