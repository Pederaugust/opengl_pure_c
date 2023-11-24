#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#define APP_NAME "my_gl"

#include "utils.h"
#include "renderer.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "stb_image.h"

#include "camera.h"

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
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <vertex_shader_path> <fragment_shader_path>\n", argv[0]);
    return -1;
  }
  char* vertex_shader_path = argv[1];
  char* fragment_shader_path = argv[2];

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

    unsigned int texture1;
    register_texture(&texture1, "assets/awesomeface.jpg");
    unsigned int texture2;
    register_texture(&texture2, "assets/container.jpg");

    float cube[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vec3 cube_positions[] = {
      { 0.0f,  0.0f,  0.0f}, 
      { 2.0f,  5.0f, -15.0f}, 
      {-1.5f, -2.2f, -2.5f},  
      {-3.8f, -2.0f, -12.3f},  
      { 2.4f, -0.4f, -3.5f},  
      {-1.7f,  3.0f, -7.5f},  
      { 1.3f, -2.0f, -2.5f},  
      { 1.5f,  2.0f, -2.5f}, 
      { 1.5f,  0.2f, -1.5f}, 
      {-1.3f,  1.0f, -1.5f}
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VertexBuffer vb = VertexBufferNS.create(cube, sizeof(cube));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint shader_program = Utils.load_shaders(vertex_shader_path, fragment_shader_path);
    gl_check_error();

    glEnable(GL_DEPTH_TEST);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUseProgram(shader_program);

    glUniform1i(glGetUniformLocation(shader_program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "texture2"), 1);
    
    while (!glfwWindowShouldClose(window)) {
      process_input(window);
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gl_clear_error();


      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);
      
      CameraNS.update_frame();
      mat4 view = GLM_MAT4_IDENTITY_INIT;
      vec3 res;
      glm_vec3_add(Camera.position, Camera.front, res);

      glm_lookat(Camera.position, res, Camera.up, view);

      int view_location = glGetUniformLocation(shader_program, "view");
      glUniformMatrix4fv(view_location, 1, GL_FALSE, (float*)view);


      mat4 projection = GLM_MAT4_IDENTITY_INIT;
      glm_perspective(glm_rad(Camera.fov), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f, projection);
      int projection_location = glGetUniformLocation(shader_program, "projection");
      glUniformMatrix4fv(projection_location, 1, GL_FALSE, (float*)projection);
    

      mat4 model = GLM_MAT4_IDENTITY_INIT;
      glm_rotate(model, glm_rad(-50.0f), (vec3){0.5f, 1.0f, 0.0f});
      int model_location = glGetUniformLocation(shader_program, "model");
      glUniformMatrix4fv(model_location, 1, GL_FALSE, (float*)model);

      glBindVertexArray(VAO);
      for (unsigned int i = 0; i < 10; i++) {
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        glm_translate(model, cube_positions[i]);
        float angle = 20.0f * i;
        glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});
        glUniformMatrix4fv(model_location, 1, GL_FALSE, (float*)model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      gl_check_error();
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glDeleteProgram(shader_program);
    VertexBufferNS.delete(&vb);
  }
  glfwTerminate();

  exit(EXIT_SUCCESS);
}
