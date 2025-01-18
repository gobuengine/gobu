#include "gapp_project_manager.h"
#include "binn/binn_json.h"
#include "gobu/gobu.h"
#include "gapp_widget.h"
#include "gapp_editor.h"
#include "gapp.h"
#include "config.h"

#include "gapp_project_setting.h"

struct _GappProjectManager
{
    GtkBox parent_instance;

    GtkWidget *entry_name;
    GtkWidget *dresolutions;
    GtkWidget *btn_file_chooser;
    GtkWidget *dialog_btn_create;
    // custom size
    GtkWidget *customRes[2];
    GtkWidget *customResBox;
    //
    int selected_resolution;
    int resolution_width;
    int resolution_height;
};

static void gapp_project_manager_ui_dialog_new_project(GappProjectManager *self);
static void gapp_project_manager_ui_setup(GappProjectManager *self);
static void gapp_project_manager_dispose(GObject *object);

// MARK: --- BASE CLASS ---

G_DEFINE_TYPE(GappProjectManager, gapp_project_manager, GTK_TYPE_BOX)

static void gapp_project_manager_class_init(GappProjectManagerClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->dispose = gapp_project_manager_dispose;
}

static void gapp_project_manager_dispose(GObject *object)
{
    GappProjectManager *self = GAPP_PROJECT_MANAGER(object);

    g_clear_pointer(&self, gtk_widget_unparent);

    G_OBJECT_CLASS(gapp_project_manager_parent_class)->dispose(object);
}

static void gapp_project_manager_init(GappProjectManager *self)
{
    g_return_if_fail(GAPP_IS_PROJECT_MANAGER(self));

    // config inicial
    self->selected_resolution   = DEFAULT_RESOLUTION;
    self->resolution_width      = resolutions_values[self->selected_resolution * 2];
    self->resolution_height     = resolutions_values[self->selected_resolution * 2 + 1];

    gapp_project_manager_ui_setup(self);
}

// --- END CLASS ---

// MARK: PRIVATE FUNC

static binn *gapp_project_manager_fn_filter_and_save_valid_project_paths(binn *list)
{
    g_return_val_if_fail(list != NULL, NULL);

    binn *list_new = binn_list();
    if (!list_new)
    {
        g_critical("Failed to create new binn list");
        return NULL;
    }

    binn value;
    binn_iter iter;
    binn_list_foreach(list, value)
    {
        const char *item_path = binn_object_str(&value, "path");
        if (item_path != NULL)
        {
            g_autofree gchar *project_file = go_util_path_build(item_path, GAPP_PROJECT_GAME_FILE);
            if (project_file != NULL && go_util_path_exist(project_file))
            {
                if (binn_list_add_object(list_new, &value) == FALSE)
                {
                    g_warning("Failed to add item to new list");
                }
            }
        }
        else
        {
            g_warning("Found list item with NULL path");
        }
    }

    gchar *config_file = go_util_path_build(go_util_path_user(), "GOBU", GAPP_FILE_PROJECTS_NAME);
    binn_save_to_file(list_new, config_file);

    return list_new;
}

static binn *gapp_project_manager_fn_load_and_validate_user_projects(void)
{
    binn *list = NULL;
    g_autofree gchar *config_file = go_util_path_build(go_util_path_user(), "GOBU", GAPP_FILE_PROJECTS_NAME);

    if (go_util_path_exist(config_file))
    {
        list = binn_load_from_file(config_file);
        if (list == NULL)
        {
            g_warning("No se pudo cargar el archivo de proyectos: %s", config_file);
        }
    }
    else
    {
        list = binn_list();
        if (list == NULL)
        {
            g_warning("No se pudo crear una nueva lista binn");
        }
    }

    return gapp_project_manager_fn_filter_and_save_valid_project_paths(list);
}

