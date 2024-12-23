// gapp_inspector_widgets.h
#ifndef GAPP_INSPECTOR_WIDGETS_H
#define GAPP_INSPECTOR_WIDGETS_H

#include "config.h"
#include "pixio/pixio.h"

void inspectorWidgetCreateComponentInputs(GtkWidget *content, ecs_world_t *world, void *ptr, ecs_entity_t component);

GtkWidget *inspectorWidgetCreateComponentGroup(GtkWidget *list, bool buttonRemove, const gchar *title_str, ecs_world_t *world, ecs_entity_t entity, ecs_entity_t component);

GtkWidget *inspectorWidgetCreateFieldRow(GtkWidget *size_group, const char *label_str, GtkWidget *input, GtkOrientation orientation);

#endif // GAPP_INSPECTOR_WIDGETS_H

