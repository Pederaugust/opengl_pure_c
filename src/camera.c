
#include "camera.h"

Camera_t Camera = {
    .position = {0.0f, 0.0f, 8.0f},
    .front = {0.0f, 0.0f, -1.0f},
    .up = {0.0f, 1.0f, 0.0f},
    .yaw = -90.0f,
    .pitch = 0.0f,
    .mouse_sensitivity = 0.1f,
    .fov = 45.0f,
    .last_frame = 0.0f,
    .delta_time = 0.0f,
    .last_x = SCREEN_WIDTH / 2.0f,
    .last_y = SCREEN_HEIGHT / 2.0f,
    .first_mouse = 1
};

void move_forward(void) {
  float camera_speed = 5.0f * Camera.delta_time;
  vec3 left;
  glm_vec3_cross(Camera.front, Camera.up, left);
  glm_normalize(left);

  vec3 res;
  glm_vec3_scale(Camera.front, camera_speed, res);
  glm_vec3_add(Camera.position, res, Camera.position);
}

void move_backward(void) {
  float camera_speed = 5.0f * Camera.delta_time;
  vec3 left;
  glm_vec3_cross(Camera.front, Camera.up, left);
  glm_normalize(left);

  vec3 res;
  glm_vec3_scale(Camera.front, camera_speed, res);
  glm_vec3_sub(Camera.position, res, Camera.position);
}

void move_left(void) {
  float camera_speed = 5.0f * Camera.delta_time;
  vec3 left;
  glm_vec3_cross(Camera.front, Camera.up, left);
  glm_normalize(left);

  vec3 res;
  glm_vec3_scale(left, camera_speed, res);
  glm_vec3_sub(Camera.position, res, Camera.position);
}

void move_right(void) {
  float camera_speed = 5.0f * Camera.delta_time;
  vec3 left;
  glm_vec3_cross(Camera.front, Camera.up, left);
  glm_normalize(left);

  vec3 res;
  glm_vec3_scale(left, camera_speed, res);
  glm_vec3_add(Camera.position, res, Camera.position);
}

void change_camera_direction(double xpos, double ypos) {
  if (Camera.first_mouse) {
    Camera.last_x = xpos;
    Camera.last_y = ypos;
    Camera.first_mouse = 0;
  }

  float xoffset = xpos - Camera.last_x;
  float yoffset = Camera.last_y - ypos;
  Camera.last_x = xpos;
  Camera.last_y = ypos;

  xoffset *= Camera.mouse_sensitivity;
  yoffset *= Camera.mouse_sensitivity;
  
  Camera.yaw += xoffset;
  Camera.pitch += yoffset;
  
  if (Camera.pitch > 89.0f) {
    Camera.pitch = 89.0f;
  }
  if (Camera.pitch < -89.0f) {
    Camera.pitch = -89.0f;
  }

  vec3 direction = {cos(glm_rad(Camera.yaw))*cos(glm_rad(Camera.pitch)),
    sin(glm_rad(Camera.pitch)),
    sin(glm_rad(Camera.yaw))*cos(glm_rad(Camera.pitch))};
  glm_normalize(direction);
  glm_vec3_copy(direction, Camera.front);
}

void update_frame(void) {
  float current_frame = glfwGetTime();
  Camera.delta_time = current_frame - Camera.last_frame;
  Camera.last_frame = current_frame;
}

void zoom(double yoffset) {
  Camera.fov -= yoffset;
  if (Camera.fov < 1.0f) {
    Camera.fov = 1.0f;
  }
  if (Camera.fov > 45.0f) {
    Camera.fov = 45.0f;
  }
}

CameraNS_t CameraNS = {
  .move_forward = move_forward,
  .move_backward = move_backward,
  .move_left = move_left,
  .move_right = move_right,
  .change_camera_direction = change_camera_direction,
  .update_frame = update_frame,
  .zoom = zoom
};

