#include <dag_viewer.h>
#include <raylib.h>
#include <stdint.h>
#include <string.h>

char* PROJECT_PATH = NULL;

#include "load.h"

// TODO load project using bst show --deps none --format %{deps}

int main(int argc, char** argv) {
  PROJECT_PATH = ".";
  load_elements();

  char* src = LoadFileText(argv[1]);

  const uint32_t w = 1800;
  const uint32_t h = 900;
  bool mouse_is_down = false;
  Vector2 mouse_click_pos = {0, 0};

  InitWindow(w, h, "bst-ide");
  SetTargetFPS(60);
  dag_viewer_init((float)w, (float)h, (uint8_t*)src, strlen(src));
  while (!WindowShouldClose()) {
    mouse_click_pos = GetMousePosition();
    if (IsMouseButtonPressed(0)) {
      mouse_is_down = true;
      dag_viewer_click(mouse_click_pos.x, mouse_click_pos.y);
    }
    if (IsMouseButtonReleased(0)) {
      mouse_is_down = false;
    }
    if (!mouse_is_down) {
      dag_viewer_hover(mouse_click_pos.x, mouse_click_pos.y);
    }
    if (mouse_is_down) {
      Vector2 delta_mouse = GetMouseDelta();
      dag_viewer_drag(0 - delta_mouse.x, 0 - delta_mouse.y);
    }
    float delta_wheel = GetMouseWheelMove();
    if (delta_wheel) {
      bool direction = delta_wheel < 0;
      Vector2 pos = GetMousePosition();
      dag_viewer_zoom(pos.x, pos.y, direction);
    }
    EndDrawing();
  }

  CloseWindow();
  
  return 0;
}