static gboolean gapp_project_manager_fn_add_project_to_config(const gchar *path)
{
    g_return_val_if_fail(path != NULL && *path != '\0', FALSE);

    binn *list = binn_list();
    gboolean success = FALSE;
    g_autofree gchar *config_dir = go_util_path_build(go_util_path_user(), "GOBU");
    g_autofree gchar *config_file = go_util_path_build(config_dir, GAPP_FILE_PROJECTS_NAME);

    // Asegurar que el directorio de configuración existe
    if (!go_util_path_create(config_dir))
    {
        g_warning("No se pudo crear el directorio de configuración: %s", config_dir);
        goto cleanup;
    }

    // Cargar proyectos existentes si el archivo existe
    if (go_util_path_exist(config_file))
    {
        list = binn_load_from_file(config_file);
        if (list == NULL)
        {
            g_warning("No se pudo cargar el archivo de proyectos: %s", config_file);
            goto cleanup;
        }
    }

    // Añadir el nuevo proyecto
    binn *obj = binn_object();
    binn_object_set_str(obj, "path", g_strdup(path));
    binn_list_add_object(list, obj);
    binn_free(obj);

    // Guardar la lista actualizada
    if (!binn_save_to_file(list, config_file))
    {
        g_warning("No se pudo guardar el archivo de proyectos: %s", config_file);
        goto cleanup;
    }

    success = TRUE;

cleanup:
    binn_free(list);
    return success;
}

static gboolean gapp_project_manager_fn_initialize_game_project(const gchar *name, const gchar *path, GappProjectManager *self)
{
    g_return_val_if_fail(name != NULL && *name != '\0', FALSE);
    g_return_val_if_fail(path != NULL && *path != '\0', FALSE);

    g_autofree gchar *project_dir = go_util_path_build(path, name);

    if (!go_util_path_exist(project_dir))
    {
        g_autofree gchar *content_dir = go_util_path_build(project_dir, "resources");

        // Crear directorios
        if (go_util_path_create(project_dir) &&
            go_util_path_create(content_dir))
        {
            // WORLD + SCENE INIT
            go_ecs_init_c(name, self->resolution_width, self->resolution_height);
            go_ecs_scene_open(go_ecs_scene_new("Main"));
            go_ecs_save_to_file(go_util_path_build(project_dir, GAPP_PROJECT_GAME_FILE));
            // go_ecs_free();

            // registramos el proyecto en el archivo de configuración del usuario
            gapp_project_manager_fn_add_project_to_config(project_dir);

            return TRUE;
        }
        else
        {
            g_warning("No se pudieron crear los directorios del proyecto");
        }
    }
    else
    {
        g_warning("El directorio del proyecto ya existe: %s", project_dir);
    }

    return FALSE;
}

static gboolean gapp_project_manager_fn_launch_project_editor(const gchar *path, GappProjectManager *self)
{
    g_return_val_if_fail(path != NULL, FALSE);
    g_return_val_if_fail(GAPP_IS_PROJECT_MANAGER(self), FALSE);

    gapp_open_project(gapp_get_editor_instance(), path);

    g_debug("Editor main opened successfully for project: %s", path);
    return TRUE;
}

static gboolean gapp_project_manager_fn_is_valid_project_name(const char *project_name)
{
    if (project_name == NULL || *project_name == '\0')
    {
        return FALSE;
    }

    const char *valid_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-";
    return strspn(project_name, valid_chars) == strlen(project_name);
}

static void gapp_project_manager_fn_handle_project_location_selected(GtkFileDialog *source, GAsyncResult *res, GappProjectManager *self)
{
    g_return_if_fail(GTK_IS_FILE_DIALOG(source));
    g_return_if_fail(G_IS_ASYNC_RESULT(res));
    g_return_if_fail(GAPP_IS_PROJECT_MANAGER(self));

    GError *error = NULL;
    GFile *file = gtk_file_dialog_select_folder_finish(source, res, &error);

    if (error == NULL && file != NULL)
    {
        g_autofree gchar *path = g_file_get_path(file);
        if (path != NULL)
        {
            gtk_button_set_label(GTK_BUTTON(self->btn_file_chooser), path);
        }
        g_object_unref(file);
    }
    else if (error != NULL)
    {
        g_warning("Error al seleccionar la carpeta: %s", error->message);
        g_error_free(error);
    }
}

