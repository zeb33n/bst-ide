#include "utils.h"
#include <dag_viewer.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char* PROJECT_PATH = NULL;

#include "load.h"

// TODO load project using bst show --deps none --format %{deps}

int main(int argc, char** argv) {
  if (argc == 1) {
    printf("%s Expected A Path to a Buildstream Project\n", ERROR_MSG);
    exit(1);
  }

  PROJECT_PATH = string_format_dynalloc("%s", argv[1]);
  load_elements();


  const uint32_t w = 1800;
  const uint32_t h = 900;
  bool mouse_is_down = false;
  Vector2 mouse_click_pos = {0, 0};

  dag_viewer_new((float)w, (float)h);
  create_graph();

  InitWindow(w, h, "bst-ide");
  SetTargetFPS(60);

  dag_viewer_draw();

  if (argc > 2) {
    printf("%s\n", argv[2]);
    dag_viewer_highlight_bicone((uint8_t*)argv[2], strlen(argv[2]));
  }

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
