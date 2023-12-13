#include "gobu.h"
#include "thirdparty/raygo/rlgl.h"


// VERSION 2.0.0
// #ifdef GOBU_LIB_APP_IMPL
// #define SOKOL_NO_ENTRY
// #include "thirdparty/sokol/sokol_app.h"
// #endif
// // #ifdef GOBU_LIB_IMPL
// #include "thirdparty/sokol/sokol_gfx.h"
// // #include "thirdparty/sokol/sokol_log.h"
// #include "thirdparty/sokol/sokol_glue.h"
// #define SOKOL_GL_IMPL
// #include "thirdparty/sokol/sokol_gl.h"
// // #endif

// --------------------------------------------------------------
// Cree el archivo gobu-engine para implementar la idea
// de tener todo o casi todo en un solo lugar se que esto
// seria mas dificil de mantener y es una idea que loca.
// En una version futura se podria dividir en varios archivos.
// --------------------------------------------------------------

ECS_COMPONENT_DECLARE(gb_render_phases_t);
ECS_COMPONENT_DECLARE(gb_app_t);
ECS_COMPONENT_DECLARE(gb_color_t);
ECS_COMPONENT_DECLARE(gb_rect_t);
ECS_COMPONENT_DECLARE(gb_vec2_t);
ECS_COMPONENT_DECLARE(gb_vec3_t);
ECS_COMPONENT_DECLARE(gb_bounding_t);
ECS_COMPONENT_DECLARE(gb_transform_t);
ECS_COMPONENT_DECLARE(gb_info_t);
ECS_COMPONENT_DECLARE(gb_animated_t);
ECS_COMPONENT_DECLARE(gb_text_t);
ECS_COMPONENT_DECLARE(gb_sprite_t);
ECS_COMPONENT_DECLARE(gb_shape_rect_t);
ECS_COMPONENT_DECLARE(gb_shape_circle_t);
ECS_COMPONENT_DECLARE(gb_resource_t);
ECS_COMPONENT_DECLARE(gb_camera_t);
ECS_COMPONENT_DECLARE(gb_gizmos_t);
ECS_COMPONENT_DECLARE(enumCameraMode);

gb_render_phases_t render_phases;
gb_engine_t engine;

struct gb_setting_project_t {
    char* path;
    char* thumbnail;
    char* name;
    char* description;
    char* author;
    char* license;
    char* version;
    int width;
    int height;
    int mode;
    bool resizable;
    bool borderless;
    bool alwayontop;
    bool transparent;
    bool nofocus;
    bool highdpi;
    bool vsync;
    bool aspectration;

    // file project data
    GKeyFile* setting;
}project;


// ########################################
// Project functions
// ########################################
// Description de project functions:
//

static bool gb_setting_load(const char* filename);

bool gb_project_load(const char* filename)
{
    bool test = gb_fs_file_exist(filename);
    if (test)
    {
        project.path = gb_path_dirname(filename);

        char* file_gobuproject = gb_path_join(project.path, "game.gobuproject", NULL);

        gb_setting_load(file_gobuproject);

        g_free(file_gobuproject);
    }

    return test;
}

const char* gb_project_get_path(void)
{
    return gb_strdup(project.path);
}


// ########################################
// Project Setting functions
// ########################################
// Description de project-setting functions: Son funciones que se utilizarán en el código
// de Gobu para cargar y guardar la configuración del proyecto, como por ejemplo, el nombre
// del proyecto, la descripción, el autor, etc.
//
static bool gb_setting_load(const char* filename)
{
    project.setting = g_key_file_new();

    bool r = g_key_file_load_from_file(project.setting, filename, G_KEY_FILE_NONE, NULL);
    if (r)
    {
        project.name = g_key_file_get_string(project.setting, "project", "name", NULL);
        project.description = g_key_file_get_string(project.setting, "project", "description", NULL);
        project.author = g_key_file_get_string(project.setting, "project", "author", NULL);

        project.width = g_key_file_get_integer(project.setting, "display.resolution", "width", NULL);
        project.height = g_key_file_get_integer(project.setting, "display.resolution", "height", NULL);
        project.mode = g_key_file_get_integer(project.setting, "display.resolution", "mode", NULL);
        project.resizable = g_key_file_get_boolean(project.setting, "display.window", "resizable", NULL);
        project.borderless = g_key_file_get_boolean(project.setting, "display.window", "borderless", NULL);
        project.alwayontop = g_key_file_get_boolean(project.setting, "display.window", "alwayontop", NULL);
        project.transparent = g_key_file_get_boolean(project.setting, "display.window", "transparent", NULL);
        project.nofocus = g_key_file_get_boolean(project.setting, "display.window", "nofocus", NULL);
        project.highdpi = g_key_file_get_boolean(project.setting, "display.window", "highdpi", NULL);
        project.vsync = g_key_file_get_boolean(project.setting, "display.window", "vsync", NULL);
        project.aspectration = g_key_file_get_boolean(project.setting, "display.window", "aspectration", NULL);
    }

    return r;
}

GKeyFile* gb_setting_project_setting(void)
{
    return project.setting;
}

void gb_setting_unload(void)
{
    g_free(project.path);
    g_free(project.name);
    g_free(project.description);
    g_free(project.author);
    g_free(project.license);
    g_free(project.version);
    g_free(project.thumbnail);
    g_key_file_free(project.setting);
}

void gb_setting_save(void)
{
    char* file_gobuproject = gb_path_join(project.path, "game.gobuproject", NULL);
    g_key_file_save_to_file(project.setting, file_gobuproject, NULL);
    g_free(file_gobuproject);
}

void gb_setting_project_set_name(const char* name)
{
    g_key_file_set_string(project.setting, "project", "name", name);
    project.name = g_key_file_get_string(project.setting, "project", "name", NULL);
}

void gb_setting_project_set_description(const char* description)
{
    g_key_file_set_string(project.setting, "project", "description", description);
    project.description = g_key_file_get_string(project.setting, "project", "description", NULL);
}

void gb_setting_project_set_author(const char* author)
{
    g_key_file_set_string(project.setting, "project", "author", author);
    project.author = g_key_file_get_string(project.setting, "project", "author", NULL);
}

void gb_setting_project_set_license(const char* license)
{
    g_key_file_set_string(project.setting, "project", "license", license);
    project.license = g_key_file_get_string(project.setting, "project", "license", NULL);
}

void gb_setting_project_set_version(const char* version)
{
    g_key_file_set_string(project.setting, "project", "version", version);
    project.version = g_key_file_get_string(project.setting, "project", "version", NULL);
}

void gb_setting_project_set_thumbnail(const char* thumbnail)
{
    g_key_file_set_string(project.setting, "project", "thumbnail", thumbnail);
    project.thumbnail = g_key_file_get_string(project.setting, "project", "thumbnail", NULL);
}

void gb_setting_project_set_width(int width)
{
    g_key_file_set_integer(project.setting, "display.resolution", "width", width);
    project.width = g_key_file_get_integer(project.setting, "display.resolution", "width", NULL);
}

void gb_setting_project_set_height(int height)
{
    g_key_file_set_integer(project.setting, "display.resolution", "height", height);
    project.height = g_key_file_get_integer(project.setting, "display.resolution", "height", NULL);
}

void gb_setting_project_set_mode(int mode)
{
    g_key_file_set_integer(project.setting, "display.window", "mode", mode);
    project.mode = g_key_file_get_integer(project.setting, "display.window", "mode", NULL);
}