static void gapp_project_manager_fn_handle_project_open_selected(GObject *source, GAsyncResult *result, GappProjectManager *self)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    GFile *file = gtk_file_dialog_open_finish(dialog, result, NULL);
    if (file)
    {
        g_autofree gchar *filename = g_file_get_path(file);
        if (gapp_project_manager_fn_launch_project_editor(filename, self))
        {
            gapp_project_manager_fn_add_project_to_config(go_util_path_dirname(filename));
        }

        g_object_unref(file);
    }
}

static GtkStringList *gapp_project_manager_fn_create_project_paths_model(void)
{
    GtkStringList *sl = gtk_string_list_new(NULL);
    binn *list = gapp_project_manager_fn_load_and_validate_user_projects();

    if (list != NULL)
    {
        binn value;
        binn_iter iter;
        binn_list_foreach(list, value)
        {
            const char *item_path = binn_object_str(&value, "path");
            if (item_path != NULL)
            {
                g_autofree gchar *content_path = go_util_path_build(item_path, "Content");
                gtk_string_list_append(sl, content_path);
            }
        }
        binn_free(list);
    }

    return sl;
}

// MARK:SIGNAL

static void gapp_project_manager_on_item_factory_setup_item(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
    g_return_if_fail(GTK_IS_LIST_ITEM_FACTORY(factory));
    g_return_if_fail(GTK_IS_LIST_ITEM(list_item));

    GtkWidget *box, *imagen, *label;

    // Crear y configurar la caja contenedora
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_list_item_set_child(list_item, box);
    gtk_widget_set_size_request(box, 120, 120);

    // Crear y configurar la imagen
    imagen = gtk_image_new();
    gtk_image_set_icon_size(GTK_IMAGE(imagen), GTK_ICON_SIZE_LARGE);
    gtk_widget_set_vexpand(imagen, TRUE);
    gtk_box_append(GTK_BOX(box), imagen);

    // Crear y configurar la etiqueta
    label = gtk_label_new(NULL);
    gtk_label_set_wrap_mode(GTK_LABEL(label), PANGO_WRAP_CHAR);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_label_set_ellipsize(GTK_LABEL(label), PANGO_ELLIPSIZE_END);
    gtk_box_append(GTK_BOX(box), label);
}

static void gapp_project_manager_on_item_factory_bind_item(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
    g_return_if_fail(GTK_IS_LIST_ITEM(list_item));

    GtkWidget *box, *image, *label;

    // Obtener los widgets del ítem
    box = gtk_list_item_get_child(list_item);
    g_return_if_fail(GTK_IS_BOX(box));

    image = gtk_widget_get_first_child(box);
    g_return_if_fail(GTK_IS_IMAGE(image));

    label = gtk_widget_get_next_sibling(image);
    g_return_if_fail(GTK_IS_LABEL(label));

    // Obtener la ruta del proyecto
    GtkStringObject *obj = gtk_list_item_get_item(list_item);
    g_return_if_fail(GTK_IS_STRING_OBJECT(obj));
    const char *path_project = go_util_path_dirname(gtk_string_object_get_string(obj));

    // Configurar la imagen
    g_autofree gchar *thumbnail_path = go_util_path_build(path_project, GAPP_PROJECT_MANAGER_PREVIEW);
    if (go_util_path_exist(thumbnail_path))
    {
        gtk_image_set_from_file(GTK_IMAGE(image), thumbnail_path);
    }
    else
    {
        gtk_image_set_from_icon_name(GTK_IMAGE(image), GAPP_PROJECT_MANAGER_ICON);
    }

    // Configurar la etiqueta y el tooltip
    gtk_label_set_text(GTK_LABEL(label), go_util_path_basename(path_project));
    gtk_widget_set_tooltip_text(box, path_project);
}

static void gapp_project_manager_on_btn_file_chooser_clicked(GtkWidget *button, GappProjectManager *self)
{
    g_return_if_fail(GTK_IS_WIDGET(button));
    g_return_if_fail(GAPP_IS_PROJECT_MANAGER(self));

    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_select_folder(GTK_FILE_DIALOG(dialog),
                                  GTK_WINDOW(self),
                                  NULL,
                                  gapp_project_manager_fn_handle_project_location_selected,
                                  self);
}

