#include "gapp_widget.h"
#include "config.h"

/**
 * Muestra una alerta de widget en Gobu.
 *
 * @param parent   El widget padre al que se asociará la alerta.
 * @param message  El mensaje de la alerta a mostrar.
 */
void gapp_widget_alert(GtkWidget *parent, const gchar *message)
{
    GtkAlertDialog *alert = gtk_alert_dialog_new(message);
    gtk_alert_dialog_set_modal(alert, TRUE);
    gtk_alert_dialog_show(alert, parent);
}

/**
 * Muestra un diálogo de confirmación de eliminación en Gobu.
 *
 * @param parent     El widget padre al que se asociará el diálogo.
 * @param name       El nombre del elemento que se desea eliminar.
 * @param data       Datos adicionales relacionados con la eliminación.
 * @param c_handler  El manejador de callback a invocar cuando se confirma la eliminación.
 */
void gapp_widget_dialog_confirm_delete(GtkWidget *parent, const gchar *name, gpointer *data, GCallback c_handler)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Delete Assets", GTK_WINDOW(gtk_widget_get_root(parent)), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_USE_HEADER_BAR, "Delete", GTK_RESPONSE_OK, NULL);
    gtk_window_set_resizable(dialog, FALSE);

    GtkWidget *box = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gapp_widget_set_margin(GTK_WIDGET(box), 18);

    GtkWidget *label = gtk_label_new(g_strdup_printf("Esta seguro que desea eliminar <b>%s</b> ?", name));
    gtk_label_set_use_markup(label, TRUE);
    gtk_box_append(GTK_BOX(box), label);

    g_signal_connect(dialog, "response", G_CALLBACK(c_handler), data);

    gtk_window_present(dialog);
}

/**
 * Muestra un diálogo de entrada de texto en Gobu.
 *
 * @param parent         El widget padre al que se asociará el diálogo.
 * @param title          El título del diálogo.
 * @param text_default   El texto por defecto que se mostrará en el campo de entrada.
 *
 * @return Un widget que representa el diálogo de entrada de texto.
 */
static void dialog_input_close(GtkWidget *widget, GtkWindow *win)
{
    gtk_window_close(win);
}

static void dialog_input_update_button(GtkWidget *entry, GParamSpec *pspec, GtkWidget *button)
{
    const char *value = gtk_editable_get_text(GTK_EDITABLE(entry));
    gtk_widget_set_sensitive(button, value[0] != '\0');
}

GtkWidget *gapp_widget_dialog_input(GtkWidget *parent, const gchar *title, const gchar *label, const gchar *text_default)
{
    GtkWidget *win = gtk_window_new();
    gtk_window_set_modal(GTK_WINDOW(win), TRUE);
    gtk_window_set_title(GTK_WINDOW(win), title);
    gtk_window_set_default_size(GTK_WINDOW(win), 500, 100);
    gtk_window_set_resizable(GTK_WINDOW(win), FALSE);
    gtk_window_set_deletable(GTK_WINDOW(win), FALSE);
    gtk_window_set_transient_for(GTK_WINDOW(win), GTK_WINDOW(gtk_widget_get_root(parent)));

    GtkWidget *headerbar = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(win), headerbar);

    // Button::[Cancel, Crear]
    GtkWidget *btn_cancel = gtk_button_new_with_label("Cancel");
    gtk_header_bar_pack_start(headerbar, btn_cancel);
    g_signal_connect(btn_cancel, "clicked", G_CALLBACK(dialog_input_close), win);

    GtkWidget *btn_ok = gtk_button_new_with_label("Crear");
    gtk_header_bar_pack_end(headerbar, btn_ok);
    gtk_widget_set_sensitive(btn_ok, FALSE);
    g_signal_connect(btn_ok, "clicked", G_CALLBACK(dialog_input_close), win);

    // Content::
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gapp_widget_set_margin(GTK_WIDGET(box), 15);
    gtk_window_set_child(GTK_WINDOW(win), GTK_WIDGET(box));

    GtkWidget *wlabel = gtk_label_new(label);
    gtk_label_set_xalign(wlabel, 0);
    gtk_widget_set_hexpand(wlabel, TRUE);
    gapp_widget_set_margin_bottom(wlabel, 5);
    gtk_box_append(GTK_BOX(box), wlabel);

    GtkWidget *entry = gtk_entry_new_with_buffer(gtk_entry_buffer_new(text_default, -1));
    gtk_entry_set_placeholder_text(entry, label);
    gtk_box_append(GTK_BOX(box), entry);
    g_signal_connect(entry, "notify::text", G_CALLBACK(dialog_input_update_button), btn_ok);

    g_object_set_data(btn_ok, "entry", entry);

    gtk_window_present(win);

    return btn_ok;
}

