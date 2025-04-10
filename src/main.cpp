#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <vector>

std::string readShaderFile(const char *filePath) {
  std::ifstream file(filePath, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open shader file: " +
                             std::string(filePath));
  }
  size_t fileSize = file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();
  return std::string(buffer.begin(), buffer.end());
}

unsigned int compileShader(const char *source, unsigned int type) {
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  return shader;
}

unsigned int createShaderProgram(const char *vertexShaderSource,
                                 const char *fragmentShaderSource) {
  unsigned int vertexShader =
      compileShader(vertexShaderSource, GL_VERTEX_SHADER);
  unsigned int fragmentShader =
      compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

int main() {
  const int WIDTH = 1280;
  const int HEIGHT = 1440;

  GLFWwindow *window;

  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW!" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window =
      glfwCreateWindow(WIDTH, HEIGHT, "Highlands ~> Main", nullptr, nullptr);
  if (!window) {
    std::cerr << "Error! Window Null!" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // Initialize GLAD after creating the window
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD!" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

  // Now that we have a window, we can initialize ImGui
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard navigation

  // Initialize ImGui OpenGL3 backend
  ImGui_ImplOpenGL3_Init("#version 130");
  ImGui_ImplGlfw_InitForOpenGL(window, true);

  io.DisplaySize = ImVec2((float)WIDTH, (float)HEIGHT);

  glViewport(0, 0, WIDTH, HEIGHT);

  float vertices[] = {
      // First triangle
      0.5f, 0.5f, 0.0f,  // Top right
      -0.5f, 0.5f, 0.0f, // Top left
      0.5f, -0.5f, 0.0f, // Bottom right

      // Second triangle
      -0.5f, 0.5f, 0.0f,  // Top left
      -0.5f, -0.5f, 0.0f, // Bottom left
      0.5f, -0.5f, 0.0f,  // Bottom right
  };

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Load shaders from file
  std::string vertexShaderSource =
      readShaderFile("resources/shaders/shader.vert");
  std::string fragmentShaderSource =
      readShaderFile("resources/shaders/shader.frag");

  unsigned int shaderProgram = createShaderProgram(
      vertexShaderSource.c_str(), fragmentShaderSource.c_str());

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glClearColor(0.175f, 0.175f, 0.521f, 1.0f); // Set background to dark blue

  bool render = true;

  while (!glfwWindowShouldClose(window)) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Test");
    if (ImGui::Button("Toggle Render")) {
      if (render) {
        render = false;
      } else {
        render = true;
      }
    }
    ImGui::End();

    glClear(GL_COLOR_BUFFER_BIT);

    if (render) {
      glUseProgram(shaderProgram);
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
