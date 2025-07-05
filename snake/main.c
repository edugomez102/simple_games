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

static const int SCREEN_WIDTH	= 801;
static const int SCREEN_HEIGHT = 800;

static const SDL_Color COLOR_BG_BLACK    = {0x00, 0x00, 0x00, 0x00};
static const SDL_Color COLOR_BG_GREY     = {0x10, 0x10, 0x10, 0x00};


static const SDL_Color COLOR_PLAYER_HEAD = {0x00, 0x00, 0xFF, 0x00};
static const SDL_Color COLOR_PLAYER_TAIL = {0x00, 0x00, 0xFF, 0x00};
static const SDL_Color COLOR_FRUIT       = {0xFF, 0x00, 0x00, 0x00};

static const uint16_t GRID_SIZE = 25;

// static variables
static int frame_count;

static SDL_Window* window = NULL;
static SDL_Surface* surface= NULL;
static SDL_Event e;

static bool quit_window = false;

direction_e last_dir = DIR_RIGHT;

const vec2_t HEAD_COORD_INIT = {4 * GRID_SIZE, 0};

vec2_t head_coord = HEAD_COORD_INIT;
vec2_t last_head_coord;
/* vec2_t tail_coords[32 * 32 - 1] = {0}; */

vec2_t tail_coords[32 * 32 - 1] = {
  {HEAD_COORD_INIT.x - -1 * GRID_SIZE, HEAD_COORD_INIT.y},
  {HEAD_COORD_INIT.x - 0 * GRID_SIZE, HEAD_COORD_INIT.y},
  {HEAD_COORD_INIT.x - 1 * GRID_SIZE, HEAD_COORD_INIT.y},
};

static uint16_t delay_duration_ms = 400;


static size_t tail_count = 4;

void tail_init();
void tail_update();
void tail_draw();

void quit_sdl();
void draw_background();
void update_input(vec2_t *rect);

// ...x
// [4, 0]
// ---x
// ..........x
//        ---x
//

void tail_draw() {
  for (size_t i = 0; i < tail_count; i++) {
    SDL_FillRect(
      surface,
      &(SDL_Rect){tail_coords[i].x, tail_coords[i].y, GRID_SIZE, GRID_SIZE},
      SDL_MapRGB(surface->format, COLOR_PLAYER_TAIL.r, COLOR_PLAYER_TAIL.g, COLOR_PLAYER_TAIL.b)
    );
  }
}

// ---x
// [0,0] [1,0] [2,0] 

//       [  ] [2,1]
//
// 

void shift_right(vec2_t* start, vec2_t* end, const size_t dif)
{
  for (size_t i = (end - start) - 1 ; i > dif - 1 ; i--)
    start[i] = start[i - dif];
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


int main(void) {
	if (SDL_Init( SDL_INIT_VIDEO ) < 0)	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
  window = SDL_CreateWindow(
    "SNAKE WEbos",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN
  );
  if ( window == NULL )	{
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }
  //	Get window surface
  surface = SDL_GetWindowSurface(window);


  while (!quit_window) {

    printf("%i\n", frame_count++);
    
    while (SDL_PollEvent(&e) != 0)	{
      switch(e.type) {
        case SDL_KEYDOWN:
          switch(e.key.keysym.sym) {
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
          quit_window = true;
          break;
      }
    }

    last_head_coord = head_coord;
    update_input(&head_coord);
    tail_update();
    
    draw_background();

    SDL_Rect player_head = {head_coord.x, head_coord.y, 25, 25};

    tail_draw();
    SDL_FillRect(
      surface,
      &player_head,
      SDL_MapRGB(surface->format, COLOR_FRUIT.r, COLOR_FRUIT.g, COLOR_FRUIT.b)
    );

    SDL_UpdateWindowSurface( window );
    SDL_Delay(delay_duration_ms);
    delay_duration_ms -= 20;
  }

  /// end

  quit_sdl();
	return EXIT_SUCCESS;
}

/// ---------------------------------------------------------------------------

void quit_sdl(void) {
  SDL_DestroyWindow( window );
  SDL_Quit();
}

void draw_background() {
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
      surface,
      NULL,
      SDL_MapRGB(surface->format, COLOR_BG_BLACK.r, COLOR_BG_BLACK.g, COLOR_BG_BLACK.b)
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
