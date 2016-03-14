#ifndef SDL_PLAYLIST_H_
#define SDL_PLAYLIST_H_

#include <iostream>
#include <string>
#include <map>

#include <SDL_mixer.h>

struct SDL_playlist_files {
	std::string			Directory;
	std::string			Pattern;

	std::string Path() {
		return Directory + Pattern;
	}
};

struct SDL_playlist_options {
	int					FadeTime = 1000;
	SDL_playlist_files	Files;
	bool				Shuffle = true;

	//TODO
	// OnSongChange - bool callback to determine if song change should continue
	// OnShuffle - bool callback to determine if shuffle should continue
	// OnStatusChange - callback that is passed the new status
};

struct SDL_playlist_track {
	Mix_Music*			Stream = NULL;
	std::string			Title = "Unknown Title";
	std::string			Author = "Unknown Author";
	std::string			Path;

	SDL_playlist_track(std::string filename) : Details(this) {
		Path = filename;
	}

	~SDL_playlist_track() {
		Unload();
	}

	void Load() {
		if (!Stream) {
			Stream = Mix_LoadMUS(Path.c_str());

			if (!Stream) {
				std::cerr << "Could not load music file \"" << Path << "\": " << Mix_GetError() << std::endl;
			}
		}
	}

	void Unload() {
		std::cout << "unloading " << Path << std::endl;
		if (Stream) {
			Mix_FreeMusic(Stream);
			Stream = NULL;
		}
	}

	struct Details {
		SDL_playlist_track* parent;
		Details(SDL_playlist_track* parent) : parent(parent) {}

		std::string Text() { return parent->Title + " - " + parent->Author + " (" + parent->Path + ")"; }
		// TODO option to render text to a renderer
	};

	// Expose inner structs
	Details Details;
};

enum SDL_playlist_status {
	Loading,
	Playing,
	Paused,
	Stopped
};

class SDL_playlist {
private:
	static SDL_playlist_options							mOptions;
	static std::map<unsigned, SDL_playlist_track*>		mTracks;
	static SDL_playlist_status							mStatus;
	static unsigned										mCurrentTrack;
	static Uint32										mSongStart;
	static void											mPlay();
	static void											mSongEnd();
	static void											mSetStatus(SDL_playlist_status status);

public:
	static void					Initialize(SDL_playlist_options options);
	static void					Destroy();

	static void					Start();
	static void					Pause();
	static void					Resume();
	static void					Stop();
	static void					TogglePause();
	static void					Next();
	static void					Prev();
	static void					Shuffle();

	static SDL_playlist_track*	CurrentTrack() { return mTracks[mCurrentTrack]; }
	static SDL_playlist_status	Status() { return mStatus; }
};

#endif;