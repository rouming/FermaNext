
#ifndef LIVEUPDATEDIALOG_H
#define LIVEUPDATEDIALOG_H

#include <QDialog>

#include "ui_LiveUpdateDialog.h"

class LiveUpdateDialog : public QDialog, 
                         public Ui::LiveUpdateDialog
{
    Q_OBJECT
public:
    LiveUpdateDialog ( QWidget* parent = 0 );

};

#endif //LIVEUPDATEDIALOG_H
