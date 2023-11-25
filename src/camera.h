#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


typedef struct Camera_t
{
    vec3 position;
    vec3 front;
    vec3 up;

    float yaw;
    float pitch;

    float mouse_sensitivity;

    float fov;

    float last_frame;
    float delta_time;
    float last_x;
    float last_y;

    short first_mouse;
} Camera_t;

Camera_t Camera;

void move_forward(void);
void move_backward(void);
void move_left(void);
void move_right(void);
void change_camera_direction(double xpos, double ypos);
void zoom(double yoffset);
void update_frame(void);

typedef struct CameraNS_t {
  void (*move_forward)(void);
  void (*move_backward)(void);
  void (*move_left)(void);
  void (*move_right)(void);
  void (*change_camera_direction)(double xpos, double ypos);
  void (*update_frame)(void);
  void (*zoom)(double yoffset);
} CameraNS_t;

extern CameraNS_t CameraNS;
#endif

