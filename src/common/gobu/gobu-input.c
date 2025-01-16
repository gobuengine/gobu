#include "gobu-input.h"

typedef struct
{
    go_vec2_t position;
    go_vec2_t wheel;
    bool buttons[MAX_MOUSE_BUTTONS];
} go_mouse_state_t;

typedef struct
{
    bool keys[MAX_KEYBOARD_KEYS];
} go_keyboard_state_t;

static struct
{
    go_mouse_state_t current_mouse;
    go_mouse_state_t previous_mouse;
    go_keyboard_state_t current_keyboard;
    go_keyboard_state_t previous_keyboard;
} input_state = {0};

// Input state management
go_public void go_event_pool(void)
{
    // Update previous states before processing new events
    input_state.previous_mouse = input_state.current_mouse;
    input_state.previous_keyboard = input_state.current_keyboard;

    // Reset wheel state for new frame
    input_state.previous_mouse.wheel = input_state.current_mouse.wheel;
    input_state.current_mouse.wheel = (go_vec2_t){0.0f, 0.0f};
}

go_public void go_event_pool_events(const go_event_t *event)
{
    if (!event)
        return;

    switch (event->type)
    {
    case GO_EVENT_KEY_DOWN:
    case GO_EVENT_KEY_UP:
        go_event_keyboard_set_keys(event->key, event->type);
        break;

    case GO_EVENT_MOUSE_DOWN:
    case GO_EVENT_MOUSE_UP:
        go_event_mouse_set_buttons(event->mouse_button, event->type);
        break;

    case GO_EVENT_MOUSE_MOVE:
        go_event_mouse_set_position(event->mouse_x, event->mouse_y);
        break;

    case GO_EVENT_MOUSE_SCROLL:
        go_event_mouse_set_wheel(event->scroll_x, event->scroll_y);
        break;
    }
}

// Keyboard functions
go_public void go_event_keyboard_set_keys(int key, int action)
{
    if (key < 0 || key >= MAX_KEYBOARD_KEYS)
        return;

    input_state.current_keyboard.keys[key] = (action == GO_EVENT_KEY_DOWN);
}

go_public bool go_keyboard_is_down(go_keyboard key)
{
    if (key < 0 || key >= MAX_KEYBOARD_KEYS)
        return false;
    return input_state.current_keyboard.keys[key];
}

go_public bool go_keyboard_is_up(go_keyboard key)
{
    return !go_keyboard_is_down(key);
}

go_public bool go_keyboard_is_pressed(go_keyboard key)
{
    if (key < 0 || key >= MAX_KEYBOARD_KEYS)
        return false;

    return !input_state.previous_keyboard.keys[key] &&
           input_state.current_keyboard.keys[key];
}

go_public bool go_keyboard_is_released(go_keyboard key)
{
    if (key < 0 || key >= MAX_KEYBOARD_KEYS)
        return false;

    return input_state.previous_keyboard.keys[key] &&
           !input_state.current_keyboard.keys[key];
}

// Mouse functions
go_public void go_event_mouse_set_buttons(int button, int action)
{
    if (button < 0 || button >= MAX_MOUSE_BUTTONS)
        return;

    input_state.current_mouse.buttons[button] = (action == GO_EVENT_MOUSE_DOWN);
}

go_public void go_event_mouse_set_position(float x, float y)
{
    input_state.current_mouse.position = (go_vec2_t){x, y};
}

go_public void go_event_mouse_set_wheel(float x, float y)
{
    input_state.current_mouse.wheel = (go_vec2_t){x, y};
}

go_public bool go_mouse_is_down(go_mousebutton button)
{
    if (button < 0 || button >= MAX_MOUSE_BUTTONS)
        return false;
    return input_state.current_mouse.buttons[button];
}

go_public bool go_mouse_is_up(go_mousebutton button)
{
    return !go_mouse_is_down(button);
}

go_public bool go_mouse_is_pressed(go_mousebutton button)
{
    if (button < 0 || button >= MAX_MOUSE_BUTTONS)
        return false;

    return !input_state.previous_mouse.buttons[button] &&
           input_state.current_mouse.buttons[button];
}

go_public bool go_mouse_is_released(go_mousebutton button)
{
    if (button < 0 || button >= MAX_MOUSE_BUTTONS)
        return false;

    return input_state.previous_mouse.buttons[button] &&
           !input_state.current_mouse.buttons[button];
}

go_public int go_mouse_x(void)
{
    return (int)input_state.current_mouse.position.x;
}

go_public int go_mouse_y(void)
{
    return (int)input_state.current_mouse.position.y;
}

go_public go_vec2_t go_mouse_pos(void)
{
    return input_state.current_mouse.position;
}

go_public go_vec2_t go_mouse_delta(void)
{
    return go_math_vec2_sub(input_state.current_mouse.position,
                            input_state.previous_mouse.position);
}

go_public float go_mouse_wheel(void)
{
    return input_state.current_mouse.wheel.y;
}

go_public go_vec2_t go_mouse_wheel_vec(void)
{
    return input_state.current_mouse.wheel;
}

