#ifndef TRUSSDEFORMPICTURE_H
#define TRUSSDEFORMPICTURE_H

#include <QList>
#include <QRectF>

#include "AggWidget.h"
#include "AggSubsidiary.h"
#include "TrussUnit.h"

class ColorRangeControl;
class LoadCaseResults;
class PluginResults;
class TrussUnitCopy;
class TrussCopyNode;
class TrussCopyPivot;

/*****************************************************************************/

class TrussDeformPicture : public AggWidget
{
    Q_OBJECT
public:
    TrussDeformPicture ( const ColorRangeControl& colCtrl, 
                         QWidget* parent = 0 );
    ~TrussDeformPicture ();
    
    void fill ( TrussUnitCopy&, const PluginResults& );
    void setCanvasPosition ( const QPoint& );
    void resizeCanvas ( const QSize& size );
    double getBaseScaleMultiplier () const;
    void setCurrentScaleMultiplier ( double );
    void showElementNumbers ( bool );
    void clearDeformation ();

    // Handlers on events
    void aggPaintEvent ( QPaintEvent* );
    void aggResizeEvent ( QResizeEvent* );
    void aggKeyPressEvent ( QKeyEvent* );
    void aggMouseMoveEvent ( QMouseEvent* );
    void aggMouseReleaseEvent ( QMouseEvent* );
    void aggMousePressEvent ( QMouseEvent* );
    void aggCtrlChangedEvent ( const agg::ctrl* );

protected:
    DoublePoint getTrussCoordFromWidgetPos ( double, double ) const;
    DoublePoint getTrussCoordFromWidgetPos ( const DoublePoint& ) const;
    DoublePoint getWidgetPosFromTrussCoord ( double, double ) const;
    DoublePoint getWidgetPosFromTrussCoord ( const DoublePoint& ) const;

    void drawCoordDomain ( ren_dynarow& ) const;
    void drawTrussAreaBoundaries ( ren_dynarow& ) const;

    void drawNodeNumber ( const TrussCopyNode&, ren_dynarow&, 
                          solidRenderer&, scanline_rasterizer&, 
                          agg::scanline_p8& ) const;

    void drawNodeFixation ( const TrussCopyNode&, scanline_rasterizer&, 
                            solidRenderer&, agg::scanline_p8&, 
                            color_type ) const;

    void drawNode ( const TrussCopyNode&, ren_dynarow& ) const;

    void drawPivot ( const TrussCopyPivot&, ren_dynarow&, color_type ) const;

    void drawPivotNumber ( const TrussCopyPivot&, ren_dynarow&, 
                           solidRenderer&, scanline_rasterizer&, 
                           agg::scanline_p8& ) const;

    void drawTruss ( ren_dynarow& ) const;
    void paint ( base_renderer& ) const; 

protected slots:
    void changeScale ( double );
    void changeDeform ( double );
    void showNumbers ( bool );
    void showDeform  ( bool, double );
    void switchLoadCaseResults ( int );
    void updateScaleMultiplier ();

signals:
    void loadCaseSwitched ( const LoadCaseResults& );
    void displacementLoaded ( bool );

private:
    TrussUnitCopy* trussCopy;
    rbuf_dynarow *canvasBuf;
    QRectF canvasBufRect, paintAreaRect;
    double scaleMult, currentScaleMult;
    bool numbersDrawing;
    const PluginResults* pluginResults;
    const ColorRangeControl& colorCtrl;
};

#endif //TRUSSDEFORMPICTURE_H
