#include <SDL2/SDL.h>

#include <SDL2/SDL_shape.h>
#include <stdio.h>
#include <stdbool.h>

//Screen dimension constants
const int SCREEN_WIDTH	= 800;
const int SCREEN_HEIGHT = 800;

static SDL_Window* window = NULL;
SDL_Surface* surface= NULL;

static const SDL_Color COLOR_BG_BLACK    = {0x00, 0x00, 0x00, 0x00};
static const SDL_Color COLOR_BG_GREY     = {0x10, 0x10, 0x10, 0x00};

static const SDL_Color COLOR_PLAYER_HEAD = {0x00, 0x00, 0xFF, 0x00};
static const SDL_Color COLOR_PLAYER_TAIL = {0x00, 0xFF, 0x00, 0x00};
static const SDL_Color COLOR_FRUIT       = {0xFF, 0x00, 0x00, 0x00};

static const uint16_t GRID_SIZE = 25;


static bool quit_window = false;

void quit_sdl();

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

  SDL_Event e;

  SDL_Rect test_rect = {0, 0, 25, 25};
  int count;

  while (!quit_window) {

    printf("%i\n", count++);
    
    while ( SDL_PollEvent(&e) != 0 )	{
      switch(e.type) {
        case SDL_QUIT:
          quit_window = true;
          break;
      }
    }

    test_rect.x += GRID_SIZE;

    SDL_Rect bg_rect = {0, 0, GRID_SIZE, GRID_SIZE};
    /* bool s1 = false; */

    for (size_t i = 0; i < 32; i++) {
      for (size_t j = 0; j < 32; j++) {
        bg_rect.x += GRID_SIZE;
        const SDL_Color color = (j % 2 == 0) ? COLOR_BG_GREY : COLOR_BG_BLACK;
        SDL_FillRect(
          surface,
          &bg_rect,
          SDL_MapRGB(surface->format, color.r, color.g, color.b)
        );
        SDL_UpdateWindowSurface( window );
        SDL_Delay(100);
        printf("i:%zu j:%zu\n", i, j);
        printf("x:%i y:%i\n", bg_rect.x, bg_rect.y);
      }
      bg_rect.y += GRID_SIZE;
      bg_rect.x = (i % 2 == 0) ? GRID_SIZE : 0;
    }


    SDL_FillRect(
      surface,
      &test_rect,
      SDL_MapRGB(surface->format, COLOR_FRUIT.r, COLOR_FRUIT.g, COLOR_FRUIT.b)
    );

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
