// prime_rain_generator.cpp — v2
// ---------------------------------------------------------------
// Multithreaded “Make‑it‑rain” generator for 8‑digit primes.
// For every random 8‑digit seed, we keep nudging it with *another* random
// 8‑digit delta (add or subtract) until we land on a prime.  Every hop is
// recorded to prime_rain_log.txt, guarded by a mutex so multiple threads
// can party in the same file without stepping on each other’s confetti.
// ---------------------------------------------------------------
// Build:   g++ -std=c++17 -O2 -pthread prime_rain_generator.cpp -o prime_rain
// Run:     ./prime_rain [count] [threads]
//   count   = how many seeds in total (default 100)
//   threads = #worker threads        (default hw_concurrency)
// ---------------------------------------------------------------
// Finale: a glorious ANSI‑color confetti shower plus stats on how many
// 8‑digit raindrops each seed needed on average. Enjoy! 🌈💧🔢
// ---------------------------------------------------------------

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <mutex>
#include <thread>
#include <atomic>

// ─────────────────── primality (deterministic < 2^32) ───────────────────────
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

// ───────────────────────── global stuff ─────────────────────────────────────
constexpr uint32_t LOWER = 10000000;      // 8‑digit range
constexpr uint32_t UPPER = 99999999;

std::mutex logMutex;
std::ofstream logFile("prime_rain_log.txt");

std::atomic<size_t> totalHops{0};

// ────────────────── worker that handles a slice of seeds ────────────────────
void rainWorker(size_t startIdx, size_t endIdx, std::vector<uint32_t>& primes) {
    // thread‑local RNG
    std::mt19937 rng(static_cast<uint32_t>(
        std::chrono::steady_clock::now().time_since_epoch().count() + startIdx));
    std::uniform_int_distribution<uint32_t> dist8(LOWER, UPPER);
    std::bernoulli_distribution flip(0.5);

    for (size_t i = startIdx; i < endIdx; ++i) {
        uint32_t n = dist8(rng);
        uint32_t original = n;
        size_t hops = 0;

        {
            std::lock_guard<std::mutex> lk(logMutex);
            logFile << "Seed #" << i + 1 << " (thread " << std::this_thread::get_id() << ") : " << original << "\n";
        }

        while (!isPrime(n)) {
            uint32_t delta = dist8(rng);              // 8‑digit hop
            int64_t signedN = static_cast<int64_t>(n);
            int64_t signedDelta = static_cast<int64_t>(delta);
            signedN += flip(rng) ? signedDelta : -signedDelta;

            // wrap back into 8‑digit range
            signedN = (signedN - LOWER) % (UPPER - LOWER + 1);
            if (signedN < 0) signedN += (UPPER - LOWER + 1);
            n = static_cast<uint32_t>(signedN + LOWER);

            ++hops;
            {
                std::lock_guard<std::mutex> lk(logMutex);
                logFile << "  hop " << std::setw(4) << hops << ": ±" << delta << " -> " << n << "\n";
            }
        }

        {
            std::lock_guard<std::mutex> lk(logMutex);
            logFile << "  prime reached after " << hops << " hops: " << n << "\n\n";
        }

        totalHops += hops;
        primes[i] = n;
    }
}

// ──────────────────────────── main ──────────────────────────────────────────
int main(int argc, char* argv[]) {
    if (!logFile) {
        std::cerr << "Cannot open log file!\n";
        return 1;
    }

    size_t count   = (argc > 1) ? std::stoul(argv[1]) : 100;
    size_t threads = (argc > 2) ? std::stoul(argv[2]) : std::thread::hardware_concurrency();
    threads = std::max<size_t>(1, threads);

    logFile << "Prime‑Rain log — " << count << " seeds with " << threads << " threads\n\n";

    std::vector<uint32_t> primes(count);
    std::vector<std::thread> pool;
    size_t chunk = (count + threads - 1) / threads;

    for (size_t t = 0; t < threads; ++t) {
        size_t start = t * chunk;
        size_t end   = std::min(count, start + chunk);
        if (start >= end) break;
        pool.emplace_back(rainWorker, start, end, std::ref(primes));
    }

    for (auto& th : pool) th.join();

    double avgHops = static_cast<double>(totalHops) / count;

    // ───────────── Funky ANSI‑color confetti finale ──────────────
    const char* colors[] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};
    constexpr size_t C = sizeof(colors)/sizeof(colors[0]);

    std::cout << "\n\033[1m\033[7m=>  IT'S  RAINING  8‑DIGIT  PRIMES!  <=\033[0m\n\n";
    for (size_t i = 0; i < primes.size(); ++i) {
        std::cout << colors[i % C] << primes[i] << " \033[0m";
        if ((i + 1) % 5 == 0) std::cout << "\n";
    }
    std::cout << "\n\nAverage hops per seed: " << std::fixed << std::setprecision(2) << avgHops << "\n";

    std::cout << (avgHops < 3 ? "Lucky cloud! 🌧️" : "Primes played hard‑to‑get today. ⚡") << "\n";
    std::cout << "(Full journey logged to prime_rain_log.txt)\n";

    return 0;
}
