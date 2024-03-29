
TARGET = agg23

LEVEL = ../../..
include($$LEVEL/FermaNext.pri)
include(agg23.pri)

TEMPLATE = lib
CONFIG += staticlib warn_off
QT -= core xml gui

# For static linking with expat
DEFINES += XML_STATIC

INCLUDEPATH += include

SOURCES = \
          # General agg23 sources
          \
          src/agg_arc.cpp \
          src/agg_arrowhead.cpp \
          src/agg_bezier_arc.cpp \
          src/agg_bspline.cpp \
          src/agg_curves.cpp \
          src/agg_embedded_raster_fonts.cpp \
          src/agg_gsv_text.cpp \
          src/agg_image_filters.cpp \
          src/agg_line_aa_basics.cpp \
          src/agg_line_profile_aa.cpp \
          src/agg_path_storage.cpp \
          src/agg_rasterizer_scanline_aa.cpp \
          src/agg_rounded_rect.cpp \
          src/agg_sqrt_tables.cpp \
          src/agg_trans_affine.cpp \
          src/agg_trans_double_path.cpp \
          src/agg_trans_single_path.cpp \
          src/agg_trans_warp_magnifier.cpp \
          src/agg_vcgen_bspline.cpp \
          src/agg_vcgen_contour.cpp \
          src/agg_vcgen_dash.cpp \
          src/agg_vcgen_markers_term.cpp \
          src/agg_vcgen_smooth_poly1.cpp \
          src/agg_vcgen_stroke.cpp \
          src/agg_vpgen_clip_polygon.cpp \
          src/agg_vpgen_clip_polyline.cpp \
          src/agg_vpgen_segmentator.cpp \
          src/ctrl/agg_bezier_ctrl.cpp \
          src/ctrl/agg_gamma_ctrl.cpp \
          src/ctrl/agg_polygon_ctrl.cpp \
          src/ctrl/agg_scale_ctrl.cpp \
          src/ctrl/agg_spline_ctrl.cpp \
          src/ctrl/agg_cbox_ctrl.cpp \
          src/ctrl/agg_gamma_spline.cpp \
          src/ctrl/agg_rbox_ctrl.cpp \
          src/ctrl/agg_slider_ctrl.cpp \
          \
          # SVG sources
          \
          svg/agg_svg_parser.cpp \
          svg/agg_svg_path_renderer.cpp \
          svg/agg_svg_path_tokenizer.cpp
          

