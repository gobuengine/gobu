#ifndef GOBU_INPUT_H
#define GOBU_INPUT_H

#include "gobu.h"

#define MAX_KEYBOARD_KEYS 512
#define MAX_MOUSE_BUTTONS 3

typedef enum go_event_type
{
    GO_EVENT_INVALID,
    GO_EVENT_KEY_DOWN,
    GO_EVENT_KEY_UP,
    GO_EVENT_CHAR,
    GO_EVENT_MOUSE_DOWN,
    GO_EVENT_MOUSE_UP,
    GO_EVENT_MOUSE_SCROLL,
    GO_EVENT_MOUSE_MOVE,
    GO_EVENT_MOUSE_ENTER,
    GO_EVENT_MOUSE_LEAVE,
    GO_EVENT_TOUCHES_BEGAN,
    GO_EVENT_TOUCHES_MOVED,
    GO_EVENT_TOUCHES_ENDED,
    GO_EVENT_TOUCHES_CANCELLED,
    GO_EVENT_RESIZED,
    GO_EVENT_ICONIFIED,
    GO_EVENT_RESTORED,
    GO_EVENT_FOCUSED,
    GO_EVENT_UNFOCUSED,
    GO_EVENT_SUSPENDED,
    GO_EVENT_RESUMED,
    GO_EVENT_QUIT_REQUESTED,
    GO_EVENT_CLIPBOARD_PASTED,
    GO_EVENT_FILES_DROPPED,
    _GO_EVENT_NUM,
    _GO_EVENT_FORCE_U32 = 0x7FFFFFFF
} go_event_type;

typedef enum go_keyboard
{
    GO_KEY_INVALID = 0,
    GO_KEY_SPACE = 32,
    GO_KEY_APOSTROPHE = 39, /* ' */
    GO_KEY_COMMA = 44,      /* , */
    GO_KEY_MINUS = 45,      /* - */
    GO_KEY_PERIOD = 46,     /* . */
    GO_KEY_SLASH = 47,      /* / */
    GO_KEY_0 = 48,
    GO_KEY_1 = 49,
    GO_KEY_2 = 50,
    GO_KEY_3 = 51,
    GO_KEY_4 = 52,
    GO_KEY_5 = 53,
    GO_KEY_6 = 54,
    GO_KEY_7 = 55,
    GO_KEY_8 = 56,
    GO_KEY_9 = 57,
    GO_KEY_SEMICOLON = 59, /* ; */
    GO_KEY_EQUAL = 61,     /* = */
    GO_KEY_A = 65,
    GO_KEY_B = 66,
    GO_KEY_C = 67,
    GO_KEY_D = 68,
    GO_KEY_E = 69,
    GO_KEY_F = 70,
    GO_KEY_G = 71,
    GO_KEY_H = 72,
    GO_KEY_I = 73,
    GO_KEY_J = 74,
    GO_KEY_K = 75,
    GO_KEY_L = 76,
    GO_KEY_M = 77,
    GO_KEY_N = 78,
    GO_KEY_O = 79,
    GO_KEY_P = 80,
    GO_KEY_Q = 81,
    GO_KEY_R = 82,
    GO_KEY_S = 83,
    GO_KEY_T = 84,
    GO_KEY_U = 85,
    GO_KEY_V = 86,
    GO_KEY_W = 87,
    GO_KEY_X = 88,
    GO_KEY_Y = 89,
    GO_KEY_Z = 90,
    GO_KEY_LEFT_BRACKET = 91,  /* [ */
    GO_KEY_BACKSLASH = 92,     /* \ */
    GO_KEY_RIGHT_BRACKET = 93, /* ] */
    GO_KEY_GRAVE_ACCENT = 96,  /* ` */
    GO_KEY_WORLD_1 = 161,      /* non-US #1 */
    GO_KEY_WORLD_2 = 162,      /* non-US #2 */
    GO_KEY_ESCAPE = 256,
    GO_KEY_ENTER = 257,
    GO_KEY_TAB = 258,
    GO_KEY_BACKSPACE = 259,
    GO_KEY_INSERT = 260,
    GO_KEY_DELETE = 261,
    GO_KEY_RIGHT = 262,
    GO_KEY_LEFT = 263,
    GO_KEY_DOWN = 264,
    GO_KEY_UP = 265,
    GO_KEY_PAGE_UP = 266,
    GO_KEY_PAGE_DOWN = 267,
    GO_KEY_HOME = 268,
    GO_KEY_END = 269,
    GO_KEY_CAPS_LOCK = 280,
    GO_KEY_SCROLL_LOCK = 281,
    GO_KEY_NUM_LOCK = 282,
    GO_KEY_PRINT_SCREEN = 283,
    GO_KEY_PAUSE = 284,
    GO_KEY_F1 = 290,
    GO_KEY_F2 = 291,
    GO_KEY_F3 = 292,
    GO_KEY_F4 = 293,
    GO_KEY_F5 = 294,
    GO_KEY_F6 = 295,
    GO_KEY_F7 = 296,
    GO_KEY_F8 = 297,
    GO_KEY_F9 = 298,
    GO_KEY_F10 = 299,
    GO_KEY_F11 = 300,
    GO_KEY_F12 = 301,
    GO_KEY_F13 = 302,
    GO_KEY_F14 = 303,
    GO_KEY_F15 = 304,
    GO_KEY_F16 = 305,
    GO_KEY_F17 = 306,
    GO_KEY_F18 = 307,
    GO_KEY_F19 = 308,
    GO_KEY_F20 = 309,
    GO_KEY_F21 = 310,
    GO_KEY_F22 = 311,
    GO_KEY_F23 = 312,
    GO_KEY_F24 = 313,
    GO_KEY_F25 = 314,
    GO_KEY_KP_0 = 320,
    GO_KEY_KP_1 = 321,
    GO_KEY_KP_2 = 322,
    GO_KEY_KP_3 = 323,
    GO_KEY_KP_4 = 324,
    GO_KEY_KP_5 = 325,
    GO_KEY_KP_6 = 326,
    GO_KEY_KP_7 = 327,
    GO_KEY_KP_8 = 328,
    GO_KEY_KP_9 = 329,
    GO_KEY_KP_DECIMAL = 330,
    GO_KEY_KP_DIVIDE = 331,
    GO_KEY_KP_MULTIPLY = 332,
    GO_KEY_KP_SUBTRACT = 333,
    GO_KEY_KP_ADD = 334,
    GO_KEY_KP_ENTER = 335,
    GO_KEY_KP_EQUAL = 336,
    GO_KEY_LEFT_SHIFT = 340,
    GO_KEY_LEFT_CONTROL = 341,
    GO_KEY_LEFT_ALT = 342,
    GO_KEY_LEFT_SUPER = 343,
    GO_KEY_RIGHT_SHIFT = 344,
    GO_KEY_RIGHT_CONTROL = 345,
    GO_KEY_RIGHT_ALT = 346,
    GO_KEY_RIGHT_SUPER = 347,
    GO_KEY_MENU = 348,
} go_keyboard;

