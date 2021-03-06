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

/**
 * @file slope/figure.h
 * @defgroup Figure
 * @ingroup Figure
 * 
 * @author Elvis Teixeira
 * @date 18 Jan 2015
 * 
 * @brief Figure is the object that holds any of slope's drawables
 * 
 * Every time one wants to produce a figure one will instantiate a
 * slope_figure_t, add one or more slope_metrics_t to it and then
 * place the slope_item_t's in the metrics. All that remains to do
 * is draw the figure to any of cairo's backends, such as a GtkWidget
 * a PNG file or any other.
 */

#ifndef SLOPE_SCENE_H
#define SLOPE_SCENE_H

#include "slope/list.h"
#include "slope/primitives.h"

SLOPE_BEGIN_DECLS

/**
 * @ingroup Figure
 * @brief Creates a new figure object.
 * 
 * @returns A new figure instance.
 */
slope_public slope_figure_t* slope_figure_create();

/**
 * @ingroup Figure
 * @brief Retrieves the metrics list of figure.
 * 
 * @returns The list of metrics in this figure.
 */
slope_public slope_list_t*
slope_figure_get_metrics_list (const slope_figure_t *figure);

/**
 * @ingroup Figure
 * @brief Adds a metrics the metrics list of figure.
 */
slope_public void
slope_figure_add_metrics (slope_figure_t *figure,
                          slope_metrics_t *metrics);

/**
 * @ingroup Figure
 * @brief Destroys any figure object and frees the memory used by it.
 */
slope_public void
slope_figure_destroy (slope_figure_t *figure);

/**
 * @ingroup Figure
 * @brief Draw the contents of this figure to one of cairo's backends via cr.
 * 
 * @param[in] figure The figure to be drawn.
 * @param[in] cr The cairo context to draw the figure.
 * @param[in] rect The rectange that limits the area of the cairo_surface_t to draw figure.
 */
slope_public void
slope_figure_draw (slope_figure_t *figure, cairo_t *cr,
                   const slope_rect_t *rect);

/**
 * @ingroup Figure
 * @brief Writes the figure to a png file
 * 
 * @param[in] figure The figure to be drawn
 * @param[in] The figure to be drawn.
 * @param[in] filename The path to the png file to be output, include the .png ending.
 * @param[in] width The width in pixels of the output file.
 * @param[in] height The height in pixels of the output file.
 */
slope_public void
slope_figure_write_to_png (slope_figure_t *figure,
                          const char *filename,
                          int width, int height);

/**
 * @ingroup Figure
 * @brief The figure to be drawn.
 * 
 * @param[in] figure The figure to be drawn
 * @param[in] filename The path to the svg file to be output, include the .svg ending.
 * @param[in] width The width in pixels of the output file.
 * @param[in] height The height in pixels of the output file.
 * 
 * @return The status of the operation
 */
slope_public int
slope_figure_write_to_svg (slope_figure_t *figure,
                           const char *filename,
                           int width, int height);

/**
 * @ingroup Figure
 * @brief The figure to be drawn.
 * 
 * @param[in] figure The figure to be drawn
 * @param[in] filename The path to the pdf file to be output, include the .pdf ending.
 * @param[in] width The width in pixels of the output file.
 * @param[in] height The height in pixels of the output file.
 * 
 * @return The status of the operation
 */
slope_public int
slope_figure_write_to_pdf (slope_figure_t *figure,
                           const char *filename,
                           int width, int height);

/**
 * @ingroup Figure
 * @brief The figure to be drawn.
 * 
 * @param[in] figure The figure to be drawn
 * @param[in] filename The path to the postscript file to be output, include the .ps ending.
 * @param[in] width The width in pixels of the output file.
 * @param[in] height The height in pixels of the output file.
 * 
 * @return The status of the operation
 */
slope_public int
slope_figure_write_to_ps (slope_figure_t *figure,
                          const char *filename,
                          int width, int height);

/**
 * @ingroup Figure
 * @brief Retrieves the default metrics of the figure, normaly the last to be inserted.
 * It is the one that will place the legend
 * 
 * @param[in] figure The figure from wich to get the default metrics.
 * 
 * @return A pointer default metrics of figure.
 */
slope_public slope_metrics_t*
slope_figure_get_default_metrics (slope_figure_t *figure);

/**
 * @ingroup Figure
 * @brief Sets a callback to be called when some thing change on the figure,
 * e. g. useful to tell a widget to update it1s contents.
 * 
 * @param[in] figure The figure in which some thing changed
 * @param[in] callback A pointer to a function to be called when figure changes
 */
slope_public void
slope_figure_set_change_callback (slope_figure_t *figure,
                                  slope_callback_t callback);

/**
 */
slope_public void
slope_figure_notify_appearence_change (slope_figure_t *figure,
                                       slope_item_t *item);

/**
 */
slope_public void
slope_figure_notify_data_change (slope_figure_t *figure,
                                 slope_item_t *item);

/**
 */
slope_public void
slope_figure_track_region (slope_figure_t *figure,
                           double x1, double y1,
                           double x2, double y2);

/**
 */
slope_public void
slope_figure_update (slope_figure_t *figure);

SLOPE_END_DECLS

#endif /* SLOPE_SCENE_H */