void gb_setting_project_set_resizable(bool resizable)
{
    g_key_file_set_boolean(project.setting, "display.window", "resizable", resizable);
    project.resizable = g_key_file_get_boolean(project.setting, "display.window", "resizable", NULL);
}

void gb_setting_project_set_borderless(bool borderless)
{
    g_key_file_set_boolean(project.setting, "display.window", "borderless", borderless);
    project.borderless = g_key_file_get_boolean(project.setting, "display.window", "borderless", NULL);
}

void gb_setting_project_set_alwayontop(bool alwayontop)
{
    g_key_file_set_boolean(project.setting, "display.window", "alwayontop", alwayontop);
    project.alwayontop = g_key_file_get_boolean(project.setting, "display.window", "alwayontop", NULL);
}

void gb_setting_project_set_transparent(bool transparent)
{
    g_key_file_set_boolean(project.setting, "display.window", "transparent", transparent);
    project.transparent = g_key_file_get_boolean(project.setting, "display.window", "transparent", NULL);
}

void gb_setting_project_set_nofocus(bool nofocus)
{
    g_key_file_set_boolean(project.setting, "display.window", "nofocus", nofocus);
    project.nofocus = g_key_file_get_boolean(project.setting, "display.window", "nofocus", NULL);
}

void gb_setting_project_set_highdpi(bool highdpi)
{
    g_key_file_set_boolean(project.setting, "display.window", "highdpi", highdpi);
    project.highdpi = g_key_file_get_boolean(project.setting, "display.window", "highdpi", NULL);
}

void gb_setting_project_set_vsync(bool vsync)
{
    g_key_file_set_boolean(project.setting, "display.window", "vsync", vsync);
    project.vsync = g_key_file_get_boolean(project.setting, "display.window", "vsync", NULL);
}

void gb_setting_project_set_aspectration(bool aspectration)
{
    g_key_file_set_boolean(project.setting, "display.window", "aspectration", aspectration);
    project.aspectration = g_key_file_get_boolean(project.setting, "display.window", "aspectration", NULL);
}

const char* gb_setting_project_name(void)
{
    return gb_strdup(project.name);
}

const char* gb_setting_project_description(void)
{
    return project.description == NULL ? "" : gb_strdup(project.description);
}

const char* gb_setting_project_author(void)
{
    return project.author == NULL ? "" : gb_strdup(project.author);
}

const char* gb_setting_project_license(void)
{
    return project.license == NULL ? "" : gb_strdup(project.license);
}

const char* gb_setting_project_version(void)
{
    return project.version == NULL ? "0.0.1" : gb_strdup(project.version);
}

const char* gb_setting_project_thumbnail(void)
{
    return project.thumbnail == NULL ? "" : gb_strdup(project.thumbnail);
}

int gb_setting_project_width(void)
{
    return project.width;
}

int gb_setting_project_height(void)
{
    return project.height;
}

int gb_setting_project_mode(void)
{
    return project.mode;
}

bool gb_setting_project_resizable(void)
{
    return project.resizable;
}

bool gb_setting_project_borderless(void)
{
    return project.borderless;
}

bool gb_setting_project_alwayontop(void)
{
    return project.alwayontop;
}

bool gb_setting_project_transparent(void)
{
    return project.transparent;
}

bool gb_setting_project_nofocus(void)
{
    return project.nofocus;
}

bool gb_setting_project_highdpi(void)
{
    return project.highdpi;
}

bool gb_setting_project_vsync(void)
{
    return project.vsync;
}

bool gb_setting_project_aspectration(void)
{
    return project.aspectration;
}


// ########################################
// Utility functions
// ########################################
// Description de utility functions: Son funciones que se utilizarán en el código
// de Gobu para realizar tareas específicas, como por ejemplo, obtener el nombre
// de un archivo, verificar si existe, etc.
//

/**
 * @brief Normaliza una ruta de archivo.
 *
 * Esta función toma una ruta de archivo y la normaliza, eliminando cualquier
 * caracter especial o redundante. Devuelve un puntero a la ruta de archivo
 * normalizada.
 *
 * @param path La ruta de archivo a normalizar.
 * @return Un puntero a la ruta de archivo normalizada.
 */
char* gb_path_normalize(const char* path)
{
    return gb_str_replace(path, "\\", "/");
}

/**
 * @brief Combina múltiples rutas en una sola ruta.
 *
 * Esta función toma una ruta inicial y una lista variable de rutas adicionales y las combina en una sola ruta.
 *
 * @param first_path La ruta inicial.
 * @param ... Las rutas adicionales a combinar.
 *
 * @return La ruta combinada.
 */
char* gb_path_join(const char* first_path, ...)
{
    gchar* buffer;
    va_list args;

    va_start(args, first_path);
    buffer = g_build_filename_valist(first_path, &args);
    va_end(args);

    return buffer;
}

/**
 * @brief Obtiene el nombre base de una ruta de archivo.
 *
 * Esta función toma una ruta de archivo como entrada y devuelve el nombre base de la ruta.
 * El nombre base es el último componente de la ruta, sin la extensión del archivo.
 *
 * @param filename La ruta de archivo de la cual se desea obtener el nombre base.
 * @return El nombre base de la ruta de archivo.
 */
char* gb_path_basename(const char* filename)
{
    return g_path_get_basename(filename);
}

/**
 * @brief Retorna el directorio de la ruta de un archivo.
 *
 * Esta función toma una cadena de caracteres que representa la ruta de un archivo y devuelve
 * el directorio de esa ruta. El directorio se determina eliminando el nombre del archivo de la ruta.
 *
 * @param filename La ruta del archivo.
 * @return El directorio de la ruta del archivo.
 */
char* gb_path_dirname(const char* filename)
{
    return g_path_get_dirname(filename);
}

/**
 * Retorna la ruta del directorio actual.
 *
 * @return Un puntero a una cadena que representa la ruta del directorio actual.
 */
char* gb_path_current_dir(void)
{
    return g_get_current_dir();
}

/**
 * @brief Verifica si existe un folder en la ruta especificada.
 *
 * @param filename El nombre del folder a verificar.
 * @return true si el folder existe, false en caso contrario.
 */
bool gb_path_exist(const char* filename)
{
    return gb_fs_test(filename, G_FS_TEST_IS_DIR);
}

/**
 * @brief Copia un archivo desde la ruta de origen a la ruta de destino.
 *
 * Esta función copia el contenido del archivo de origen al archivo de destino.
 *
 * @param src El archivo de origen que se copiará.
 * @param dest El archivo de destino donde se copiará el contenido.
 */
bool gb_fs_copyc(GFile* src, GFile* dest, GError** error)
{
    if (gb_path_exist(g_file_get_path(src))) {
        if (gb_fs_mkdir(g_file_get_path(dest))) {
            GFileEnumerator* enumerator = g_file_enumerate_children(src, G_FILE_ATTRIBUTE_STANDARD_NAME, G_FILE_QUERY_INFO_NONE, NULL, NULL);
            GFileInfo* file_info = NULL;
            while ((file_info = g_file_enumerator_next_file(enumerator, NULL, NULL)) != NULL)
            {
                const gchar* file_name = g_file_info_get_name(file_info);
                GFile* file_src = g_file_new_for_path(gb_path_join(g_file_get_path(src), file_name, NULL));
                GFile* file_dest = g_file_new_for_path(gb_path_join(g_file_get_path(dest), file_name, NULL));
                gb_fs_copyc(file_src, file_dest, error);
            }
        }
        return TRUE;
    }

    return g_file_copy(src, dest, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, error);
}