typedef enum go_mousebutton
{
    GO_MOUSEBUTTON_LEFT = 0,
    GO_MOUSEBUTTON_RIGHT = 1,
    GO_MOUSEBUTTON_MIDDLE = 2,
    GO_MOUSEBUTTON_INVALID = 3,
} go_mousebutton;

typedef struct go_event_t
{
    go_event_type type;
    go_keyboard key;
    go_mousebutton mouse_button;
    uint32_t char_code;
    int width;
    int height;
    float mouse_x;
    float mouse_y;
    float mouse_dx;
    float mouse_dy;
    float scroll_x;
    float scroll_y;
} go_event_t;

go_public void go_event_pool_events(const go_event_t *event);
go_public void go_event_pool(void);

go_public void go_event_mouse_set_position(float x, float y);
go_public void go_event_mouse_set_wheel(float wheel_x, float wheel_y);
go_public void go_event_keyboard_set_keys(int key, int action);
go_public void go_event_mouse_set_buttons(int mouse_button, int action);

go_public int go_mouse_x(void);
go_public int go_mouse_y(void);
go_public go_vec2_t go_mouse_pos(void);
go_public go_vec2_t go_mouse_delta(void);

go_public bool go_mouse_is_down(go_mousebutton button);
go_public bool go_mouse_is_up(go_mousebutton button);
go_public bool go_mouse_is_pressed(go_mousebutton button);
go_public bool go_mouse_is_released(go_mousebutton button);
go_public float go_mouse_wheel(void);
go_public go_vec2_t go_mouse_wheel_vec(void);

go_public bool go_keyboard_is_up(go_keyboard key);
go_public bool go_keyboard_is_down(go_keyboard key);
go_public bool go_keyboard_is_pressed(go_keyboard key);
go_public bool go_keyboard_is_released(go_keyboard key);


#endif // GOBU_INPUT_H