static void gapp_project_manager_on_open_dialog_new_project_clicked(GtkWidget *button, GappProjectManager *self)
{
    g_return_if_fail(GTK_IS_WIDGET(button));
    g_return_if_fail(GAPP_IS_PROJECT_MANAGER(self));
    gapp_project_manager_ui_dialog_new_project(self);
}

static void gapp_project_manager_on_create_project_clicked(GtkWidget *button, GappProjectManager *self)
{
    g_return_if_fail(GTK_IS_WIDGET(button));
    g_return_if_fail(GAPP_IS_PROJECT_MANAGER(self));

    const gchar *name = gtk_editable_get_text(GTK_EDITABLE(self->entry_name));
    const gchar *path = gtk_button_get_label(GTK_BUTTON(self->btn_file_chooser));

    gboolean is_created = gapp_project_manager_fn_initialize_game_project(name, path, self);

    if (is_created)
        gapp_project_manager_fn_launch_project_editor(go_util_path_build(path, name, GAPP_PROJECT_MANAGER_FILE), self);
    else
    {
        g_warning("Falló la creación del proyecto");
        // TODO: Mostrar un mensaje de error al usuario
    }

    gtk_window_close(GTK_WINDOW(gtk_widget_get_ancestor(button, GTK_TYPE_WINDOW)));
}

static void gapp_project_manager_on_list_project_activated(GtkGridView *grid_view, guint position, GappProjectManager *self)
{
    GtkSelectionModel *select_model = gtk_grid_view_get_model(grid_view);
    g_return_if_fail(GTK_IS_SELECTION_MODEL(select_model));

    GtkStringObject *obj = g_list_model_get_item(select_model, position);

    g_return_if_fail(GTK_IS_STRING_OBJECT(obj));
    const char *path_project = go_util_path_dirname(gtk_string_object_get_string(obj));

    gapp_project_manager_fn_launch_project_editor(go_util_path_build(path_project, GAPP_PROJECT_MANAGER_FILE), self);
}

static void gapp_project_manager_on_open_other_project_clicked(GtkWidget *button, GappProjectManager *self)
{
    g_return_if_fail(GAPP_IS_PROJECT_MANAGER(self));

    GtkFileDialog *dialog = gtk_file_dialog_new();
    GtkRoot *root = gtk_widget_get_root(button);

    gtk_file_dialog_open(dialog,
                         GTK_WINDOW(root),
                         NULL,
                         gapp_project_manager_fn_handle_project_open_selected,
                         self);
}

static void gapp_project_manager_on_entry_name_changed(GtkWidget *entry, GappProjectManager *self)
{
    g_return_if_fail(GTK_IS_ENTRY(entry));
    g_return_if_fail(GAPP_IS_PROJECT_MANAGER(self));

    const char *name = gtk_editable_get_text(GTK_EDITABLE(self->entry_name));
    const char *path = gtk_button_get_label(GTK_BUTTON(self->btn_file_chooser));
    g_autofree gchar *project_dir = go_util_path_build(path, name);

    gboolean name_is_valid = gapp_project_manager_fn_is_valid_project_name(name);
    gboolean dir_not_exists = !go_util_path_exist(project_dir);
    gboolean is_valid = name_is_valid && dir_not_exists;

    gtk_widget_set_sensitive(self->dialog_btn_create, is_valid);

    // Proporcionar retroalimentación visual al usuario
    if (is_valid)
    {
        gtk_widget_remove_css_class(entry, "error");
        gtk_widget_add_css_class(entry, "success");
    }
    else
    {
        gtk_widget_remove_css_class(entry, "success");
        gtk_widget_add_css_class(entry, "error");

        // Depuración: identificar la razón específica del error
        if (!name_is_valid)
        {
            g_debug("Nombre de proyecto no válido: %s", name);
        }
        else if (!dir_not_exists)
        {
            g_debug("Ya existe un proyecto con el nombre: %s", name);
        }
    }

    g_debug("Nombre de proyecto: %s, Válido: %s", name, is_valid ? "Sí" : "No");
}

