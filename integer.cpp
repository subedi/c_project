// number_generator.cpp
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
char randomDigit() {
    return '0' + rand() % 10;
}
char randomEvenDigit() {
    return '0' + (rand() % 5) * 2;
}
char randomOddDigit() {
    return '0' + (rand() % 5) * 2 + 1;
}

// --- Numeric String Generation Algorithms ---

// 1. Random Style
std::string generateRandomNumberString() {
    std::string numberString = "";
    for (int i = 0; i < 8; ++i) {
        numberString += randomDigit();
    }
    return numberString;
}

// 2. Checksum Style
std::string generateChecksumNumberString() {
    std::string numberString = "";
    int sum = 0;
    for (int i = 0; i < 7; ++i) {
        char digit = randomDigit();
        numberString += digit;
        sum += (digit - '0');
    }
    numberString += '0' + (sum % 10);
    return numberString;
}

// 3. Paired Style
std::string generatePairedNumberString() {
    std::string numberString = "";
    const int offset = 3;
    for (int i = 0; i < 4; ++i) {
        char firstDigit = randomDigit();
        char secondDigit = '0' + ((firstDigit - '0' + offset) % 10);
        numberString += firstDigit;
        numberString += secondDigit;
    }
    return numberString;
}

// 4. Mirrored Style
std::string generateMirroredNumberString() {
    std::string numberString = "";
    for (int i = 0; i < 4; ++i) {
        numberString += randomDigit();
    }
    for (int i = 3; i >= 0; --i) {
        numberString += '0' + (9 - (numberString[i] - '0'));
    }
    return numberString;
}

// 5. Alternating Parity Style
std::string generateAlternatingParityNumberString() {
    std::string numberString = "";
    for (int i = 0; i < 8; ++i) {
        if (i % 2 == 0) {
            numberString += randomEvenDigit();
        } else {
            numberString += randomOddDigit();
        }
    }
    return numberString;
}

int main() {
    srand(time(0));
    std::ofstream logFile("integer.log", std::ios_base::app);

    int choice, n;
    std::cout << "Select an algorithm style for numeric string generation:\n";
    std::cout << "1. Random Style\n";
    std::cout << "2. Checksum Style\n";
    std::cout << "3. Paired Style\n";
    std::cout << "4. Mirrored Style\n";
    std::cout << "5. Alternating Parity Style\n";
    std::cout << "Enter your choice (1-5): ";
    std::cin >> choice;

    std::cout << "How many numeric strings would you like to generate? ";
    std::cin >> n;

    if (choice < 1 || choice > 5 || n <= 0) {
        std::cout << "Invalid input.\n";
        return 1;
    }

    std::cout << "\n--- Generating and logging " << n << " Numeric Strings to integer.log ---\n";
    logFile << "\n" << getCurrentTimestamp() << " --- Session Start: Generating " << n << " numbers with style " << choice << " ---\n";

    for (int i = 0; i < n; ++i) {
        std::string generatedNumberString;
        switch (choice) {
            case 1: generatedNumberString = generateRandomNumberString(); break;
            case 2: generatedNumberString = generateChecksumNumberString(); break;
            case 3: generatedNumberString = generatePairedNumberString(); break;
            case 4: generatedNumberString = generateMirroredNumberString(); break;
            case 5: generatedNumberString = generateAlternatingParityNumberString(); break;
        }
        logFile << getCurrentTimestamp() << " " << generatedNumberString << std::endl;
        std::cout << generatedNumberString << std::endl;
    }

    logFile.close();
    std::cout << "--- Logging complete. ---\n";
    return 0;
}