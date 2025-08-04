// string_generator.cpp
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

// --- Generation Helpers ---
char randomChar() {
    return 'A' + rand() % 26;
}
char randomVowel() {
    const std::string vowels = "AEIOU";
    return vowels[rand() % vowels.length()];
}
char randomConsonant() {
    char c;
    do {
        c = randomChar();
    } while (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
    return c;
}

// --- String Generation Algorithms ---

// 1. Random Style
std::string generateRandomString() {
    std::string resultString = "";
    for (int i = 0; i < 8; ++i) {
        resultString += randomChar();
    }
    return resultString;
}

// 2. Checksum Style
std::string generateChecksumString() {
    std::string resultString = "";
    int sum = 0;
    for (int i = 0; i < 7; ++i) {
        char c = randomChar();
        resultString += c;
        sum += c;
    }
    resultString += 'A' + (sum % 26);
    return resultString;
}

// 3. Paired Style
std::string generatePairedString() {
    std::string resultString = "";
    const int offset = 5;
    for (int i = 0; i < 4; ++i) {
        char firstChar = randomChar();
        char secondChar = 'A' + ((firstChar - 'A' + offset) % 26);
        resultString += firstChar;
        resultString += secondChar;
    }
    return resultString;
}

// 4. Mirrored Style
std::string generateMirroredString() {
    std::string resultString = "";
    for (int i = 0; i < 4; ++i) {
        resultString += randomChar();
    }
    for (int i = 3; i >= 0; --i) {
        resultString += 'A' + (25 - (resultString[i] - 'A'));
    }
    return resultString;
}

// 5. Alternating Phonetic Style
std::string generateAlternatingPhoneticString() {
    std::string resultString = "";
    for (int i = 0; i < 8; ++i) {
        if (i % 2 == 0) {
            resultString += randomConsonant();
        } else {
            resultString += randomVowel();
        }
    }
    return resultString;
}

int main() {
    srand(time(0));
    std::ofstream logFile("string.log", std::ios_base::app);

    int choice, n;
    std::cout << "Select an algorithm style for string generation:\n";
    std::cout << "1. Random Style\n";
    std::cout << "2. Checksum Style\n";
    std::cout << "3. Paired Style\n";
    std::cout << "4. Mirrored Style\n";
    std::cout << "5. Alternating Phonetic Style\n";
    std::cout << "Enter your choice (1-5): ";
    std::cin >> choice;

    std::cout << "How many strings would you like to generate? ";
    std::cin >> n;

    if (choice < 1 || choice > 5 || n <= 0) {
        std::cout << "Invalid input.\n";
        return 1;
    }

    std::cout << "\n--- Generating and logging " << n << " Strings to string.log ---\n";
    logFile << "\n" << getCurrentTimestamp() << " --- Session Start: Generating " << n << " strings with style " << choice << " ---\n";

    for (int i = 0; i < n; ++i) {
        std::string generatedString;
        switch (choice) {
            case 1: generatedString = generateRandomString(); break;
            case 2: generatedString = generateChecksumString(); break;
            case 3: generatedString = generatePairedString(); break;
            case 4: generatedString = generateMirroredString(); break;
            case 5: generatedString = generateAlternatingPhoneticString(); break;
        }
        logFile << getCurrentTimestamp() << " " << generatedString << std::endl;
        std::cout << generatedString << std::endl;
    }
    
    logFile.close();
    std::cout << "--- Logging complete. ---\n";
    return 0;
}