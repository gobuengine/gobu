/**********************************************************************************
 * gapp_level_outliner.h                                                            *
 * ********************************************************************************
 * GOBU ENGINE                                                                    *
 * https://gobuengine.org                                                         *
 * ********************************************************************************
 * Copyright (c) 2023 Jhonson Ozuna Mejia                                         *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 **********************************************************************************/

#ifndef __GAPP_LEVEL_OUTLINER_H__
#define __GAPP_LEVEL_OUTLINER_H__
#include <stdio.h>
#include <gtk/gtk.h>
#include "gapp_level_editor.h"

G_BEGIN_DECLS

// -------------------------
// BEGIN DEFINE TYPE
// ------------------------
typedef struct _ObjectOutlinerItem ObjectOutlinerItem;

#define OBJECT_TYPE_OUTLINER_ITEM (object_outliner_item_get_type())
G_DECLARE_FINAL_TYPE(ObjectOutlinerItem, object_outliner_item, OBJECT, OUTLINER_ITEM, GObject);

// -------------------------
// END DEFINE TYPE
// -------------------------

// -------------------------
// BEGIN DEFINE TYPE
// ------------------------

typedef struct _GappLevelOutlinerPrivate GappLevelOutlinerPrivate;

#define GAPP_LEVEL_TYPE_OUTLINER (gapp_level_outliner_get_type())
G_DECLARE_FINAL_TYPE(GappLevelOutliner, gapp_level_outliner, GAPP_LEVEL, OUTLINER, GtkBox)

struct _GappLevelOutlinerPrivate
{
    GtkWidget parent_instance;
};

// -------------------------
// END DEFINE TYPE
// -------------------------

ObjectOutlinerItem* object_outliner_item_new(ecs_world_t* world, ecs_entity_t entity);
// ObjectOutlinerItem* object_outliner_item_get_entity(GappLevelOutliner* self, ecs_entity_t entity);
// void object_outliner_item_append(ObjectOutlinerItem* parent, ObjectOutlinerItem* item);
// void object_outliner_item_remove(ObjectOutlinerItem* item);
// void object_outliner_item_set_parent(ObjectOutlinerItem* item, ObjectOutlinerItem* parent);

GappLevelOutliner* gapp_level_outliner_new(GappLevelEditor* editor);
void gapp_level_outliner_clear(GappLevelOutliner* self);
void gapp_level_outliner_events(GappLevelOutliner* self, ecs_entity_t event, ecs_entity_t entity, ecs_world_t* world);
// void gapp_level_outliner_init_root(GappLevelOutliner* self, ObjectOutlinerItem* item);
void gapp_level_outliner_init_root(GappLevelOutliner* self, ecs_entity_t entity);

void gapp_level_outliner_append_entity(GappLevelOutliner* self, ecs_entity_t entity);
void gapp_level_outliner_remove_entity(GappLevelOutliner* self, ecs_entity_t entity);

G_END_DECLS

#endif // __GAPP_LEVEL_OUTLINER_H__

