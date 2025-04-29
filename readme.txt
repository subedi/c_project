# Build (GCC/Clang)
 g++ -std=c++17 -O2 -pthread prime_rain_generator.cpp -o prime_rain

# Run 1 000 seeds across all logical cores
 ./prime_rain 1000

# Run 250 seeds on exactly 8 threads
 ./prime_rain 250 8