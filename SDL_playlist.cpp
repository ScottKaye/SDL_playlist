#ifndef SDL_PLAYLIST_C_
#define SDL_PLAYLIST_C_

#include <SDL.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "SDL_playlist.h"

// Windows-specific
#include <atlstr.h>
#include <Windows.h>

// Externals
SDL_playlist_options                        SDL_playlist::mOptions;
std::map<unsigned, SDL_playlist_track*>     SDL_playlist::mTracks;
SDL_playlist_status                         SDL_playlist::mStatus = SDL_playlist_status::Loading;
unsigned                                    SDL_playlist::mCurrentTrack = 0;
Uint32                                      SDL_playlist::mSongStart = 0;

void SDL_playlist::Initialize(SDL_playlist_options options) {
	mOptions = options;
	mStatus = SDL_playlist_status::Loading;

	// Load files found from options.Files pattern
	HANDLE hFind;
	WIN32_FIND_DATA data;
	std::string test = options.Files.Path();
	std::wstring stemp = std::wstring(test.begin(), test.end());
	LPCWSTR path = stemp.c_str();
	std::vector<std::string> filenames;

	hFind = FindFirstFile(path, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			std::string filename = CW2A(data.cFileName);
			filenames.push_back(options.Files.Directory + filename);
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}

	// Load filenames into playlist tracks
	for (unsigned i = 0; i < filenames.size(); ++i) {
		SDL_playlist_track* track = new SDL_playlist_track(filenames[i]);
		mTracks.insert(std::pair<int, SDL_playlist_track*>(i, track));
	}

	// Sort filenames
	if (options.Shuffle) {
		Shuffle();
	}

	// Play next track when song ends
	Mix_HookMusicFinished(mSongEnd);
}

void SDL_playlist::Destroy() {
	Mix_HaltMusic();

	for (auto i : mTracks) {
		i.second->Unload();
		delete i.second;
	}

	mTracks.clear();
}

void SDL_playlist::Shuffle() {
	if (mTracks.size() == 0) return;

	std::vector<unsigned> keys;
	for (auto i : mTracks) {
		keys.push_back(i.first);
	}

	std::random_shuffle(keys.begin(), keys.end());

	std::vector<unsigned>::iterator it = keys.begin();
	for (auto& i : mTracks) {
		SDL_playlist_track* ts = i.second;
		i.second = mTracks[*it];
		mTracks[*it] = ts;
		++it;
	}
}

void SDL_playlist::mPlay() {
	std::cout << " *** PLAYING - " << mTracks[mCurrentTrack]->Path << std::endl;

	Resume();
	mSongStart = SDL_GetTicks();
	mTracks[mCurrentTrack]->Load();
	Mix_FadeInMusic(mTracks[mCurrentTrack]->Stream, 1, mOptions.FadeTime);
}

void SDL_playlist::mSongEnd() {
	if (SDL_GetTicks() - mSongStart > 500) {
		Next();
	}
	else {
		mPlay();
	}
}

void SDL_playlist::Start() {
	mStatus = SDL_playlist_status::Playing;

	if (mTracks.size() == 0) {
		std::cerr << "No tracks loaded." << std::endl;
		return;
	}

	mPlay();
}

void SDL_playlist::Pause() {
	mStatus = SDL_playlist_status::Paused;
	Mix_PauseMusic();
}

void SDL_playlist::Resume() {
	mStatus = SDL_playlist_status::Playing;
	Mix_ResumeMusic();
}

void SDL_playlist::Stop() {
	mStatus = SDL_playlist_status::Stopped;
	mCurrentTrack = 0;
	Mix_HookMusicFinished(NULL);
	Mix_HaltMusic();
}

void SDL_playlist::TogglePause() {
	if (mStatus == SDL_playlist_status::Playing) {
		Pause();
	}
	else Resume();
}

void SDL_playlist::Next() {
	mStatus = SDL_playlist_status::Loading;

	// Unload last track (2 tracks ago)
	if (mCurrentTrack >= 2) {
		mTracks[mCurrentTrack - 1]->Unload();
	}

	++mCurrentTrack;
	if (mCurrentTrack >= mTracks.size()) {
		mCurrentTrack = 0;
		Shuffle();
	}

	mPlay();
}

void SDL_playlist::Prev() {
	mStatus = SDL_playlist_status::Loading;

	if (mCurrentTrack == 0) {
		mCurrentTrack = mTracks.size() - 1;
		Shuffle();
	}
	else {
		--mCurrentTrack;
	}

	mPlay();
}

void SDL_playlist::mSetStatus(SDL_playlist_status status) {
	mStatus = status;

	//TODO call callback
}

#endif;