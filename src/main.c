#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#define APP_NAME "my_gl"

#include "renderer.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "stb_image.h"
#include "shader.h"

#include "camera.h"

float cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

VertexBuffer* init_cube(GLuint VAO);
void draw_cube(Shader shader, GLuint VAO, vec3 position, vec3 scale);

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    CameraNS.move_forward();
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    CameraNS.move_backward();
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    CameraNS.move_left();
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    CameraNS.move_right();
  }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  CameraNS.change_camera_direction(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  CameraNS.zoom(yoffset);
}

void register_texture(GLuint* texture, char * name) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nr_channels;
    unsigned char *data = stbi_load(name, &width, &height, &nr_channels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, 
          GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      fprintf(stderr, "Failed to load texture.\n");
    }
    stbi_image_free(data); 
}

int main(int argc, char** argv) {
  if (argc < 4) {
    fprintf(stderr, "Usage: %s <vertex_shader_path> <fragment_shader_path> <light_fragment_shader_path>\n", argv[0]);
    return -1;
  }
  char* vertex_shader_path = argv[1];
  char* fragment_shader_path = argv[2];
  char* light_fragment_shader_path = argv[3];

  GLFWwindow* window;
  

  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
    printf("I'm apple machine\n");
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME, NULL, NULL);

  if (window == NULL) {
    fprintf(stderr, "Failed to open GLFW window.\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSwapInterval(1);


  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }

  {
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    VertexBuffer* v = init_cube(VAO);
   
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    VertexBuffer* lv = init_cube(lightVAO);

    Shader default_shader = ShaderNS.new(vertex_shader_path, fragment_shader_path);
    ShaderNS.use(default_shader);
    ShaderNS.setVec3(default_shader, "objectColor", (vec3){1.0f, 0.5f, 0.31f});
    ShaderNS.setVec3(default_shader, "lightColor", (vec3){1.0f, 1.0f, 1.0f});

    Shader light_shader = ShaderNS.new(vertex_shader_path, light_fragment_shader_path);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
      process_input(window);
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gl_clear_error();

      
      CameraNS.update_frame();
      
      vec3 light_pos = {sin(glfwGetTime()) * 2.0f, sin(glfwGetTime() / 2.0f) * 1.0f, 2.0f};

      ShaderNS.use(light_shader);
      ShaderNS.setVec3(light_shader, "lightPos", light_pos);
      
      ShaderNS.use(default_shader);
      ShaderNS.setVec3(default_shader, "lightPos", light_pos);
      ShaderNS.setVec3(default_shader, "viewPos", Camera.position);

      draw_cube(default_shader, VAO, (vec3){0.0f, 0.0f, 0.0f}, (vec3){1.0f, 1.0f, 1.0f});
      draw_cube(light_shader, lightVAO, light_pos, (vec3){0.2f, 0.2f, 0.2f});

      gl_check_error();
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    VertexBufferNS.delete(v);
    VertexBufferNS.delete(lv);
    ShaderNS.delete(default_shader);
    ShaderNS.delete(light_shader);
  }
  glfwTerminate();

  exit(EXIT_SUCCESS);
}

VertexBuffer* init_cube(GLuint VAO) {
  glBindVertexArray(VAO);

  VertexBuffer vb = VertexBufferNS.create(cube, sizeof(cube));

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
  return &vb;
}

void draw_cube(Shader shader, GLuint VAO, vec3 position, vec3 scale) {
  ShaderNS.use(shader);
  mat4 view = GLM_MAT4_IDENTITY_INIT;
  vec3 res;
  glm_vec3_add(Camera.position, Camera.front, res);
  glm_lookat(Camera.position, res, Camera.up, view);
  
  ShaderNS.setMat4(shader, "view", view);

  mat4 projection = GLM_MAT4_IDENTITY_INIT;
  glm_perspective(glm_rad(Camera.fov), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f, projection);

  ShaderNS.setMat4(shader, "projection", projection);

  glBindVertexArray(VAO);
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  glm_translate(model, position);
  glm_scale(model, scale);

  ShaderNS.setMat4(shader, "model", model);

  glDrawArrays(GL_TRIANGLES, 0, 36); 
}