/**
 * Obtiene el texto ingresado en un diálogo de entrada de texto en Gobu.
 *
 * @param dialog  El widget que representa el diálogo de entrada de texto.
 *
 * @return El texto ingresado en el diálogo de entrada de texto.
 */
const gchar *gapp_widget_dialog_input_get_text(GtkWidget *dialog)
{
    return gtk_editable_get_text(GTK_EDITABLE(g_object_get_data(G_OBJECT(dialog), "entry")));
}

/**
 * Establece el margen de un widget en Gobu.
 *
 * @param widget  El widget al que se aplicará el margen.
 * @param margin  El valor del margen que se desea establecer.
 */
void gapp_widget_set_margin(GtkWidget *widget, gint margin)
{
    gapp_widget_set_margin_start(widget, margin);
    gapp_widget_set_margin_end(widget, margin);
    gapp_widget_set_margin_top(widget, margin);
    gapp_widget_set_margin_bottom(widget, margin);
}

/**
 * Crea un nuevo widget de botón con icono y etiqueta en Gobu.
 *
 * @param icon_name  El nombre del icono a mostrar en el botón.
 * @param label      El texto de la etiqueta del botón.
 *
 * @return Un nuevo widget de botón con icono y etiqueta.
 */
GtkWidget *gapp_widget_button_new_icon_with_label(const gchar *icon_name, const gchar *label)
{
    GtkWidget *button = gtk_button_new();
    gtk_button_set_has_frame(GTK_BUTTON(button), FALSE);
    {
        GtkWidget *hbox_parent = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        gtk_button_set_child(GTK_BUTTON(button), hbox_parent);

        GtkWidget *icon = gtk_image_new_from_icon_name(icon_name);
        gtk_image_set_icon_size(GTK_IMAGE(icon), GTK_ICON_SIZE_INHERIT);
        gtk_box_append(GTK_BOX(hbox_parent), icon);

        if (label != NULL)
            gtk_box_append(GTK_BOX(hbox_parent), gtk_label_new(label));
    }

    return button;
}

/**
 * Crea un nuevo widget de botón de información en Gobu.
 *
 * @param title  El título de la información a mostrar en el botón.
 * @param desc   La descripción de la información a mostrar en el botón.
 *
 * @return Un nuevo widget de botón de información.
 */
GtkWidget *gapp_widget_button_new_info(const gchar *title, const gchar *desc)
{
    GtkWidget *button = gtk_button_new();
    gtk_button_set_has_frame(GTK_BUTTON(button), FALSE);
    {
        GtkWidget *hbox_parent = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        gtk_button_set_child(GTK_BUTTON(button), GTK_WIDGET(hbox_parent));

        GtkWidget *icon = gtk_image_new_from_icon_name("input-gaming");
        gtk_image_set_icon_size(GTK_IMAGE(icon), GTK_ICON_SIZE_LARGE);
        gtk_box_append(GTK_BOX(hbox_parent), GTK_WIDGET(icon));

        gtk_box_append(GTK_BOX(hbox_parent), gtk_separator_new(GTK_ORIENTATION_VERTICAL));

        GtkWidget *hbox_text = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
        gtk_box_append(GTK_BOX(hbox_parent), GTK_WIDGET(hbox_text));

        GtkWidget *label_title = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label_title), g_strdup_printf("<b><big>%s</big></b>", title));
        gtk_widget_set_halign(label_title, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(hbox_text), GTK_WIDGET(label_title));

        gtk_box_append(GTK_BOX(hbox_text), gtk_label_new(desc));
    }

    return button;
}

