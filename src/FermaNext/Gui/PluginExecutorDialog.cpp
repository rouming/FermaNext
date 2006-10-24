
#include "PluginExecutorDialog.h"

/*****************************************************************************/

PluginExecutorDialog::PluginExecutorDialog ( PluginManager& mng, QWidget* p ) :
    QDialog(p),
    plgMng(mng)    
{
    setupUi(this);

    
}

/*****************************************************************************/
