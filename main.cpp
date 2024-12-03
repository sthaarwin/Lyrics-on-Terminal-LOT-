#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <regex>
#include <dirent.h>
#include <iomanip>

struct LyricLine {
    std::string text;
    int timestamp;
};

int parseTimestamp(const std::string& timestamp) {
    std::regex regex("\\[(\\d+):(\\d+\\.\\d+)\\]");
    std::smatch match;
    if (std::regex_search(timestamp, match, regex) && match.size() > 2) {
        try {
            int minutes = std::stoi(match[1]);
            double seconds = std::stod(match[2]);
            return minutes * 60 + static_cast<int>(seconds);
        } catch (const std::exception&) {
            return -1;
        }
    }
    return -1;
}

std::vector<LyricLine> loadLyrics(const std::string& filename) {
    std::vector<LyricLine> lyrics;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return lyrics;
    }

    std::string line;
    std::regex timestampRegex("\\[(\\d+):(\\d+\\.\\d+)\\]");

    while (std::getline(file, line)) {
        std::smatch match;
        if (std::regex_search(line, match, timestampRegex) && match.size() > 2) {
            std::string timestamp = match.str(0);
            std::string text = line.substr(match.position() + match.length());

            int timeInSeconds = parseTimestamp(timestamp);
            if (timeInSeconds != -1) {
                lyrics.push_back({text, timeInSeconds});
            } else {
                std::cerr << "Warning: Invalid timestamp format in line: " << line << std::endl;
            }
        }
    }
    return lyrics;
}

void banner() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    std::cout << " _______________________________________" << std::endl;
    std::cout << "|                                       | " << std::endl;
    std::cout << "|          LOT - BY ARWIN               | " << std::endl;
    std::cout << "|_______________________________________| " << std::endl;
}

std::vector<std::string> listFilesInDirectory(const std::string& directoryPath, const std::string& extension) {
    std::vector<std::string> files;
    DIR* dir = opendir(directoryPath.c_str());
    if (dir == nullptr) {
        std::cerr << "Error: Could not open directory " << directoryPath << std::endl;
        return files;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename = entry->d_name;
        if (filename.find(extension) != std::string::npos) {
            files.push_back(filename.substr(0, filename.size() - extension.size()));
        }
    }
    closedir(dir);
    return files;
}

void displaySongsInColumns(const std::vector<std::string>& songFiles, int columns) {
    if (columns <= 0) columns = 1;

    size_t numSongs = songFiles.size();
    size_t numRows = (numSongs + columns - 1) / columns;

    size_t maxWidth = 0;
    for (const auto& song : songFiles) {
        maxWidth = std::max(maxWidth, song.length());
    }

    for (size_t row = 0; row < numRows; ++row) {
        for (int col = 0; col < columns; ++col) {
            size_t index = row + col * numRows;
            if (index < numSongs) {
                std::cout << std::right << std::setw(3) << index + 1 << ". "
                          << std::left << std::setw(maxWidth + 2) << songFiles[index];
            }
        }
        std::cout << std::endl;
    }
}

void playAudio(const std::string& musicFile) {
    std::string command = "ffplay \"" + musicFile + "\" -nodisp -autoexit -loglevel quiet";
    system(command.c_str());
}

void typeInText(const std::string& text, int delayMs) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    std::cout << std::endl;
}

int main() {
    banner();

    std::string songsDir = "songs";
    std::string lyricsDir = "lyrics";
    std::string extension = ".mp3";

    auto songFiles = listFilesInDirectory(songsDir, extension);

    if (songFiles.empty()) {
        std::cerr << "No songs found in the directory." << std::endl;
        return 1;
    }

    std::cout << "Available songs:" << std::endl;
    displaySongsInColumns(songFiles, 3);

    size_t choice;
    std::cout << "Enter the number of the song you want to play: ";
    if (!(std::cin >> choice) || choice < 1 || choice > songFiles.size()) {
        std::cerr << "Invalid choice." << std::endl;
        return 1;
    }
    system("clear");
    banner();
    std::string selectedSong = songFiles[choice - 1];
    std::string musicFile = songsDir + "/" + selectedSong + extension;
    std::string lyricsFile = lyricsDir + "/" + selectedSong + ".lrc";

    auto lyrics = loadLyrics(lyricsFile);
    if (lyrics.empty()) {
        std::cerr << "No lyrics found for the selected song." << std::endl;
        return 1;
    }

    std::thread audioThread(playAudio, musicFile);

    auto startTime = std::chrono::steady_clock::now();
    size_t currentLyricIndex = 0;

    while (audioThread.joinable()) {
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = currentTime - startTime;

        while (currentLyricIndex < lyrics.size() && lyrics[currentLyricIndex].timestamp <= static_cast<int>(elapsed.count())) {
            std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Added delay
            typeInText(lyrics[currentLyricIndex].text, 50);
            currentLyricIndex++;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    audioThread.join();
    
    return 0;
}