/**
 * Crea un nuevo widget de paneles (paned) en Gobu.
 *
 * @param orientation  La orientación de los paneles (GtkOrientation).
 * @param start        TRUE si se debe colocar el widget en el inicio, FALSE en caso contrario.
 *
 * @return Un nuevo widget de paneles (paned).
 */
GtkWidget *gapp_widget_paned_new(GtkOrientation orientation, gboolean start)
{
    GtkWidget *paned;

    paned = gtk_paned_new(orientation);
    if (start)
    {
        gtk_paned_set_resize_start_child(GTK_PANED(paned), FALSE);
        gtk_paned_set_shrink_start_child(GTK_PANED(paned), FALSE);
    }
    else
    {
        gtk_paned_set_resize_end_child(GTK_PANED(paned), FALSE);
        gtk_paned_set_shrink_end_child(GTK_PANED(paned), FALSE);
    }

    gtk_paned_set_wide_handle(GTK_PANED(paned), TRUE);

    if (orientation == GTK_ORIENTATION_VERTICAL)
        gtk_widget_set_vexpand(GTK_WIDGET(paned), TRUE);
    else if (orientation == GTK_ORIENTATION_HORIZONTAL)
        gtk_widget_set_hexpand(GTK_WIDGET(paned), TRUE);

    return paned;
}

/**
 * Crea un nuevo widget de paneles con pestañas en Gobu.
 *
 * @param orientation  La orientación de los paneles (GtkOrientation).
 * @param rink_start   TRUE si se deben mostrar las pestañas al inicio, FALSE en caso contrario.
 * @param label_start  El widget de etiqueta para el panel de inicio.
 * @param start        El contenido del panel de inicio.
 * @param label_end    El widget de etiqueta para el panel de fin.
 * @param end          El contenido del panel de fin.
 *
 * @return Un nuevo widget de paneles con pestañas.
 */
GtkWidget *gapp_widget_paned_new_with_notebook(GtkOrientation orientation, gboolean rink_start, GtkWidget *label_start, GtkWidget *start, GtkWidget *label_end, GtkWidget *end)
{
    GtkWidget *paned, *notebook;

    paned = gapp_widget_paned_new(orientation, rink_start);

    notebook = gtk_notebook_new();
    gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(notebook), DOCK_L_MIN_SIZE, DOCK_T_MIN_SIZE);
    gtk_paned_set_start_child(GTK_PANED(paned), notebook);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), start, label_start);

    if (label_end != NULL && end != NULL)
    {
        notebook = gtk_notebook_new();
        gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
        gtk_widget_set_size_request(GTK_WIDGET(notebook), DOCK_L_MIN_SIZE, DOCK_T_MIN_SIZE);
        gtk_paned_set_end_child(GTK_PANED(paned), notebook);

        gtk_notebook_append_page(GTK_NOTEBOOK(notebook), end, label_end);
    }

    return paned;
}

/**
 * Crea una nueva barra de herramientas en Gobu.
 *
 * @return Una nueva barra de herramientas.
 */
GtkWidget *gapp_widget_toolbar_new(void)
{
    GtkWidget *self;
    self = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_add_css_class(self, "toolbar");
    gapp_widget_set_margin(self, 3);
    return self;
}

/**
 * Agrega un separador en una barra de herramientas en Gobu.
 *
 * @param toolbar  La barra de herramientas a la que se agregará el separador.
 */
void gapp_widget_toolbar_separator_new(GtkWidget *toolbar)
{
    GtkWidget *self;
    self = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    // gapp_widget_set_margin_end(self, 2);
    // gapp_widget_set_margin_start(self, 2);
    gtk_box_append(GTK_BOX(toolbar), GTK_WIDGET(self));
}

/**
 * Crea un separador vertical en Gobu.
 *
 * @return Un nuevo separador vertical.
 */
GtkWidget *gapp_widget_separator_v(void)
{
    return gtk_separator_new(GTK_ORIENTATION_VERTICAL);
}

