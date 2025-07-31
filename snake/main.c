#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_shape.h>
#include <stdio.h>
#include <stdbool.h>
#include <wchar.h>

typedef enum {
  DIR_UP,
  DIR_DOWN,
  DIR_LEFT,
  DIR_RIGHT,
} direction_e;

typedef struct {
  int x;
  int y;
} vec2_t;

// Render constants
static const int SCREEN_WIDTH	= 801;
static const int SCREEN_HEIGHT = 800;
static const uint16_t GRID_SIZE = 25;

static const SDL_Color COLOR_BG_BLACK    = {0x00, 0x00, 0x00, 0x00};
static const SDL_Color COLOR_BG_GREY     = {0x10, 0x10, 0x10, 0x00};
static const SDL_Color COLOR_PLAYER_HEAD = {0x00, 0x00, 0xFF, 0x00};
static const SDL_Color COLOR_PLAYER_TAIL = {0x00, 0x00, 0xFF, 0x00};
static const SDL_Color COLOR_FRUIT       = {0xFF, 0x00, 0x00, 0x00};

static struct {
  SDL_Window* window;
  SDL_Surface* surface;
  SDL_Event event;
  bool quit_window;
  uint32_t frame_count;
} render = {
  .window = NULL,
  .surface = NULL,
  .quit_window = false,
  .frame_count = 0
};

//  struct {
//   SDL_Window* window;
//   SDL_Surface* surface;
//   SDL_Event;
//   bool quit_window;
// } render_t;

// render_t render = {
//   .window = NULL,
//   .surface = NULL,
//   .quit_window = false
// };

// Render functions
int render_init(void);
void render_handle_events(void);
void render_background(void);
void render_quit(void);



// Game 
static const vec2_t HEAD_COORD_INIT = {4 * GRID_SIZE, 0};
static vec2_t head_coord;
static vec2_t last_head_coord;
static vec2_t fruit_coord;
static vec2_t tail_coords[32 * 32 - 1];
static size_t tail_count = 4;
static uint16_t delay_duration_ms = 400;
direction_e last_dir = DIR_RIGHT;


// Game functions
void game_on_init(void);
void tail_init();
void tail_update();
void tail_draw();
void fruit_draw(void);
void fruit_generate(void);
void update_input(vec2_t *rect);


void game_on_init(void) {
  head_coord = HEAD_COORD_INIT;
  fruit_generate();
  // tail_coords[0] = (vec2_t){HEAD_COORD_INIT.x + 1 * GRID_SIZE, HEAD_COORD_INIT.y};
  // tail_coords[1] = (vec2_t){HEAD_COORD_INIT.x - 0 * GRID_SIZE, HEAD_COORD_INIT.y};
  // tail_coords[2] = (vec2_t){HEAD_COORD_INIT.x - 1 * GRID_SIZE, HEAD_COORD_INIT.y};
}

void tail_draw() {
  for (size_t i = 0; i < tail_count; i++) {
    SDL_FillRect(
      render.surface,
      &(SDL_Rect){tail_coords[i].x, tail_coords[i].y, GRID_SIZE, GRID_SIZE},
      SDL_MapRGB(render.surface->format, COLOR_PLAYER_TAIL.r, COLOR_PLAYER_TAIL.g, COLOR_PLAYER_TAIL.b)
    );
  }
}

void fruit_draw(void) {
  SDL_FillRect(
    render.surface,
    &(SDL_Rect){fruit_coord.x, fruit_coord.y, GRID_SIZE, GRID_SIZE},
    SDL_MapRGB(render.surface->format, COLOR_FRUIT.r, COLOR_FRUIT.g, COLOR_FRUIT.b)
  );
}

void tail_update() {
  vec2_t tmp = tail_coords[0];
  tail_coords[0] = last_head_coord;

  /* vec2_t tmp = last_head_coord; */

  for (size_t i = 1; i < tail_count; i++) {
    tail_coords[i] = tmp;
    tail_coords[i + 1] = tail_coords[i];
    tmp = tail_coords[i + 1];
  }
}

// ----------------------------------------------------------------------------
// render
// ----------------------------------------------------------------------------

int render_init(void) {
  if (SDL_Init( SDL_INIT_VIDEO ) < 0)	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
  render.window = SDL_CreateWindow(
    "SNAKE webOS",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN
  );
  if ( render.window == NULL )	{
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }
  //	Get window surface
  render.surface = SDL_GetWindowSurface(render.window);
}

void render_handle_events(void) {
  while (SDL_PollEvent(&render.event) != 0)	{
    switch(render.event.type) {
      case SDL_KEYDOWN:
        switch(render.event.key.keysym.sym) {
          case SDLK_UP:
            last_dir = DIR_UP;
            break;
          case SDLK_DOWN:
            last_dir = DIR_DOWN;
            break;
          case SDLK_LEFT:
            last_dir = DIR_LEFT;
            break;
          case SDLK_RIGHT:
            last_dir = DIR_RIGHT;
            break;
        }
        break;
      case SDL_QUIT:
        render.quit_window = true;
        break;
    }
  }
};

int main(void) {
	int ec = render_init();
  if (ec != 0) return ec;
  
  game_on_init();

  while (!render.quit_window) {

    printf("%i\n", render.frame_count++);
    
    render_handle_events();

    last_head_coord = head_coord;
    update_input(&head_coord);
    tail_update();
    
    render_background();

    SDL_Rect player_head = {head_coord.x, head_coord.y, 25, 25};

    tail_draw();
    fruit_draw();
    SDL_FillRect(
      render.surface,
      &player_head,
      SDL_MapRGB(render.surface->format, COLOR_FRUIT.r, COLOR_FRUIT.g, COLOR_FRUIT.b)
    );

    SDL_UpdateWindowSurface( render.window );
    SDL_Delay(delay_duration_ms);
    // delay_duration_ms -= 20;
  }

  /// end

  render_quit();
	return EXIT_SUCCESS;
}

/// ---------------------------------------------------------------------------

void render_quit(void) {
  SDL_DestroyWindow( render.window );
  SDL_Quit();
}

void render_background(void) {
  // SDL_Rect bg_rect = {0, 0, GRID_SIZE, GRID_SIZE};
  // for (size_t i = 0; i < 32; i++) {
  //   for (size_t j = 0; j < 32; j++) {
  //     bg_rect.x += GRID_SIZE;
  //     const SDL_Color color = (j % 2 == 0) ? COLOR_BG_GREY : COLOR_BG_BLACK;
  //     SDL_FillRect(
  //       surface,
  //       &bg_rect,
  //       SDL_MapRGB(surface->format, color.r, color.g, color.b)
  //     );
  //   }
  //   bg_rect.y += GRID_SIZE;
  //   bg_rect.x = (i % 2 == 0) ? GRID_SIZE : 0;
  // }

  SDL_FillRect(
      render.surface,
      NULL,
      SDL_MapRGB(render.surface->format, COLOR_BG_BLACK.r, COLOR_BG_BLACK.g, COLOR_BG_BLACK.b)
    );
}

void update_input(vec2_t *rect) {
  switch (last_dir) {
    case DIR_UP:
      rect -> y -= GRID_SIZE;
      break;
    case DIR_DOWN:
      rect -> y += GRID_SIZE;
      break;
    case DIR_LEFT:
      rect -> x -= GRID_SIZE;
      break;
    case DIR_RIGHT:
      rect -> x += GRID_SIZE;
      break;
  }
}

void fruit_generate(void) {
  fruit_coord = (vec2_t){0,0};
}
