#ifndef FALLOUT_PLIB_GNW_CONTROLLER_H_
#define FALLOUT_PLIB_GNW_CONTROLLER_H_

#include <SDL.h>

#include "plib/gnw/dxinput.h"

namespace fallout {

void controller_init();

void controller_exit();

bool controller_get_mouse_state(MouseData*);

SDL_Scancode controller_button_to_scancode(int);

int controller_axis_get(SDL_GameControllerAxis axis);

} // namespace fallout

#endif /* FALLOUT_PLIB_GNW_CONTROLLER_H_ */
