#include "joystick.h"

void Joystick::begin() {
  up = new RBD::Button(_pin_up);
  down = new RBD::Button(_pin_down);
  left = new RBD::Button(_pin_left);
  right = new RBD::Button(_pin_right);
  push = new RBD::Button(_pin_push);
}