static void gapp_project_manager_on_activate_resolution(GtkDropDown *drop_down, GParamSpec *pspec, GappProjectManager *self)
{
    self->selected_resolution = gtk_drop_down_get_selected(drop_down);
    // seleccionamos la resolución
    self->resolution_width = resolutions_values[self->selected_resolution * 2];
    self->resolution_height = resolutions_values[self->selected_resolution * 2 + 1];
    // si seleccionamos el custom size, mostramos los spinButton
    gtk_widget_set_visible(self->customResBox, self->selected_resolution == CUSTOM_SIZE_KEY);
}

static void gapp_project_manager_on_activate_resolution_custom(GtkSpinButton *spin_button, GappProjectManager *self)
{
    guint index = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(spin_button), "index"));
    guint value = gtk_spin_button_get_value_as_int(spin_button);

    if (index == 0)
        self->resolution_width = value;
    else
        self->resolution_height = value;
}

// MARK: UI

static void gapp_project_manager_ui_dialog_new_project(GappProjectManager *self)
{
    GtkWidget *win, *vbox, *hbox, *label, *btn_create;

    g_autofree gchar *pathProjects = go_util_path_build(g_get_user_special_dir(G_USER_DIRECTORY_DOCUMENTS), "GobuProjects");

    GtkWidget *parent = gtk_widget_get_root(self);

    // Crear la ventana del diálogo
    win = gtk_window_new();
    gtk_window_set_modal(GTK_WINDOW(win), TRUE);
    gtk_window_set_title(GTK_WINDOW(win), "Create new project");
    gtk_window_set_default_size(GTK_WINDOW(win), 500, 200);
    gtk_window_set_resizable(GTK_WINDOW(win), FALSE);
    gtk_window_set_transient_for(GTK_WINDOW(win), GTK_WINDOW(parent));

    // Crear el contenedor principal
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(vbox, 15);
    gtk_widget_set_margin_end(vbox, 15);
    gtk_widget_set_margin_top(vbox, 15);
    gtk_widget_set_margin_bottom(vbox, 15);
    gtk_window_set_child(GTK_WINDOW(win), GTK_WIDGET(vbox));

    // Añadir campo para el nombre del proyecto
    label = gtk_label_new("Project Name:");
    gtk_label_set_xalign(GTK_LABEL(label), 0.0);
    gtk_box_append(GTK_BOX(vbox), GTK_WIDGET(label));

    self->entry_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(self->entry_name), "New Project");
    gtk_box_append(GTK_BOX(vbox), self->entry_name);
    g_signal_connect(self->entry_name, "changed", G_CALLBACK(gapp_project_manager_on_entry_name_changed), self);

    // Lista de resoluciones de pantalla
    label = gtk_label_new("Screen Resolution:");
    gtk_label_set_xalign(GTK_LABEL(label), 0.0);
    gtk_box_append(GTK_BOX(vbox), GTK_WIDGET(label));

    self->dresolutions = gtk_drop_down_new_from_strings(resolutions);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(self->dresolutions), self->selected_resolution);
    gtk_box_append(GTK_BOX(vbox), self->dresolutions);
    g_signal_connect(self->dresolutions, "notify::selected", G_CALLBACK(gapp_project_manager_on_activate_resolution), self);

    // Custom size
    self->customResBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_visible(self->customResBox, FALSE);
    gtk_box_append(GTK_BOX(vbox), self->customResBox);
    {
        for (int i = 0; i < 2; i++)
        {
            self->customRes[i] = gtk_spin_button_new_with_range(0, 9999, 1);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(self->customRes[i]), resolutions_values[CUSTOM_SIZE_SELECTED * 2 + i]);
            gtk_widget_set_hexpand(self->customRes[i], TRUE);
            g_object_set_data(G_OBJECT(self->customRes[i]), "index", GUINT_TO_POINTER(i));
            gtk_box_append(GTK_BOX(self->customResBox), self->customRes[i]);
            g_signal_connect(self->customRes[i], "value-changed", G_CALLBACK(gapp_project_manager_on_activate_resolution_custom), self);
        }
    }

    // Añadir campo para la ruta del proyecto
    label = gtk_label_new("Project Path:");
    gtk_label_set_xalign(GTK_LABEL(label), 0.0);
    gtk_box_append(GTK_BOX(vbox), GTK_WIDGET(label));

    self->btn_file_chooser = gtk_button_new_with_label(pathProjects);
    gtk_box_append(GTK_BOX(vbox), self->btn_file_chooser);
    g_signal_connect(self->btn_file_chooser, "clicked", G_CALLBACK(gapp_project_manager_on_btn_file_chooser_clicked), self);

    // Añadir botón de creación
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_margin_top(hbox, 15);
    gtk_widget_set_margin_bottom(hbox, 15);
    gtk_box_append(GTK_BOX(vbox), hbox);

    self->dialog_btn_create = gtk_button_new_with_label("Create");
    gtk_widget_set_sensitive(GTK_WIDGET(self->dialog_btn_create), FALSE);
    gtk_box_append(GTK_BOX(hbox), self->dialog_btn_create);
    gtk_widget_add_css_class(self->dialog_btn_create, "suggested-action");
    g_signal_connect(self->dialog_btn_create, "clicked", G_CALLBACK(gapp_project_manager_on_create_project_clicked), self);

    gtk_window_present(GTK_WINDOW(win));
}

