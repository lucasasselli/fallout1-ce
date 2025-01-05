#include "plib/gnw/controller.h"

#include "plib/gnw/debug.h"
#include "plib/gnw/mouse.h"

#define CONTROLLER_AXIS_DEADZONE 0.1
#define CONTROLLER_AXIS_MOD 2.0f;
#define CONTROLLER_AXIS_SPEEDUP 1.0f
#define CONTROLLER_SPEED_MOD 0.2f;

namespace fallout {
static SDL_GameController* controller = nullptr;
static int axis_time_old[SDL_CONTROLLER_AXIS_MAX];
static int axis_leftover[SDL_CONTROLLER_AXIS_MAX];

void controller_init()
{
    controller_exit();

    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            debug_printf("Controller %0d found!", i);
            controller = SDL_GameControllerOpen(i);
            break;
        }
    }
}

void controller_exit()
{
    SDL_GameControllerClose(controller);
}

int controller_axis_get(SDL_GameControllerAxis axis)
{
    const int axis_time_new = SDL_GetTicks();
    const int axis_time_delta = axis_time_new - axis_time_old[axis];
    axis_time_old[axis] = axis_time_new;

    float v = SDL_GameControllerGetAxis(controller, axis) / (float)SDL_JOYSTICK_AXIS_MAX;

    if (std::abs(v) < CONTROLLER_AXIS_DEADZONE)
        v = 0;

    v *= CONTROLLER_AXIS_MOD;

    const int32_t sign = (v > 0) - (v < 0);
    float delta = std::pow(std::abs(v), CONTROLLER_AXIS_SPEEDUP) * sign * axis_time_delta * CONTROLLER_SPEED_MOD + axis_leftover[axis];

    axis_leftover[axis] = delta - static_cast<int>(delta);

    return delta;
}

bool controller_get_mouse_state(MouseData* mouseData)
{
#if __SWITCH__
    mouseData->buttons[0] = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B);
    mouseData->buttons[1] = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
#else
    mouseData->buttons[0] = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
    mouseData->buttons[1] = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B);
#endif

    mouseData->x = controller_axis_get(SDL_CONTROLLER_AXIS_LEFTX);
    mouseData->y = controller_axis_get(SDL_CONTROLLER_AXIS_LEFTY);

    mouseData->wheelX = 0;
    mouseData->wheelY = 0;

    return true;
}

SDL_Scancode controller_button_to_scancode(int btn)
{
    switch (btn) {
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
        // inventory
        return SDL_SCANCODE_I;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        // pipboy
        return SDL_SCANCODE_P;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        // skills
        return SDL_SCANCODE_S;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        // character
        return SDL_SCANCODE_C;
    case SDL_CONTROLLER_BUTTON_START:
        // new turn
        return SDL_SCANCODE_SPACE;
    case SDL_CONTROLLER_BUTTON_BACK:
        // esc
        return SDL_SCANCODE_ESCAPE;
    case SDL_CONTROLLER_BUTTON_X:
        // TODO
        return SDL_SCANCODE_S;
    case SDL_CONTROLLER_BUTTON_Y:
        // change active item
        return SDL_SCANCODE_B;
    case SDL_CONTROLLER_BUTTON_LEFTSTICK:
        // alternate actions
        return SDL_SCANCODE_N;
    case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
        // center character
        return SDL_SCANCODE_HOME;
    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
        // TODO
        return SDL_SCANCODE_UNKNOWN;
    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
        // TODO
        return SDL_SCANCODE_UNKNOWN;
    default:
        return SDL_SCANCODE_UNKNOWN;
    }
}

} // namespace fallout
