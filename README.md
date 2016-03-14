# Intro
SDL_playlist is a small class that automatically loads and manages music files for your game.  This repo comes with `main.cpp` to serve as an example, and is of course not necessary for actual usage.

# Usage
1. Add both `SDL_playlist.h` and `SDL_playlist.cpp` to your project
5. Add `#include "SDL_playlist.h"` in your main file (or wherever you need to control the playlist)
42. Create an `SDL_playlist_options` struct and fill in the values you need
3. Call `SDL_playlist::Initialize` with your options
	- Once you do this, make sure you put `SDL_playlist::Destroy` in your cleanup routine!
7. To start playing whatever is in the playlist, call `SDL_playlist::Start`

# Methods
SDL_playlist has the following static methods.

<!-- seriously -->
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Signature&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|Description
---|---
`void Initialize(SDL_playlist_options options)`|Initializes the playlist and sets up callbacks
`void Destroy()`|Unloads all music files and cleans up
`void Start()`|Starts playing the playlist
`void Pause()`|Pauses music
`void Resume()`|I'm really not sure
`void TogglePause()`|If the playlist is paused, play, if it's playing, pause
`void Stop()`|Stops the playlist - if resumed, it will start from the beginning without reshuffling
`void Next()`|Plays the next song in the playlist.  If it has reached the end, it will reshuffle and start over
`void Prev()`|Plays the previous song in the playlist.  If it has reached the beginning, it will reshuffle and start at the end
`void Shuffle()`|Shuffles the entire playlist
`SDL_playlist_track* CurrentTrack()`|Returns the internal struct used for the current track
`SDL_playlist_status Status()`|Returns what the playlist is doing (usually will just say `Playing`)

# Limitations
Currently SDL_playlist is **Windows only!**  This is something I want to change, but have not gotten around to doing yet.  Please feel free to submit a PR if you get this done!