/**
 * @brief Escribe el contenido en un archivo.
 *
 * Esta función escribe el contenido proporcionado en un archivo con el nombre especificado.
 *
 * @param filename El nombre del archivo en el que se va a escribir.
 * @param contents El contenido que se va a escribir en el archivo.
 * @return true si se pudo escribir el archivo correctamente, false en caso contrario.
 */
bool gb_fs_write_file(const char* filename, const char* contents)
{
    return g_file_set_contents(filename, contents, strlen(contents), NULL);
}

/**
 * @brief Crea un directorio con la ruta especificada.
 *
 * @param pathname La ruta del directorio a crear.
 * @return true si el directorio se creó correctamente, false en caso contrario.
 */
bool gb_fs_mkdir(const char* pathname)
{
    return g_mkdir_with_parents(pathname, 0) == -1 ? false : true;
}

/**
 * @brief Verifica si un archivo existe en el sistema de archivos.
 *
 * Esta función comprueba si un archivo con el nombre especificado existe en el sistema de archivos.
 *
 * @param filename El nombre del archivo a verificar.
 * @return true si el archivo existe, false en caso contrario.
 */
bool gb_fs_file_exist(const char* filename)
{
    return gb_fs_test(filename, G_FS_TEST_EXISTS);
}

/**
 * @brief Prueba el sistema de archivos para el nombre de archivo dado utilizando la prueba especificada.
 *
 * @param filename El nombre del archivo a probar.
 * @param test El tipo de prueba a realizar en el sistema de archivos.
 * @return true si la prueba es exitosa, false en caso contrario.
 */
bool gb_fs_test(const char* filename, GFSTest test)
{
    return g_file_test(filename, (GFileTest)test);
}

/**
 * @brief Obtiene la extensión de un nombre de archivo.
 *
 * Esta función toma un nombre de archivo como entrada y devuelve la extensión del archivo.
 *
 * @param filename El nombre del archivo del cual se desea obtener la extensión.
 * @return La extensión del archivo.
 */
const char* gb_fs_extname(const char* filename)
{
    if (filename[0] == '.') return filename;
    const char* dot = strrchr(filename, '.');
    if (!dot || dot == filename) return NULL;
    return dot;
}

/**
 * @brief Obtiene el nombre de un archivo a partir de su ruta.
 *
 * Esta función toma una ruta de archivo y devuelve el nombre del archivo sin la extensión.
 *
 * @param filename La ruta del archivo.
 * @param with_ext Indica si se debe incluir la extensión en el nombre del archivo.
 * @return El nombre del archivo sin la extensión, si with_ext es false.
 *         Si with_ext es true, devuelve el nombre del archivo con la extensión.
 */
char* gb_fs_get_name(const char* filename, bool with_ext)
{
    gchar* basename = gb_path_basename(filename);
    if (with_ext) {
        gchar** sep_name = gb_str_split(basename, ".");
        gchar* name = gb_strdup(sep_name[0]);
        gb_str_split_free(sep_name);
        return name;
    }
    return basename;
}

/**
 * @brief Verifica si la extensión de un archivo coincide con una extensión dada.
 *
 * @param filename El nombre del archivo.
 * @param ext La extensión a comparar.
 * @return true si la extensión del archivo coincide con la extensión dada, false en caso contrario.
 */

bool gb_fs_is_extension(const char* filename, const char* ext)
{
    return (g_strcmp0(gb_fs_extname(filename), ext) == 0);
}

/**
 * @brief Obtiene el contenido de un archivo.
 *
 * Esta función lee el contenido de un archivo y lo devuelve como una cadena de caracteres.
 *
 * @param filename El nombre del archivo a leer.
 * @param length La longitud máxima del contenido a leer.
 * @return Un puntero a la cadena de caracteres que contiene el contenido del archivo.
 */
gchar* gb_fs_get_contents(const char* filename, gsize* length)
{
    GError* error = NULL;
    gchar* contents = NULL;
    *length = 0;

    if (g_file_get_contents(filename, &contents, length, &error) == FALSE)
    {
        g_error("Error reading file: %s", error->message);
        g_error_free(error);
    }
    return contents;
}

/**
 * @brief Duplica una cadena de texto con formato opcional.
 *
 * Esta función duplica una cadena de texto con formato opcional. Toma una cadena de formato y argumentos adicionales,
 * similares a la función printf, y devuelve una cadena recién asignada que es una duplicación de la cadena formateada.
 *
 * @param format La cadena de formato.
 * @param ... Argumentos adicionales para el formato.
 * @return Una cadena recién asignada que es una duplicación de la cadena formateada.
 */
char* gb_strdups(const char* format, ...)
{
    gchar* buffer;
    va_list args;

    va_start(args, format);
    buffer = g_strdup_vprintf(format, args);
    va_end(args);

    return buffer;
}

/**
 * @brief Duplica una cadena de texto.
 *
 * Esta función crea una copia de la cadena de texto de entrada y devuelve un puntero a la memoria recién asignada.
 *
 * @param str La cadena de texto a duplicar.
 * @return Un puntero a la cadena de texto duplicada, o NULL si falla la asignación de memoria.
 */
char* gb_strdup(const char* str)
{
    return g_strdup(str);
}

/**
 * Convierte una cadena de texto a minúsculas.
 *
 * @param text La cadena de texto a convertir.
 * @return La cadena de texto convertida en minúsculas.
 */
char* gb_str_tolower(const char* text)
{
    return g_utf8_strdown(text, strlen(text));
}

/**
 * Convierte una cadena a mayúsculas.
 *
 * @param text La cadena que se va a convertir.
 * @return La cadena convertida en mayúsculas.
 */
char* gb_str_uppercase(const char* text)
{
    return g_utf8_strup(text, strlen(text));
}

/**
 * @brief Compara dos cadenas de texto y devuelve verdadero si son iguales.
 *
 * @param text1 La primera cadena de texto a comparar.
 * @param text2 La segunda cadena de texto a comparar.
 * @return true si las cadenas de texto son iguales, false en caso contrario.
 */
bool gb_str_isequal(const char* text1, const char* text2)
{
    bool result = false;

    if ((text1 != NULL) && (text2 != NULL))
    {
        if (strcmp(text1, text2) == 0) result = true;
    }

    return result;
}

/**
 * Divide una cadena en un array de subcadenas basado en un delimitador.
 *
 * @param string La cadena a dividir.
 * @param delimiter El delimitador utilizado para dividir la cadena.
 * @return Un array de subcadenas.
 */
char** gb_str_split(const char* string, const char* delimiter)
{
    return g_strsplit(string, delimiter, -1);
}

/**
 * @brief Libera la memoria asignada a un array de cadenas de caracteres.
 *
 * @param str_array El array de cadenas de caracteres a liberar.
 */
void gb_str_split_free(char** str_array)
{
    g_strfreev(str_array);
}

/**
 * @brief Elimina los espacios en blanco al principio y al final de una cadena de caracteres.
 *
 * @param str La cadena de caracteres a recortar.
 * @return Un puntero a la cadena de caracteres recortada.
 */
char* gb_str_trim(char* str)
{
    return g_strstrip(str);
}

