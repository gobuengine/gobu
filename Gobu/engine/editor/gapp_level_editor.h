/**********************************************************************************
 * gapp_level_editor.h                                                            *
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

#pragma once
#ifndef __GOBU_LEVEL_EDITOR_H__
#define __GOBU_LEVEL_EDITOR_H__
#include "gapp_gobu_embed.h"

G_BEGIN_DECLS

typedef struct _GappLevelEditorPrivate GappLevelEditorPrivate;

#define GAPP_LEVEL_TYPE_EDITOR (gapp_level_editor_get_type())
G_DECLARE_FINAL_TYPE(GappLevelEditor, gapp_level_editor, GAPP_LEVEL, EDITOR, GtkBox)

struct _GappLevelEditorPrivate
{
    GtkWidget parent_instance;

    GtkWidget* viewport;
    GtkWidget* inspector;
    GtkWidget* outliner;

    ecs_world_t* world; // world ecs
    ecs_entity_t root;  // entity root level
    gchar* filename;
};

GappLevelEditor* gapp_level_editor_new(const gchar* filename);
ecs_world_t *gapp_level_editor_get_world(GappLevelEditor *self);

G_END_DECLS

#endif // __GOBU_LEVEL_EDITOR_H__