/**
 * Crea un separador horizontal en Gobu.
 *
 * @return Un nuevo separador horizontal.
 */
GtkWidget *gapp_widget_separator_h(void)
{
    return gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
}

/**
 * Crea un nuevo expansor con un widget interno en Gobu.
 *
 * @param title   El título del expansor.
 * @param widget  El widget interno que se mostrará u ocultará al expandir o contraer el expansor.
 *
 * @return Un nuevo widget que representa el expansor con el widget interno.
 */
GtkWidget *gapp_widget_expander_with_widget(const gchar *title, GtkWidget *widget)
{
    GtkWidget *box, *expand, *label;

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    {
        label = gtk_label_new(title);
        gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
        gtk_widget_set_hexpand(GTK_WIDGET(label), TRUE);
        gtk_widget_set_halign(GTK_WIDGET(label), GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(box), GTK_WIDGET(label));

        if (widget != NULL)
            gtk_box_append(GTK_BOX(box), GTK_WIDGET(widget));
    }

    expand = gtk_expander_new(NULL);
    {
        gtk_expander_set_label_widget(GTK_EXPANDER(expand), GTK_WIDGET(box));
        gtk_expander_set_expanded(GTK_EXPANDER(expand), TRUE);
        gtk_expander_set_use_markup(GTK_EXPANDER(expand), TRUE);
        gtk_widget_add_css_class(GTK_WIDGET(expand), "expander_n");
    }

    return expand;
}

void gapp_widget_entry_set_text(GtkEntry *entry, const gchar *text)
{
    gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(entry)), text, strlen(text));
}

const gchar *gapp_widget_entry_get_text(GtkEntry *entry)
{
    return gtk_entry_buffer_get_text(gtk_entry_get_buffer(entry));
}

void gapp_widget_view_set_text(GtkTextView *view, const gchar *text)
{
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(view), text, strlen(text));
}

gchar *gapp_widget_view_get_text(GtkTextBuffer *buffer)
{
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

GtkWidget *gapp_widget_input_number(GtkWidget *box, const gchar *label_input, double min, double max, double step)
{
    GtkWidget *spin_button, *label;

    GtkWidget *c_paned = gapp_widget_paned_new(GTK_ORIENTATION_HORIZONTAL, TRUE);
    gtk_paned_set_wide_handle(GTK_PANED(c_paned), FALSE);
    gtk_box_append(GTK_BOX(box), GTK_WIDGET(c_paned));
    {
        label = gtk_label_new(label_input);
        gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
        gapp_widget_set_margin(GTK_WIDGET(label), 5);
        gtk_paned_set_start_child(GTK_PANED(c_paned), GTK_WIDGET(label));

        spin_button = gtk_spin_button_new_with_range(min, max, step);
        gapp_widget_set_margin(GTK_WIDGET(spin_button), 5);
        gtk_paned_set_end_child(GTK_PANED(c_paned), GTK_WIDGET(spin_button));
        // gtk_size_group_add_widget(group_size, label);
    }

    return spin_button;
}

GtkWidget *gapp_widget_input_str(GtkWidget *box, const gchar *label_input, const gchar *default_str)
{
    GtkWidget *entry, *label;

    GtkWidget *c_paned = gapp_widget_paned_new(GTK_ORIENTATION_HORIZONTAL, TRUE);
    gtk_paned_set_wide_handle(GTK_PANED(c_paned), FALSE);
    gtk_box_append(GTK_BOX(box), GTK_WIDGET(c_paned));
    {
        label = gtk_label_new(label_input);
        gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
        gapp_widget_set_margin(GTK_WIDGET(label), 5);
        gtk_paned_set_start_child(GTK_PANED(c_paned), GTK_WIDGET(label));

        entry = gtk_entry_new();
        gapp_widget_set_margin(GTK_WIDGET(entry), 5);
        gtk_paned_set_end_child(GTK_PANED(c_paned), GTK_WIDGET(entry));
        gapp_widget_entry_set_text(GTK_ENTRY(entry), default_str);
        // gtk_size_group_add_widget(group_size, label);
    }

    return entry;
}
