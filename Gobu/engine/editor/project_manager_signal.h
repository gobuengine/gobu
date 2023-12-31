/**********************************************************************************
 * project_manager_signal.h                                                       *
 **********************************************************************************
 * GOBU ENGINE                                                                    *
 * https://gobuengine.org                                                         *
 **********************************************************************************
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

#ifndef __GAPP_PROJECT_MANAGER_SIGNAL_H__
#define __GAPP_PROJECT_MANAGER_SIGNAL_H__
#include "project_manager.h"

void project_manager_signal_file_dialog_open_project(GtkWidget* widget, GobuProjectManager* ctx);
void project_manager_signal_create_project(GtkWidget* widget, GobuProjectManager* ctx);
void project_manager_signal_change_entry_check_name_project(GtkWidget* entry, GobuProjectManager* ctx);
void project_manager_signal_open_dialog_file_chooser_folder(GtkWidget* widget, GobuProjectManager* ctx);

#endif // __GAPP_PROJECT_MANAGER_SIGNAL_H__