# Lyrics on Terminal (LOT)

## Description

This project is a simple terminal-based lyrics and music player. It allows you to select a song from a list, play the song, and display the lyrics in sync with the music.

## Features

- List available songs
- Play selected song
- Display lyrics with type-in animation, synchronized with the music

## Setup

### Prerequisites

- `ffplay` (part of `ffmpeg` package) for audio playback
- A C++ compiler (GCC recommended)
- Standard C++ library

### Installation

1. Clone the repository:
   ```sh
   git clone https://github.com/sthaarwin/Lyrics-on-Terminal-LOT-.git
   cd Lyrics-on-Terminal-LOT-
2. Compile the code:
   ```sh
   g++ -o lyrics_player lyrics_player.cpp -std=c++17

## Usage

1. Place your MP3 files in the `songs` folder.

2. Place the corresponding LRC files in the `lyrics` folder.

3. Run the compiled application:
   ```sh
   ./lyrics_player
4. Follow the prompts to select and play a song.

## Contribution Guidelines

To contribute to the project, please follow these guidelines:

### Adding Songs and Lyrics

- Add your MP3 file to the `songs` folder. The filename should follow the format:
  ```sh
  Artist - Songname.mp3

- Add the corresponding LRC file to the `lyrics` folder. The filename should match the MP3 file name, but with an `.lrc` extension. For example:
  ```sh
  Artist - Songname.lrc

- Ensure that the song and lyrics files have the same base name, differing only in extension.

  ### Creating Pull Requests

- Fork the repository and clone your fork.
- Make your changes (add your song and lyrics files).
- Commit your changes and push them to your fork.
- Create a pull request from your fork to the main repository.

### Code Style

- Follow the existing code style for consistency.
- Ensure your changes do not break existing functionality.