/**
 * @brief Elimina los espacios en blanco de una cadena de caracteres.
 *
 * @param str La cadena de caracteres de la cual se eliminarán los espacios en blanco.
 * @return Un puntero a la cadena de caracteres resultante sin espacios en blanco.
 */
char* gb_str_remove_spaces(char* str)
{
    return gb_str_replace(g_strdup(str), " ", "");
}

/**
 * @brief Sanitiza una cadena eliminando cualquier carácter especial o símbolo.
 *
 * @param str La cadena que se va a sanitizar.
 * @return Un puntero a la cadena sanitizada.
 */
char* gb_str_sanitize(char* str)
{
    return gb_str_replace(g_strdup(str), G_STR_DELIMITERS, "");
}

/**
 * Reemplaza todas las apariciones de una subcadena en una cadena con otra subcadena.
 *
 * @param str La cadena original.
 * @param find La subcadena que se va a reemplazar.
 * @param replace La subcadena para reemplazar las apariciones de 'find'.
 * @return Una nueva cadena con todas las apariciones de 'find' reemplazadas por 'replace'.
 */
char* gb_str_replace(const char* str, const char* find, const char* replace)
{
    char** split = g_strsplit_set(str, find, -1);
    char* result = g_strjoinv(replace, split);
    g_strfreev(split);
    return result;
}



// ########################################
// gblib functions
// ########################################
//

void gb_gfx_begin_lines(void) { rlBegin(RL_LINES); }
void gb_gfx_end() { rlEnd(); }
void gb_gfx_color4ub(gb_color_t color) { rlColor4ub(color.r, color.g, color.b, color.a); }
void gb_gfx_vertex2f(float x, float y) { rlVertex2f(x, y); }
void gb_gfx_vertex3f(float x, float y, float z) { rlVertex3f(x, y, z); }
void gb_gfx_translatef(float x, float y, float z) { rlTranslatef(x, y, z); }
void gb_gfx_rotatef(float angle, float x, float y, float z) { rlRotatef(angle, x, y, z); }
void gb_gfx_scalef(float x, float y, float z) { rlScalef(x, y, z); }
void gb_gfx_push_matrix(void) { rlPushMatrix(); }
void gb_gfx_pop_matrix(void) { rlPopMatrix(); }

void gb_gfx_init() {}
void gb_gfx_close() {}


// ########################################
// Rendering functions
// ########################################
//


/**
 * @brief Dibuja una cuadrícula 2D en la pantalla.
 *
 * Esta función dibuja una cuadrícula 2D en la pantalla utilizando el número de divisiones y el espaciado especificados.
 * Los colores de la cuadrícula se pueden personalizar utilizando los parámetros de color1 y color2.
 *
 * @param slices El número de divisiones de la cuadrícula.
 * @param spacing El espaciado entre las líneas de la cuadrícula.
 * @param color1 El color de las líneas de la cuadrícula.
 * @param color2 El color alternativo de las líneas de la cuadrícula.
 */
void gb_rendering_draw_grid_2d(int slices, float spacing, gb_color_t color)
{
    int halfSlices = slices / 2;

    gb_gfx_begin_lines();
    for (int i = -halfSlices; i <= halfSlices; i++)
    {
        gb_gfx_color4ub(color);
        gb_gfx_color4ub(color);
        gb_gfx_color4ub(color);
        gb_gfx_color4ub(color);

        gb_gfx_vertex2f((float)i * spacing, (float)-halfSlices * spacing);
        gb_gfx_vertex2f((float)i * spacing, (float)halfSlices * spacing);

        gb_gfx_vertex2f((float)-halfSlices * spacing, (float)i * spacing);
        gb_gfx_vertex2f((float)halfSlices * spacing, (float)i * spacing);
    }
    gb_gfx_end();
}

void gb_rendering_draw_gismos(gb_transform_t transform, gb_bounding_t bonding_box)
{
    Rectangle bonding = (Rectangle){ 0.0f, 0.0f, bonding_box.max.x, bonding_box.max.y };

    gb_gfx_push_matrix();
    {
        gb_gfx_translatef(transform.position.x, transform.position.y, transform.position.z);
        gb_gfx_rotatef(transform.rotation.x, 0.0f, 0.0f, 1.0f);
        gb_gfx_rotatef(transform.rotation.y, 0.0f, 1.0f, 0.0f);
        gb_gfx_rotatef(transform.rotation.z, 1.0f, 0.0f, 0.0f);
        gb_gfx_translatef(-transform.origin.x, -transform.origin.y, 0.0f);
        gb_gfx_scalef(transform.scale.x, transform.scale.y, 1.0f);

        DrawRectangleLinesEx(bonding, 2, SKYBLUE);

        // creamos 4 rectangulos en cada esquina del bounding box para cambiar el tamaño de la entidad
        Rectangle rect1 = (Rectangle){ bonding.x - 4, bonding.y - 4, 8, 8 };
        Rectangle rect2 = (Rectangle){ bonding.x - 4, bonding.y + bonding.height - 4, 8, 8 };
        Rectangle rect3 = (Rectangle){ bonding.x + bonding.width - 4, bonding.y - 4, 8, 8 };
        Rectangle rect4 = (Rectangle){ bonding.x + bonding.width - 4, bonding.y + bonding.height - 4, 8, 8 };

        DrawRectangleRec(rect1, SKYBLUE);
        DrawRectangleRec(rect2, SKYBLUE);
        DrawRectangleRec(rect3, SKYBLUE);
        DrawRectangleRec(rect4, SKYBLUE);

        // En cada rectangulo dibujamos un cuadrado de color blanco para poder verlo
        DrawRectangle(rect1.x + 2.5, rect1.y + 2.5, 4, 4, WHITE);
        DrawRectangle(rect2.x + 2.5, rect2.y + 2.5, 4, 4, WHITE);
        DrawRectangle(rect3.x + 2.5, rect3.y + 2.5, 4, 4, WHITE);
        DrawRectangle(rect4.x + 2.5, rect4.y + 2.5, 4, 4, WHITE);

        // Creamos un círculo de línea en el centro del bounding box para rotar la entidad seleccionada
        Vector2 center = (Vector2){ bonding.x + bonding.width / 2, (bonding.y + bonding.height / 2) + (bonding.height / 2 + 20) };
        DrawCircle(center.x, center.y, 5, WHITE);
        DrawCircleLines(center.x, center.y, 5, BLACK);
    }
    gb_gfx_pop_matrix();
}

/**
 * Función que dibuja un rectángulo en la pantalla.
 *
 * @param rect El rectángulo a dibujar.
 */
void gb_rendering_draw_rect(gb_shape_rect_t rect)
{
    Color color = (Color){ rect.color.r, rect.color.g, rect.color.b, rect.color.a };
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
}

/**
 * Función que dibuja un círculo en la pantalla.
 *
 * @param circle El círculo a dibujar.
 */
void gb_rendering_draw_circle(gb_shape_circle_t circle)
{
    Color color = (Color){ circle.color.r, circle.color.g, circle.color.b, circle.color.a };
    DrawCircle(circle.x, circle.y, circle.radius, color);
}

/**
 * Función que dibuja un sprite en la pantalla.
 *
 * @param sprite El sprite a dibujar.
 */
