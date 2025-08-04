// emoji_generator.cpp
#include <iostream>
#include <string>
#include <vector>
#include <fstream>  // For file output
#include <chrono>   // For time
#include <ctime>    // For time formatting
#include <cstdlib>

// Function to get a formatted timestamp string
std::string getCurrentTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto in_time_t = std::chrono::system_clock::to_time_t(now);
    char buffer[22];
    strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", std::localtime(&in_time_t));
    return std::string(buffer);
}

// --- Emoji Data Sets ---
const std::vector<std::string> EMOJI_SET = { "😀", "😂", "🥰", "😎", "🤔", "😴", "🥳", "🤯", "😡", "😭", "👍", "👎", "🙏", "💪", "👀", "🧠", "🔥", "💯", "🚀", "🎉", "❤️", "💔", "⭐️", "✨", "☀️", "🌙", "🌍", "✈️", "🚗", "💻", "🐶", "🐱", "🐭", "🦊", "🐻", "🐼", "🐨", "🦁", "🐸", "🐢", "🍕", "🍔", "🍓", "🥑", "☕️", "🍺", "📚", "🎸", "⚽️", "🏆" };
const std::vector<std::string> FACE_EMOJIS = {"😀", "😂", "🥰", "😎", "🤔", "😴", "🥳", "🤯", "😡", "😭"};
const std::vector<std::string> OBJECT_EMOJIS = {"🍕", "🍔", "🍓", "🥑", "☕️", "🍺", "📚", "🎸", "⚽️", "🏆"};

// Helper to get a random index for the main EMOJI_SET
int randomIndex() {
    return rand() % EMOJI_SET.size();
}

// --- Emoji Sequence Generation Algorithms ---

// 1. Random Style
std::string generateRandomSequence() {
    std::string resultSequence = "";
    for (int i = 0; i < 8; ++i) {
        resultSequence += EMOJI_SET[randomIndex()];
    }
    return resultSequence;
}

// 2. Checksum Style
std::string generateChecksumSequence() {
    std::string resultSequence = "";
    int sumOfIndices = 0;
    for (int i = 0; i < 7; ++i) {
        int index = randomIndex();
        resultSequence += EMOJI_SET[index];
        sumOfIndices += index;
    }
    int checksumIndex = sumOfIndices % EMOJI_SET.size();
    resultSequence += EMOJI_SET[checksumIndex];
    return resultSequence;
}

// 3. Paired Style
std::string generatePairedSequence() {
    std::string resultSequence = "";
    const int offset = 5;
    for (int i = 0; i < 4; ++i) {
        int firstIndex = randomIndex();
        int secondIndex = (firstIndex + offset) % EMOJI_SET.size();
        resultSequence += EMOJI_SET[firstIndex];
        resultSequence += EMOJI_SET[secondIndex];
    }
    return resultSequence;
}

// 4. Mirrored Style
std::string generateMirroredSequence() {
    std::string resultSequence = "";
    std::vector<int> firstHalfIndices;
    for (int i = 0; i < 4; ++i) {
        int index = randomIndex();
        resultSequence += EMOJI_SET[index];
        firstHalfIndices.push_back(index);
    }
    for (int i = 3; i >= 0; --i) {
        int mirroredIndex = (EMOJI_SET.size() - 1) - firstHalfIndices[i];
        resultSequence += EMOJI_SET[mirroredIndex];
    }
    return resultSequence;
}

// 5. Alternating Categories Style
std::string generateAlternatingCategorySequence() {
    std::string resultSequence = "";
    for (int i = 0; i < 8; ++i) {
        if (i % 2 == 0) {
            resultSequence += FACE_EMOJIS[rand() % FACE_EMOJIS.size()];
        } else {
            resultSequence += OBJECT_EMOJIS[rand() % OBJECT_EMOJIS.size()];
        }
    }
    return resultSequence;
}


int main() {
    #if defined(_WIN32)
        system("chcp 65001 > nul");
    #endif
    srand(time(0));
    
    std::ofstream logFile("emoji.log", std::ios_base::app);

    int choice, n;
    std::cout << "Select an algorithm style for emoji sequence generation:\n";
    std::cout << "1. Random Style\n";
    std::cout << "2. Checksum Style\n";
    std::cout << "3. Paired Style\n";
    std::cout << "4. Mirrored Style\n";
    std::cout << "5. Alternating Categories Style (Face/Object)\n";
    std::cout << "Enter your choice (1-5): ";
    std::cin >> choice;

    std::cout << "How many sequences would you like to generate? ";
    std::cin >> n;

    if (choice < 1 || choice > 5 || n <= 0) {
        std::cout << "Invalid input.\n";
        return 1;
    }

    std::cout << "\n--- Generating and logging " << n << " Emoji Sequences to emoji.log ---\n";
    logFile << "\n" << getCurrentTimestamp() << " --- Session Start: Generating " << n << " sequences with style " << choice << " ---\n";

    for (int i = 0; i < n; ++i) {
        std::string generatedSequence;
        switch (choice) {
            case 1: generatedSequence = generateRandomSequence(); break;
            case 2: generatedSequence = generateChecksumSequence(); break;
            case 3: generatedSequence = generatePairedSequence(); break;
            case 4: generatedSequence = generateMirroredSequence(); break;
            case 5: generatedSequence = generateAlternatingCategorySequence(); break;
        }
        logFile << getCurrentTimestamp() << " " << generatedSequence << std::endl;
        std::cout << generatedSequence << std::endl;
    }

    logFile.close();
    std::cout << "--- Logging complete. ---\n";
    return 0;
}