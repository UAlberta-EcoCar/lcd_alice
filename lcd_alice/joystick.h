#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <Arduino.h>
#include <RBD_Timer.h>
#include <RBD_Button.h>

class Joystick {
public:
  Joystick(int left, int right, int up, int down, int push):
   _pin_left(left), _pin_right(right), _pin_up(up), _pin_down(down), _pin_push(push) {};
  void begin();

  RBD::Button * up;
  RBD::Button * down;
  RBD::Button * left;
  RBD::Button * right;
  RBD::Button * push;
private:
  int _pin_left;
  int _pin_right;
  int _pin_up;
  int _pin_down;
  int _pin_push;
};

#endif
