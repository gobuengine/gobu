/**********************************************************************************
 * gapp_level_inspector.h                                                         *
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

#ifndef GAPP_LEVEL_EDITOR_INSPECTOR_H
#define GAPP_LEVEL_EDITOR_INSPECTOR_H
#include "gapp_level_editor.h"

G_BEGIN_DECLS

typedef struct _GappLevelInspectorPrivate GappLevelInspectorPrivate;

#define GAPP_LEVEL_TYPE_INSPECTOR (gapp_level_inspector_get_type())
G_DECLARE_FINAL_TYPE(GappLevelInspector, gapp_level_inspector, GAPP_LEVEL, INSPECTOR, GtkBox)

struct _GappLevelInspectorPrivate
{
    GtkWidget parent_instance;
};

GappLevelInspector* gapp_level_inspector_new(void);

G_END_DECLS

#endif // GAPP_LEVEL_EDITOR_INSPECTOR_H

