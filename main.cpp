#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "SDL_playlist.h"

#include <vld.h>

namespace {
	SDL_Window*     g_Window = NULL;
	int             g_WindowWidth = 0;
	int             g_WindowHeight = 0;
	Uint64          g_frameNo = 0;
	SDL_DisplayMode g_mode;

	SDL_Renderer*   g_Renderer = NULL;
	const Uint8*    g_KeyStates = NULL;

	bool            g_ShouldQuit = false;
}

int main(int argc, char** argv) {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096)) {
		std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Initialize SDL_ttf (for displaying current track info)
	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Create window
	g_WindowWidth = 400;
	g_WindowHeight = 300;
	g_Window = SDL_CreateWindow("SDL_playlist example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_WindowWidth, g_WindowHeight, SDL_WINDOW_SHOWN);
	if (!g_Window) {
		std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	g_KeyStates = SDL_GetKeyboardState(NULL);

	// Create renderer
	g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!g_Renderer) {
		std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Get display info
	SDL_GetDisplayMode(0, 0, &g_mode);

	// Create playlist with full options
	SDL_playlist_options options;
	options.FadeTime = 1000;
	options.Files = { "music/", "*.mp3" };
	options.Shuffle = true;
	SDL_playlist::Initialize(options);
	SDL_playlist::Start();

	// Main loop
	while (!g_ShouldQuit) {
		++g_frameNo;

		SDL_Rect screen = { 0, 0, g_WindowWidth, g_WindowHeight };
		SDL_SetRenderDrawColor(g_Renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(g_Renderer, &screen);

		SDL_RenderPresent(g_Renderer);

		/*
		// Print song details every second to the console
		if (g_frameNo % g_mode.refresh_rate == 0) {
			std::cout << (g_frameNo / g_mode.refresh_rate) << ": Playing " << SDL_playlist::CurrentTrack()->Details.Text() << std::endl;
		}
		*/

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				g_ShouldQuit = true;
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					g_ShouldQuit = true;
					break;
				case SDLK_SPACE:
					SDL_playlist::TogglePause();
					break;
				case SDLK_RIGHT:
					SDL_playlist::Next();
					break;
				case SDLK_LEFT:
					SDL_playlist::Prev();
					break;
				case SDLK_s:
					SDL_playlist::Shuffle();
				}
				break;
			}
		}
	}

	// Cleanup
	SDL_playlist::Destroy();

	TTF_Quit();
	Mix_Quit();
	SDL_Quit();

	std::cout << "Quitting" << std::endl;

	return 0;
}