void gb_rendering_draw_sprite(gb_sprite_t sprite)
{
    Color color = (Color){ sprite.tint.r, sprite.tint.g, sprite.tint.b, sprite.tint.a };
    Rectangle src = (Rectangle){ sprite.src.x, sprite.src.y, sprite.src.width, sprite.src.height };
    Rectangle dst = (Rectangle){ sprite.dst.x, sprite.dst.y, sprite.dst.width, sprite.dst.height };
    DrawTexturePro(sprite.texture, src, dst, Vector2Zero(), 0.0f, color);
}


/**
 * Función para dibujar texto en la pantalla.
 *
 * @param text El texto a dibujar.
 */
void gb_rendering_draw_text(gb_text_t text)
{
    float fontSize = (text.size == 0) ? 20.0f : text.size;
    float spacing = (text.spacing == 0.0f) ? (fontSize / 10) : text.spacing;

    Color color = (Color){ text.color.r, text.color.g, text.color.b, text.color.a };
    DrawTextEx(text.font, text.text, Vector2Zero(), fontSize, spacing, color);
}


// ########################################
// Components functions
// ########################################
// Description de components functions: Son funciones que se utilizarán en el código
// de Gobu para crear componentes, como por ejemplo, crear un componente de tipo
// sprite, un componente de tipo texto, etc.
//

