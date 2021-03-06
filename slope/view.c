/*
 * Copyright (C) 2015  Elvis Teixeira
 *
 * This source code is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "slope/view.h"


#define SLOPE_VIEW_PRIVATE(obj)          \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj),  \
    SLOPE_VIEW_TYPE, SlopeViewPrivate))


/**
 */
static gboolean
on_draw_event (GtkWidget *widget, cairo_t *cr, gpointer *data);


/**
 */
static gboolean
on_button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer *data);


/**
 */
static gboolean
on_button_move_event (GtkWidget *widget, GdkEventButton *event, gpointer *data);


/**
 */
static gboolean
on_button_release_event (GtkWidget *widget, GdkEventButton *event, gpointer *data);


/**
*/
typedef struct _SlopeViewPrivate SlopeViewPrivate;


/**
 */
struct _SlopeViewPrivate
{
    slope_figure_t *figure;
    cairo_surface_t *back_surf;
    slope_point_t move_start;
    slope_point_t move_end;
    slope_color_t mouse_rec_color;
    int on_move;
};


G_DEFINE_TYPE(SlopeView, slope_view, GTK_TYPE_DRAWING_AREA);


static void
slope_view_class_init(SlopeViewClass *klass)
{
    g_type_class_add_private(klass, sizeof(SlopeViewPrivate));
}


static void
slope_view_init(SlopeView *view)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_PRIVATE (view);
    GtkWidget *widget = GTK_WIDGET (view);

    priv->on_move = SLOPE_FALSE;
    slope_color_set_name(&priv->mouse_rec_color, SLOPE_BLACK);

    gtk_widget_add_events(widget,
                          GDK_EXPOSURE_MASK
                          |GDK_BUTTON_MOTION_MASK
                          |GDK_BUTTON_PRESS_MASK
                          |GDK_BUTTON_RELEASE_MASK);

    g_signal_connect(G_OBJECT(view), "draw",
                     G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(G_OBJECT(view), "button-press-event",
                     G_CALLBACK(on_button_press_event), NULL);
    g_signal_connect(G_OBJECT(view), "motion-notify-event",
                     G_CALLBACK(on_button_move_event), NULL);
    g_signal_connect(G_OBJECT(view), "button-release-event",
                     G_CALLBACK(on_button_release_event), NULL);
}


GtkWidget *
slope_view_new ()
{
    GtkWidget *view = GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));
    SlopeViewPrivate *priv = SLOPE_VIEW_PRIVATE (view);
    priv->figure = slope_figure_create();
    return view;
}


GtkWidget *
slope_view_new_for_figure (slope_figure_t *figure)
{
    GtkWidget *view = GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));
    SlopeViewPrivate *priv = SLOPE_VIEW_PRIVATE (view);
    priv->figure = figure;
    return view;
}


static gboolean
on_draw_event (GtkWidget *widget, cairo_t *cr, gpointer *data)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_PRIVATE (widget);
    int width, height;
    slope_rect_t rect;
    width = gtk_widget_get_allocated_width(widget);
    height = gtk_widget_get_allocated_height(widget);

    /* TODO: save figure in back_surf instead of recalculate
     * everithing for each zooming redraw
     */
    slope_rect_set(&rect, 0.0, 0.0, (double)width, (double)height);
    slope_figure_t *figure = priv->figure;
    slope_figure_draw(figure, cr, &rect);
        
    if (priv->on_move) {
        slope_cairo_set_color(cr, &priv->mouse_rec_color);
        cairo_set_line_width(cr, 1.0);
        double dash[2] = { 3.0, 3.0 };
        cairo_set_dash(cr, dash, 2, 0.0);
        cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
        cairo_rectangle(cr, priv->move_start.x, priv->move_start.y,
                        priv->move_end.x - priv->move_start.x,
                        priv->move_end.y - priv->move_start.y);
        cairo_stroke(cr);
    }
    return TRUE;
}


static gboolean on_button_press_event (GtkWidget *widget,
                                       GdkEventButton *event, gpointer *data)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_PRIVATE(widget);

    if (event->button == 1 /*left button*/) {
        priv->move_start.x = event->x;
        priv->move_start.y = event->y;
        priv->move_end.x = event->x;
        priv->move_end.y = event->y;
        priv->on_move = SLOPE_TRUE;
    }
    else if (event->button == 3 /*right button*/) {
        slope_figure_update(priv->figure);
        gtk_widget_queue_draw(widget);
    }
    return TRUE;
}


static gboolean on_button_move_event (GtkWidget *widget,
                                      GdkEventButton *event, gpointer *data)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_PRIVATE(widget);
    
    if (priv->on_move) {
        priv->move_end.x = event->x;
        priv->move_end.y = event->y;
        gtk_widget_queue_draw(widget);
    }
    return TRUE;
}


static gboolean on_button_release_event (GtkWidget *widget,
                                         GdkEventButton *event, gpointer *data)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_PRIVATE(widget);
    
    if (priv->on_move) {
        priv->on_move = SLOPE_FALSE;
        priv->move_end.x = event->x;
        priv->move_end.y = event->y;
        
        /* if the region is too small, the user probably just
           clicked on a point, no region to track */
        double width = priv->move_start.x - priv->move_end.x;
        double height = priv->move_start.y - priv->move_end.y;
        if (width < 0.0) width = -width;
        if (width < 3.0) return;
        if (height < 0.0) height = -height;
        if (height < 3.0) return;

        /* if a good region was selected, let's track it! */
        slope_figure_track_region(
            priv->figure,
            priv->move_start.x, priv->move_start.y,
            priv->move_end.x, priv->move_end.y);

        gtk_widget_queue_draw(widget);
    }
    return TRUE;
}

/* slope/view.c */
