#pragma once

#include "math/lin/vec3.h"
#include "base/mutex.h"

enum {
  PAD_BUTTON_A = 1,
  PAD_BUTTON_B = 2,
  PAD_BUTTON_X = 4,
  PAD_BUTTON_Y = 8,
  PAD_BUTTON_LBUMPER = 16,
  PAD_BUTTON_RBUMPER = 32,
  PAD_BUTTON_START = 64,
  PAD_BUTTON_BACK = 128,
  PAD_BUTTON_UP = 256,
  PAD_BUTTON_DOWN = 512,
  PAD_BUTTON_LEFT = 1024,
  PAD_BUTTON_RIGHT = 2048,
};

#ifndef MAX_POINTERS
#define MAX_POINTERS 8
#endif
  
// Agglomeration of all possible inputs, and automatically computed
// deltas where applicable.
struct InputState {
  // Lock this whenever you access the data in this struct.
  mutable recursive_mutex lock;
  InputState()
    : pad_buttons(0),
      pad_last_buttons(0),
      pad_buttons_down(0),
      pad_buttons_up(0),
      mouse_valid(false),
      accelerometer_valid(false) {
    memset(mouse_down, 0, sizeof(mouse_down));
    memset(mouse_last, 0, sizeof(mouse_last));
  }

  // Gamepad style input
  int pad_buttons; // bitfield
  int pad_last_buttons;
  int pad_buttons_down;  // buttons just pressed this frame
  int pad_buttons_up;  // buttons just pressed last frame
  float pad_lstick_x;
  float pad_lstick_y;
  float pad_rstick_x;
  float pad_rstick_y;
  float pad_ltrigger;
  float pad_rtrigger;

  // Mouse/touch style input
  // There are up to 8 mice / fingers.
  volatile bool mouse_valid;

  int mouse_x[MAX_POINTERS];
  int mouse_y[MAX_POINTERS];
  bool mouse_down[MAX_POINTERS];
  bool mouse_last[MAX_POINTERS];

  // Accelerometer
  bool accelerometer_valid;
  Vec3 acc;
};

inline void UpdateInputState(InputState *input) {
  input->pad_buttons_down = (input->pad_last_buttons ^ input->pad_buttons) & input->pad_buttons;
  input->pad_buttons_up = (input->pad_last_buttons ^ input->pad_buttons) & input->pad_last_buttons;
  input->pad_last_buttons = input->pad_buttons;
}
