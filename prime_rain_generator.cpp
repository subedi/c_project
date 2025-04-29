// prime_rain_generator.cpp
// "Make it rain" 8‑digit primes — because boring number generators need more confetti.
// ---------------------------------------------------------------
// This little program whips up a storm of random 8‑digit numbers,
// checks if each is prime, and—if it isn’t—keeps randomly adding
// or subtracting offsets until it *does* hit a prime. Every hop is
// splashed into a log file (prime_rain_log.txt) so you can replay
// the whole tempest later.
// ---------------------------------------------------------------
// Build:   g++ -std=c++17 -O2 prime_rain_generator.cpp -o prime_rain
// Run:     ./prime_rain [count]
//   where  count = how many initial 8‑digit numbers you want (default 20)
// ---------------------------------------------------------------
// Fun fact printed at the end: how many raindrops (adjustments) on
// average it took before finding a prime.
// ---------------------------------------------------------------

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <cmath>
#include <iomanip>

// ---------- quick & dirty primality check (deterministic for < 10^9) ---------
bool isPrime(uint32_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint32_t r = static_cast<uint32_t>(std::sqrt(n));
    for (uint32_t f = 5; f <= r; f += 6) {
        if (n % f == 0 || n % (f + 2) == 0) return false;
    }
    return true;
}

// ------------------------- main drizzle --------------------------------------
int main(int argc, char* argv[]) {
    const uint32_t LOWER = 10000000;     // smallest 8‑digit number
    const uint32_t UPPER = 99999999;     // largest  8‑digit number
    size_t count = (argc > 1) ? std::stoul(argv[1]) : 20;

    // rng setup
    std::mt19937 rng(static_cast<uint32_t>(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<uint32_t> dist8(LOWER, UPPER);
    std::uniform_int_distribution<int32_t> distStep(1, 1000);  // adjustment magnitude
    std::bernoulli_distribution flip(0.5);                      // add or subtract

    std::ofstream log("prime_rain_log.txt");
    if (!log) {
        std::cerr << "Cannot open log file!\n";
        return 1;
    }
    log << "Prime‑Rain log — " << count << " seeds\n\n";

    size_t totalHops = 0;
    std::vector<uint32_t> primes;
    primes.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        uint32_t n = dist8(rng);
        uint32_t original = n;
        size_t hops = 0;
        log << "Seed #" << i + 1 << ": " << original << "\n";

        while (!isPrime(n)) {
            int32_t delta = distStep(rng);
            if (flip(rng)) n += delta; else n -= delta;
            // wrap to stay 8 digits
            if (n < LOWER) n += LOWER;
            if (n > UPPER) n %= UPPER; // simple wrap, still 8 digits
            ++hops;
            log << "  hop " << std::setw(3) << hops << ": " << (flip(rng) ? "+" : "-") << delta << " -> " << n << "\n";
        }

        log << "  prime reached after " << hops << " hops: " << n << "\n\n";
        totalHops += hops;
        primes.push_back(n);
    }

    double avgHops = static_cast<double>(totalHops) / count;

    // Pretty finale
    std::cout << "\n*********************************************\n";
    std::cout << "*           IT'S  RAINING  PRIMES!         *\n";
    std::cout << "*********************************************\n\n";

    for (auto p : primes) std::cout << p << " ";
    std::cout << "\n\nAverage hops per seed: " << std::fixed << std::setprecision(2) << avgHops << "\n";

    // Fun trivia: How often did we need to nudge numbers?
    std::cout << "Fun tidbit: " << ((avgHops < 3) ? "You have a lucky storm cloud!" : "Those primes played hard‑to‑get today.") << "\n";

    std::cout << "(Detailed journey saved to prime_rain_log.txt)\n";
    return 0;
}