static void gb_engine_component_init(ecs_world_t* world)
{
    ECS_COMPONENT_DEFINE(world, gb_render_phases_t);
    ECS_COMPONENT_DEFINE(world, gb_app_t);
    ECS_COMPONENT_DEFINE(world, gb_color_t);
    ECS_COMPONENT_DEFINE(world, gb_rect_t);
    ECS_COMPONENT_DEFINE(world, gb_vec2_t);
    ECS_COMPONENT_DEFINE(world, gb_vec3_t);
    ECS_COMPONENT_DEFINE(world, gb_bounding_t);
    ECS_COMPONENT_DEFINE(world, gb_transform_t);
    ECS_COMPONENT_DEFINE(world, gb_info_t);
    ECS_COMPONENT_DEFINE(world, gb_animated_t);
    ECS_COMPONENT_DEFINE(world, gb_text_t);
    ECS_COMPONENT_DEFINE(world, gb_sprite_t);
    ECS_COMPONENT_DEFINE(world, gb_shape_rect_t);
    ECS_COMPONENT_DEFINE(world, gb_shape_circle_t);
    ECS_COMPONENT_DEFINE(world, gb_resource_t);
    ECS_COMPONENT_DEFINE(world, gb_camera_t);
    ECS_COMPONENT_DEFINE(world, gb_gizmos_t);
    ECS_COMPONENT_DEFINE(world, enumCameraMode);

    ecs_enum(world, {
        .entity = ecs_id(enumCameraMode),
        .constants = {
            {.name = "NONE", .value = GB_CAMERA_NONE },
            {.name = "EDITOR", .value = GB_CAMERA_EDITOR },
            {.name = "FOLLOWING", .value = GB_CAMERA_FOLLOWING },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_vec2_t),
        .members = {
            {.name = "x", .type = ecs_id(ecs_f32_t) },
            {.name = "y", .type = ecs_id(ecs_f32_t) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_vec3_t),
        .members = {
            {.name = "x", .type = ecs_id(ecs_f32_t) },
            {.name = "y", .type = ecs_id(ecs_f32_t) },
            {.name = "z", .type = ecs_id(ecs_f32_t) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_camera_t),
        .members = {
            {.name = "offset", .type = ecs_id(gb_vec2_t) },
            {.name = "target", .type = ecs_id(gb_vec2_t) },
            {.name = "rotation", .type = ecs_id(ecs_f64_t) },
            {.name = "zoom", .type = ecs_id(ecs_f64_t) },
            {.name = "mode", .type = ecs_id(enumCameraMode) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_color_t),
        .members = {
            {.name = "r", .type = ecs_id(ecs_byte_t) },
            {.name = "g", .type = ecs_id(ecs_byte_t) },
            {.name = "b", .type = ecs_id(ecs_byte_t) },
            {.name = "a", .type = ecs_id(ecs_byte_t) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_rect_t),
        .members = {
            {.name = "x", .type = ecs_id(ecs_f32_t) },
            {.name = "y", .type = ecs_id(ecs_f32_t) },
            {.name = "width", .type = ecs_id(ecs_f32_t) },
            {.name = "height", .type = ecs_id(ecs_f32_t) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_transform_t),
        .members = {
            {.name = "position", .type = ecs_id(gb_vec3_t) },
            {.name = "scale", .type = ecs_id(gb_vec3_t) },
            {.name = "rotation", .type = ecs_id(gb_vec3_t) },
            {.name = "origin", .type = ecs_id(gb_vec3_t) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_info_t),
        .members = {
            {.name = "name", .type = ecs_id(ecs_string_t) },
            {.name = "type", .type = ecs_id(ecs_u8_t) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_animated_t),
        .members = {
            {.name = "resource", .type = ecs_id(ecs_string_t) },
            {.name = "animation", .type = ecs_id(ecs_string_t) },
            {.name = "is_playing", .type = ecs_id(ecs_bool_t) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_text_t),
        .members = {
            {.name = "resource", .type = ecs_id(ecs_string_t) },
            {.name = "text", .type = ecs_id(ecs_string_t) },
            {.name = "size", .type = ecs_id(ecs_f32_t) },
            {.name = "spacing", .type = ecs_id(ecs_f32_t) },
            {.name = "color", .type = ecs_id(gb_color_t) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_sprite_t),
        .members = {
            {.name = "resource", .type = ecs_id(ecs_string_t) },
            {.name = "tint", .type = ecs_id(gb_color_t) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_shape_rect_t),
        .members = {
            {.name = "x", .type = ecs_id(ecs_f32_t) },
            {.name = "y", .type = ecs_id(ecs_f32_t) },
            {.name = "width", .type = ecs_id(ecs_f32_t) },
            {.name = "height", .type = ecs_id(ecs_f32_t) },
            {.name = "color", .type = ecs_id(gb_color_t) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_shape_circle_t),
        .members = {
            {.name = "x", .type = ecs_id(ecs_f32_t) },
            {.name = "y", .type = ecs_id(ecs_f32_t) },
            {.name = "radius", .type = ecs_id(ecs_f32_t) },
            {.name = "color", .type = ecs_id(gb_color_t) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_gizmos_t),
        .members = {
            {.name = "selected", .type = ecs_id(ecs_bool_t) },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(gb_resource_t),
        .members = {
            {.name = "path", .type = ecs_id(ecs_string_t) },
        }
    });
}


// ########################################
// System functions
// ########################################
// Description de system functions: Son funciones que se utilizarán en el código
// de Gobu para crear sistemas, como por ejemplo, crear un sistema de renderizado,
// un sistema de animación, etc.
//

static void gb_ecs_observe_set_animated(ecs_iter_t* it)
{
    gb_sprite_t* sprite = ecs_field(it, gb_sprite_t, 1);
    gb_animated_t* animated = ecs_field(it, gb_animated_t, 2);

    for (int i = 0; i < it->count; i++)
    {
        binn* resource = gb_resource(it->world, animated[i].resource)->anim2d;

        char* anim_id = (animated[i].animation == NULL) ? binn_object_str(resource, "default") : animated[i].animation;
        animated[i].width = binn_object_int32(resource, "width");
        animated[i].height = binn_object_int32(resource, "height");

        sprite[i].src.width = animated[i].width;
        sprite[i].src.height = animated[i].height;

        sprite[i].dst.width = animated[i].width;
        sprite[i].dst.height = animated[i].height;

        binn* animation = binn_object_object(binn_object_object(resource, "animations"), anim_id);
        {
            animated[i].loop = binn_object_bool(animation, "loop");
            animated[i].speed = binn_object_int32(animation, "speed");

            binn* frames = binn_object_list(animation, "frames");
            animated[i].count = binn_count(frames);
            for (int n = 1; n <= animated[i].count; n++) {
                animated[i].frames[n - 1] = binn_list_int32(frames, n);
            }

            animated[i].start_frame = animated[i].frames[0];
            animated[i].end_frame = animated[i].frames[(animated[i].count - 1)];
            animated[i].current = 0;
        }
    }
}

static void gb_ecs_observe_set_sprite(ecs_iter_t* it)
{
    gb_sprite_t* sprite = ecs_field(it, gb_sprite_t, 1);

    for (int i = 0; i < it->count; i++)
    {
        sprite[i].texture = gb_resource(it->world, sprite[i].resource)->texture;
        sprite[i].src = (gb_rect_t){ 0.0f, 0.0f, sprite[i].texture.width, sprite[i].texture.height };
        sprite[i].dst = (gb_rect_t){ 0.0f, 0.0f, sprite[i].texture.width, sprite[i].texture.height };
        sprite[i].tint = (sprite[i].tint.a == 0) ? (gb_color_t) { 245, 245, 245, 255 } : sprite[i].tint;
    }
}

static void gb_ecs_observe_set_text(ecs_iter_t* it)
{
    gb_text_t* text = ecs_field(it, gb_text_t, 1);

    for (int i = 0; i < it->count; i++)
    {
        text[i].font = gb_resource(it->world, text[i].resource)->font;
    }
}

static void gb_ecs_observe_set_resource(ecs_iter_t* it)
{
    ecs_entity_t event = it->event;
    gb_resource_t* resource = ecs_field(it, gb_resource_t, 1);

    for (int i = 0; i < it->count; i++)
    {
        char* path = gb_path_join(gb_project_get_path(), "Content", resource[i].path, NULL);
        path = gb_path_normalize(path);

        if (event == EcsOnSet) {
            if (IsFileExtension(path, ".png") || IsFileExtension(path, ".jpg"))
                resource[i].texture = LoadTexture(path);
            if (IsFileExtension(path, ".ttf"))
                resource[i].font = LoadFont(path);
            if (IsFileExtension(path, ".anim"))
                resource[i].anim2d = binn_serialize_from_file(path);

        }
        else if (event == EcsOnRemove) {
            if (IsFileExtension(path, ".png") || IsFileExtension(path, ".jpg"))
                UnloadTexture(resource[i].texture);
            if (IsFileExtension(path, ".ttf"))
                UnloadFont(resource[i].font);
            if (IsFileExtension(path, ".anim"))
                binn_free(resource[i].anim2d);
        }

        free(path);
    }
}

static void gb_ecs_observer_set_window_init(ecs_iter_t* it)
{
    ecs_entity_t event = it->event;
    gb_app_t* win = ecs_field(it, gb_app_t, 1);

    for (int i = 0; i < it->count; i++)
    {
        if (event == EcsOnSet)
            InitWindow(win->width, win->height, win->title);
        else if (event == EcsOnRemove)
            CloseWindow();
    }
}

static void gb_ecs_observer_set_camera_init(ecs_iter_t* it)
{
    gb_camera_t* camera = ecs_field(it, gb_camera_t, 1);
    for (int i = 0; i < it->count; i++)
    {
        camera[i].zoom = (camera[i].zoom == 0) ? 1.0f : camera[i].zoom;
    }
}

static void gb_ecs_update_camera_mode(ecs_iter_t* it)
{
    gb_camera_t* camera = ecs_field(it, gb_camera_t, 1);

    for (int i = 0; i < it->count; i++)
    {
        if (camera[i].mode == GB_CAMERA_FOLLOWING)
        {
        }
        else if (camera[i].mode == GB_CAMERA_EDITOR)
        {
            // move camera mouse movement
            if (engine.input.mouse_button_down(MOUSE_BUTTON_RIGHT))
            {
                Vector2 delta = engine.input.mouse_delta();
                camera[i].target.x -= delta.x / camera[i].zoom;
                camera[i].target.y -= delta.y / camera[i].zoom;
            }

            // zoom with mouse wheel
            float wheel = engine.input.mouse_wheel();
            if (wheel != 0)
            {
                Vector2 mouseWorld = engine.screen_to_world(camera[i], engine.input.mouse_position());

                camera[i].offset = engine.input.mouse_position();
                camera[i].target = mouseWorld;
                camera[i].zoom += wheel * 0.05f;
                if (camera[i].zoom < 0.1f) camera[i].zoom = 0.1f;
            }
        }
    }
}

static void gb_ecs_update_animated(ecs_iter_t* it)
{
    gb_sprite_t* sprite = ecs_field(it, gb_sprite_t, 1);
    gb_animated_t* animated = ecs_field(it, gb_animated_t, 2);

    for (int i = 0; i < it->count; i++)
    {
        gb_texture_t texture = sprite[i].texture;
        int frame_width = animated[i].width;
        int frame_height = animated[i].height;
        int start_frame = animated[i].start_frame;
        int end_frame = animated[i].end_frame;
        float speed = (float)(animated[i].speed * 0.01f);

        if (animated[i].is_playing)
        {
            animated[i].counter += GetFrameTime();
            if (animated[i].counter >= speed)
            {
                animated[i].counter = 0.0f;
                animated[i].current += 1;
                if (animated[i].current > (animated[i].count - 1))
                {
                    if (animated[i].loop)
                        animated[i].current = 0;
                    else {
                        animated[i].current = (animated[i].count - 1);
                        animated[i].is_playing = false;
                    }
                }
            }
        }

        int frame_now = animated[i].frames[animated[i].current];
        int frame_row = (texture.width / frame_width);

        sprite[i].src.x = (frame_now % frame_row) * frame_width;
        sprite[i].src.y = (frame_now / frame_row) * frame_height;
    }
}

static void gb_ecs_update_gismos(ecs_iter_t* it)
{
    gb_gizmos_t* gizmos = ecs_field(it, gb_gizmos_t, 1);
    gb_transform_t* transform = ecs_field(it, gb_transform_t, 2);

    // ecs_entity_t Engine = ecs_lookup(it->world, "Engine");
    // GCamera* camera = ecs_get(it->world, Engine, GCamera);
    // GInputSystem* input = ecs_get(it->world, Engine, GInputSystem);

    // Vector2 mouse = engine.input.mouse.get_screen_to_world(*camera);
    // Vector2 delta = engine.input.mouse.get_delta();

    // bool shift = engine.input.keyboard.down(KEY_LEFT_SHIFT);
    // bool ctrl = engine.input.keyboard.down(KEY_LEFT_CONTROL);
    // bool mouse_btn_pres_left = engine.input.mouse.button_pressed(MOUSE_BUTTON_LEFT);
    // bool mouse_btn_down_left = engine.input.mouse.button_down(MOUSE_BUTTON_LEFT);

    // for (int i = it->count - 1; i >= 0; i--)
    // {
    //     Rectangle bonding = (Rectangle){ box[i].min.x, box[i].min.y, box[i].max.x, box[i].max.y };
    //     ecs_entity_t entity = it->entities[i];

    //     // seleccionamos una sola entidad por click
    //     if (mouse_btn_pres_left)
    //     {
    //         bool selected = pointRectCollision(mouse, bonding, rot[i].x);

    //         // no deseleccionamos cuando tenemos shift presionado
    //         if (!shift)
    //         {
    //             unselected_all(it, gizmos);
    //         }

    //         if (selected)
    //         {
    //             gizmos[i].selected = true;
    //             break;
    //         }
    //     }

    //     // movemos la entidad seleccionadas
    //     if (mouse_btn_down_left)
    //     {
    //         if (gizmos[i].selected)
    //         {
    //             // !TODO: Calcular el mouse con la camara para que se mueva correctamente, cuando tenemos un zoom diferente a 1.0f
    //             // Rotamos la entidad seleccionada si tenemos Ctrl presionado
    //             // pero si no lo tenemos presionado, movemos la entidad.
    //             if (ctrl) {
    //                 // rot[i].x = point_to_angle(mouse, bonding);
    //             }
    //             else {
    //             // movemos la entidad seleccionada
    //                 post[i].x += delta.x;
    //                 post[i].y += delta.y;
    //             }
    //         }
    //     }

    //     // Duplicamos la entidad seleccionadas
    //     if (ctrl && engine.input.keyboard.pressed(KEY_D))
    //     {
    //         if (gizmos[i].selected)
    //         {
    //             ecs_clone(it->world, 0, entity, true);
    //         }
    //     }
    // }
}


static void gb_ecs_predraw_begin_drawing_rendering(ecs_iter_t* it)
{
    gb_app_t* win = ecs_field(it, gb_app_t, 1);
    gb_camera_t* camera = ecs_field(it, gb_camera_t, 2);

    for (int i = 0; i < it->count; i++)
    {
        Camera2D cam = {
            .offset = (Vector2){camera[i].offset.x, camera[i].offset.y},
            .target = (Vector2){camera[i].target.x, camera[i].target.y},
            .rotation = camera[i].rotation,
            .zoom = camera[i].zoom,
        };

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(cam);

        if (win[i].show_grid)
            gb_rendering_draw_grid_2d(win[i].width, 48, (gb_color_t) { 10, 10, 10, 255 });
    }
}

static void gb_ecs_postdraw_drawing_rendering(ecs_iter_t* it)
{
    gb_transform_t* transform = ecs_field(it, gb_transform_t, 1);
    // Drawing
    gb_shape_rect_t* rect = ecs_field(it, gb_shape_rect_t, 2);
    gb_sprite_t* sprite = ecs_field(it, gb_sprite_t, 3);
    gb_text_t* text = ecs_field(it, gb_text_t, 4);

    for (int i = 0; i < it->count; i++)
    {
        rlPushMatrix();
        {
            gb_transform_t trans = transform[i];
            rlTranslatef(trans.position.x, trans.position.y, trans.position.z);
            rlRotatef(trans.rotation.x, 0.0f, 0.0f, 1.0f);
            rlRotatef(trans.rotation.y, 0.0f, 1.0f, 0.0f);
            rlRotatef(trans.rotation.z, 1.0f, 0.0f, 0.0f);
            rlTranslatef(trans.origin.x, trans.origin.y, trans.origin.z);
            rlScalef(trans.scale.x, trans.scale.y, trans.scale.z);

            if (rect)
                gb_rendering_draw_rect(rect[i]);

            if (sprite)
                gb_rendering_draw_sprite(sprite[i]);

            if (text)
                gb_rendering_draw_text(text[i]);
        }
        rlPopMatrix();
    }
}

static void gb_ecs_postdraw_enddrawing_rendering(ecs_iter_t* it)
{
    gb_app_t* win = ecs_field(it, gb_app_t, 1);

    for (int i = 0; i < it->count; i++)
    {
        if (win[i].show_fps)
            DrawFPS(10, 20);

        EndMode2D();
        EndDrawing();

        if (WindowShouldClose()) {
            ecs_quit(it->world);
        }
    }
}

static void gb_engine_system_init(ecs_world_t* world)
{
    render_phases.PreDraw = ecs_new_w_id(world, EcsPhase);
    render_phases.Background = ecs_new_w_id(world, EcsPhase);
    render_phases.Draw = ecs_new_w_id(world, EcsPhase);
    render_phases.PostDraw = ecs_new_w_id(world, EcsPhase);

    ecs_add_pair(world, render_phases.PreDraw, EcsDependsOn, EcsOnStore);
    ecs_add_pair(world, render_phases.Background, EcsDependsOn, render_phases.PreDraw);
    ecs_add_pair(world, render_phases.Draw, EcsDependsOn, render_phases.Background);
    ecs_add_pair(world, render_phases.PostDraw, EcsDependsOn, render_phases.Draw);

    ecs_observer(world, {
        .filter = {.terms = { {.id = ecs_id(gb_sprite_t)} }},
        .events = { EcsOnSet },
        .callback = gb_ecs_observe_set_sprite
    });

    ecs_observer(world, {
        .filter = {.terms = { {.id = ecs_id(gb_text_t)} }},
        .events = { EcsOnSet },
        .callback = gb_ecs_observe_set_text
    });

    ecs_observer(world, {
        .filter = {.terms = { {.id = ecs_id(gb_sprite_t)}, {.id = ecs_id(gb_animated_t)} }},
        .events = { EcsOnSet },
        .callback = gb_ecs_observe_set_animated
    });

    ecs_observer(world, {
        .filter = {.terms = {{.id = ecs_id(gb_resource_t)}}},
        .events = { EcsOnSet, EcsOnRemove },
        .callback = gb_ecs_observe_set_resource
    });

    ecs_observer(world, {
        .filter = {.terms = {{.id = ecs_id(gb_app_t)}}},
        .events = { EcsOnSet, EcsOnRemove },
        .callback = gb_ecs_observer_set_window_init
    });

    ecs_observer(world, {
        .filter = {.terms = {{.id = ecs_id(gb_camera_t)}}},
        .events = { EcsOnSet },
        .callback = gb_ecs_observer_set_camera_init
    });

    ecs_system(world, {
        .entity = ecs_entity(world, {.name = "gb_ecs_update_camera_mode", .add = {ecs_dependson(EcsOnUpdate)} }),
        .query.filter.terms = { {.id = ecs_id(gb_camera_t)} },
        .callback = gb_ecs_update_camera_mode
    });

    ecs_system(world, {
        .entity = ecs_entity(world, {.name = "gb_ecs_update_animated", .add = {ecs_dependson(EcsOnUpdate)} }),
        .query.filter.terms = { {.id = ecs_id(gb_sprite_t)}, {.id = ecs_id(gb_animated_t)} },
        .callback = gb_ecs_update_animated
    });

    ecs_system(world, {
        .entity = ecs_entity(world, {.name = "gb_ecs_update_gismos", .add = {ecs_dependson(EcsOnUpdate)} }),
        .query.filter.terms = { {.id = ecs_id(gb_gizmos_t)}, {.id = ecs_id(gb_transform_t)} },
        .callback = gb_ecs_update_gismos
    });

    ecs_system(world, {
        .entity = ecs_entity(world, {.name = "gb_ecs_predraw_begin_drawing_rendering", .add = {ecs_dependson(render_phases.PreDraw)} }),
        .query.filter.terms = { {.id = ecs_id(gb_app_t)}, {.id = ecs_id(gb_camera_t)} },
        .callback = gb_ecs_predraw_begin_drawing_rendering
    });

    ecs_system(world, {
        .entity = ecs_entity(world, {.name = "gb_ecs_postdraw_drawing_rendering", .add = {ecs_dependson(render_phases.Draw)} }),
        .query.filter.terms = { {.id = ecs_id(gb_transform_t)}, {.id = ecs_id(gb_shape_rect_t), .oper = EcsOptional}, {.id = ecs_id(gb_sprite_t), .oper = EcsOptional}, {.id = ecs_id(gb_text_t), .oper = EcsOptional} },
        .callback = gb_ecs_postdraw_drawing_rendering
    });

    ecs_system(world, {
        .entity = ecs_entity(world, {.name = "gb_ecs_postdraw_enddrawing_rendering", .add = {ecs_dependson(render_phases.PostDraw)} }),
        .query.filter.terms = { {.id = ecs_id(gb_app_t)} },
        .callback = gb_ecs_postdraw_enddrawing_rendering
    });

}


// ########################################
// Resource functions
// ########################################
// Description de resource functions: Son funciones que se utilizarán en el código
// de Gobu para crear recursos, como por ejemplo, crear un recurso de tipo textura,
// un recurso de tipo fuente, etc.
//

/**
 * @brief Establece un recurso en el mundo de entidades.
 *
 * Esta función permite establecer un recurso en el mundo de entidades especificado.
 * El recurso se identifica mediante una clave y se especifica su ruta de acceso.
 *
 * @param world El mundo de entidades en el que se establecerá el recurso.
 * @param key La clave que identifica el recurso.
 * @param path La ruta de acceso al recurso.
 * @return true si se estableció el recurso correctamente, false en caso contrario.
 */
bool gb_resource_set(ecs_world_t* world, const char* key, const char* path)
{
    if (ecs_is_valid(world, ecs_lookup(world, key) == false)) {
        // Buscamos la ruta relativa al directorio Content
        char* npath = strstr(path, "Content");
        if (npath != NULL) {
            npath += strlen("Content");
        }

        ecs_entity_t resource = ecs_new_entity(world, key);
        ecs_set(world, resource, gb_resource_t, { .path = npath });
        return true;
    }
    return false;
}

/**
 * @brief Obtiene un recurso de Gobu.
 *
 * Esta función devuelve un puntero al recurso identificado por la clave especificada.
 *
 * @param world Puntero al mundo de entidades de Gobu.
 * @param key Clave del recurso a obtener.
 * @return Puntero al recurso correspondiente a la clave especificada, o NULL si no se encuentra.
 */
const gb_resource_t* gb_resource(ecs_world_t* world, const char* key)
{
    ecs_entity_t resource = ecs_lookup(world, key);
    return (gb_resource_t*)ecs_get(world, resource, gb_resource_t);
}


// ########################################
// ECS functions
// ########################################
// Description de ecs functions: Son funciones que se utilizarán en el código
// de Gobu para crear entidades, como por ejemplo, crear una entidad de tipo
// sprite, una entidad de tipo texto, etc.
//

ecs_entity_t gb_ecs_entity_new(ecs_world_t* world, const char* name, const gb_transform_t t)
{
    ecs_entity_t entity = ecs_new_id(world);
    gb_ecs_entity_set(world, entity, gb_info_t, { .name = gb_strdup(name) });
    gb_ecs_entity_set(world, entity, gb_transform_t, { .position = t.position, .scale = t.scale, .rotation = t.rotation, .origin = t.origin });
    gb_ecs_entity_set(world, entity, gb_bounding_t, { 0 });
    gb_ecs_entity_set(world, entity, gb_gizmos_t, { .selected = false });

    return entity;
}

void gb_ecs_entity_set_parent(ecs_world_t* world, ecs_entity_t parent, ecs_entity_t child)
{
    ecs_add_pair(world, child, EcsChildOf, parent);
}

const char* gb_ecs_entity_get_name(ecs_world_t* world, ecs_entity_t entity)
{
    return gb_strdup(ecs_get(world, entity, gb_info_t)->name);
}

void gb_ecs_entity_set_name(ecs_world_t* world, ecs_entity_t entity, const char* name)
{
    gb_info_t* info = ecs_get(world, entity, gb_info_t);
    info->name = gb_strdup(name);
}


// ########################################
// APP functions
// ########################################
// Description de window app functions: 

ecs_world_t* gb_app_init(gb_app_t* app)
{
    ecs_world_t* world = ecs_init();

    gb_engine_component_init(world);
    gb_engine_system_init(world);

    // SetConfigFlags(FLAG_MSAA_4X_HINT);
    ecs_entity_t Engine = ecs_new_entity(world, "Engine");
    ecs_set(world, Engine, gb_app_t, {
        .title = app->title,         // Establece el título de la ventana
        .width = app->width,         // Establece el ancho de la ventana
        .height = app->height,       // Establece la altura de la ventana
        .fps = app->fps,             // Establece los cuadros por segundo
        .show_fps = app->show_fps,   // Establece si se muestra los FPS
        .show_grid = app->show_grid  // Establece si se muestra la cuadrícula
    });
    ecs_set(world, Engine, gb_camera_t, { .zoom = 1.0f, .rotation = 0.0f });
    SetTargetFPS(app->fps ? app->fps : 60);

    gb_engine_init(world);

    return world;
}

void gb_app_main(ecs_world_t* world)
{
    while (ecs_progress(world, GetFrameTime())) {}
}

void gb_app_progress(ecs_world_t* world)
{
    ecs_progress(world, GetFrameTime());
}


// ########################################
// Engine functions
// ########################################
// Description de engine app functions: 

static gb_vec2_t getscreentoworld2d(gb_camera_t camera, gb_vec2_t position)
{
    Camera2D cam = {
        .offset = (Vector2){camera.offset.x, camera.offset.y},
        .target = (Vector2){camera.target.x, camera.target.y},
        .rotation = camera.rotation,
        .zoom = camera.zoom,
    };
    return GetScreenToWorld2D(position, cam);
}

static gb_vec2_t getworldtoscreen2d(gb_camera_t camera, gb_vec2_t position)
{
    Camera2D cam = {
        .offset = (Vector2){camera.offset.x, camera.offset.y},
        .target = (Vector2){camera.target.x, camera.target.y},
        .rotation = camera.rotation,
        .zoom = camera.zoom,
    };
    return GetWorldToScreen2D(position, cam);
}

void gb_engine_init(ecs_world_t* world)
{
    engine.input.mouse_button_down = IsMouseButtonDown;
    engine.input.mouse_button_pressed = IsMouseButtonPressed;
    engine.input.mouse_button_released = IsMouseButtonReleased;
    engine.input.mouse_position = GetMousePosition;
    engine.input.mouse_delta = GetMouseDelta;
    engine.input.mouse_wheel = GetMouseWheelMove;
    engine.input.mouse_position_set = SetMousePosition;
    engine.input.mouse_cursor_set = SetMouseCursor;

    engine.input.key_down = IsKeyDown;
    engine.input.key_pressed = IsKeyPressed;
    engine.input.key_released = IsKeyReleased;

    engine.world_to_screen = getworldtoscreen2d;
    engine.screen_to_world = getscreentoworld2d;
}