static void gapp_project_manager_ui_setup(GappProjectManager *self)
{
    g_return_if_fail(GAPP_IS_PROJECT_MANAGER(self));

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_margin_top(hbox, 5);
    gtk_widget_set_margin_bottom(hbox, 5);
    gtk_widget_set_margin_end(hbox, 5);
    gtk_widget_set_halign(hbox, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(self), hbox);
    {
        GtkWidget *btn_new = gapp_widget_button_new_icon_with_label("window-new-symbolic", "New");
        gtk_button_set_has_frame(GTK_BUTTON(btn_new), TRUE);
        gtk_box_append(GTK_BOX(hbox), btn_new);
        g_signal_connect(btn_new, "clicked", G_CALLBACK(gapp_project_manager_on_open_dialog_new_project_clicked), self);

        GtkWidget *btn_open = gapp_widget_button_new_icon_with_label("document-open-symbolic", "Open other");
        gtk_button_set_has_frame(GTK_BUTTON(btn_open), TRUE);
        gtk_box_append(GTK_BOX(hbox), btn_open);
        g_signal_connect(btn_open, "clicked", G_CALLBACK(gapp_project_manager_on_open_other_project_clicked), self);
    }

    GtkWidget *search = gtk_search_entry_new();
    gapp_widget_set_margin(search, 5);
    gtk_box_append(GTK_BOX(self), search);
    { // GRID LIST ITEM PROJECTS
        GtkWidget *scroll = gtk_scrolled_window_new();
        gtk_box_append(GTK_BOX(self), scroll);
        {
            GtkStringList *sl = gapp_project_manager_fn_create_project_paths_model();

            GtkFilter *filter = GTK_FILTER(gtk_string_filter_new(gtk_property_expression_new(GTK_TYPE_STRING_OBJECT, NULL, "string")));
            GtkFilterListModel *filter_model = gtk_filter_list_model_new(G_LIST_MODEL(sl), filter);
            gtk_filter_list_model_set_incremental(filter_model, TRUE);
            g_object_bind_property(search, "text", filter, "search", 0);

            GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL(filter_model));

            GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
            g_signal_connect(factory, "setup", G_CALLBACK(gapp_project_manager_on_item_factory_setup_item), NULL);
            g_signal_connect(factory, "bind", G_CALLBACK(gapp_project_manager_on_item_factory_bind_item), NULL);

            GtkWidget *grid = gtk_grid_view_new(GTK_SELECTION_MODEL(selection), factory);
            gtk_widget_set_vexpand(grid, TRUE);
            gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), grid);
            g_signal_connect(grid, "activate", G_CALLBACK(gapp_project_manager_on_list_project_activated), self);
        }
    }
}

// --- MARK:BEGIN API ---
GappProjectManager *gapp_project_manager_new(void)
{
    return g_object_new(GAPP_TYPE_PROJECT_MANAGER, "orientation", GTK_ORIENTATION_VERTICAL, NULL);
}
