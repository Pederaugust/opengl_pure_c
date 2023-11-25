#include "shader.h"

char* read_shader_file(const char* file_name, long* shader_code_length) {
  FILE * shader_file;
  char* shader_code = NULL;

  shader_file = fopen(file_name, "r");
    
  if (shader_file) {
    fseek(shader_file, 0, SEEK_END);
    *shader_code_length = ftell(shader_file);

    rewind(shader_file);

    shader_code = (char*) malloc(sizeof(char) * *shader_code_length);
    fread(shader_code, 1, *shader_code_length, shader_file);
    fclose(shader_file);

    return shader_code;
  } else {
    fprintf(stderr, "Failed to open shader file.\n");
    return NULL;
  }
}

void compile_and_check_shader(GLuint shader_id, char* shader_code, long shader_code_length) {
  int log_length;
  GLint result = GL_FALSE;
  
  glShaderSource(shader_id, 1, (const GLchar**)&shader_code, (const GLint*)&shader_code_length);
  glCompileShader(shader_id);

  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

  if (log_length > 0) {
    char * err_message = (char*) malloc(sizeof(char) * log_length);
    glGetShaderInfoLog(shader_id, log_length, NULL, err_message);
    fprintf(stderr, "%s\n", err_message);
    free(err_message);
  }
}

GLuint load_shaders(const char * vertex_file_path, const char * fragment_file_path) {
  char * vertex_shader_code = NULL;
  long vertex_shader_code_length = 0;
  char * fragment_shader_code = NULL;
  long fragment_shader_code_length = 0;
  int log_length;
  GLint result = GL_FALSE;

  GLuint program_id = glCreateProgram();
  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  
  
  if (vertex_shader_id == 0 || fragment_shader_id == 0) {
    fprintf(stderr, "Failed to create shader.\n");
    return 0;
  }

  if ((vertex_shader_code = read_shader_file(vertex_file_path, &vertex_shader_code_length)) == NULL) {
    free(vertex_shader_code);
    exit(EXIT_FAILURE);
  }
  if ((fragment_shader_code = read_shader_file(fragment_file_path, &fragment_shader_code_length)) == NULL) {
    free(vertex_shader_code);
    exit(EXIT_FAILURE);
  }

  printf("Vertex shader code:\n%s\n", vertex_shader_code);
  printf("Fragment shader code:\n%s\n", fragment_shader_code);

  compile_and_check_shader(vertex_shader_id, vertex_shader_code, vertex_shader_code_length);
  compile_and_check_shader(fragment_shader_id, fragment_shader_code, fragment_shader_code_length);
  free(vertex_shader_code);
  free(fragment_shader_code);
  

  // Link the program
  printf("Linking program\n");
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  // Check the program
  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

  if (log_length > 0) {
    char * err = (char*) malloc(sizeof(char) * log_length);
    glGetProgramInfoLog(program_id, log_length, NULL, err);
    fprintf(stderr, "%s\n", err);
    free(err);
  }


  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  return program_id;
}



Shader new(const char *vertexPath, const char *fragmentPath) {
  Shader shader;
  shader.id = load_shaders(vertexPath, fragmentPath);
  return shader;
}

void use(Shader shader) {
  glUseProgram(shader.id);
}

void setInt(Shader shader, const char *name, int value) {
  glUniform1i(glGetUniformLocation(shader.id, name), value);
}

void setFloat(Shader shader, const char *name, float value) {
  glUniform1f(glGetUniformLocation(shader.id, name), value);
}

void setVec3(Shader shader, const char *name, vec3 value) {
  glUniform3f(glGetUniformLocation(shader.id, name), value[0], value[1], value[2]);
}

void setMat4(Shader shader, const char *name, mat4 value) {
  glUniformMatrix4fv(glGetUniformLocation(shader.id, name), 1, GL_FALSE, value);
}

void delete(Shader shader) {
  glDeleteProgram(shader.id);
}

ShaderNS_t ShaderNS = {
  .new = new,
  .use = use,
  .setInt = setInt,
  .setFloat = setFloat,
  .setMat4 = setMat4,
  .setVec3 = setVec3,
  .delete = delete
};
