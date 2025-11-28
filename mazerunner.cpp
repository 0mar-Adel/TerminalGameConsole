#include "mazerunner.hpp"
#include "generalutils.hpp"
#include "vectors.hpp"
#include <chrono>
#include <iostream>
#include <vector>
void render_pixel(int x, int y, int width, int height,
                  std::string &screen_buffer, SH::vec2 &CENTER_PIXEL) {
  SH::vec2 CURRENT_PIXEL((float)x, (float)y);
  int r, g, b;
  r = SH::distance(CENTER_PIXEL, CURRENT_PIXEL);
  g = r;
  b = r;
  if (x == CENTER_PIXEL.x && y == CENTER_PIXEL.y) {
    r = 0;
    g = 0;
    b = 255;
  }
  color_pixel(screen_buffer, r, g, b);
}
void color_pixel(std::string &buffer, int r, int g, int b) {

  buffer += "\033[38;2;";
  buffer += std::to_string(r);
  buffer += ";";
  buffer += std::to_string(g);
  buffer += ";";
  buffer += std::to_string(b);
  buffer += "m\033[48;2;";
  buffer += std::to_string(r);
  buffer += ";";
  buffer += std::to_string(g);
  buffer += ";";
  buffer += std::to_string(b);
  buffer += "mC\033[m";
}
void clear_buffer(std::string &screen_buffer) {
  Clear_Terminal();
  screen_buffer = "";
}
void init_mazerunner() {
  int width;
  int height;
  std::vector<int> mslist = {};
  std::string screen_buffer = "";
  bool running = true;
  while (running) {
    auto t1 = std::chrono::high_resolution_clock::now();
    clear_buffer(screen_buffer);
    get_terminal_size(width, height);
    SH::vec2 CENTER_PIXEL((float)(width / 2), (float)(height / 2));
    for (int h = 0; h < height; h++) {
      for (int w = 0; w < width; w++) {
        render_pixel(w, h, width, height, screen_buffer, CENTER_PIXEL);
      }
    }
    std::cout << screen_buffer;
    auto t2 = std::chrono::high_resolution_clock::now();
    int ms = (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1))
                 .count();
    mslist.push_back(ms);
    float fps = (float)ms;
    int quit;
    std::cin >> quit;
    if (quit > 0) {
      for (int mis : mslist) {
        fps = ((float)(fps + mis)) / 2.0;
      }
      std::cout << "\n" << fps << "\n" << width << "\n" << height << "\n";
      return;
    }
  }
}
