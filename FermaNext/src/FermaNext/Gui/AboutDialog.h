
#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

#include "ui_AboutDialog.h"

class AboutDialog : public QDialog, 
                    public Ui::AboutDialog
{
    Q_OBJECT
public:
    AboutDialog ( QWidget* parent );
};

#endif //ABOUTDIALOG_H
