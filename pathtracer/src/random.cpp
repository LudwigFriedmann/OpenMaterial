//
// Copyright (C) 2019, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
//

/// @file      random.cpp
/// @author    Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
/// @copyright (C) 2019, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
/// @date      2019-08-30
/// @brief     Support for creating random numbers



#include <cmath>
#include <cstdint>
#include <random>
#include <string>
#include <ctime>
#include <thread>

#include "doctest.h"
#include "exceptions.h"
#include "random.h"

/// @brief Unit test for Random::getSeed
TEST_CASE("Testing Random::getSeed")
{
	// Checking random seed generation
	CHECK(Random::getSeed() != Random::getSeed());
}

/// @brief Unit test for Random::getRandomU64
TEST_CASE("Testing Random::getRandomU64")
{
	// Checking random number generation
	CHECK(Random::getRandomU64() != Random::getRandomU64());
}

/// Mathematical constant pi
#ifndef M_PI
#define M_PI 3.141592653589793
#endif

#ifdef __linux__
#include <cstdio>

static uint64_t _getrandom()
{
    uint64_t data = 0;

    FILE *f = fopen("/dev/urandom", "rb");
    if(f == nullptr)
        throw OSError("Cannot open /dev/urandom for reading");

    // Read data from /dev/urandom
    size_t out = fread(&data, sizeof(data), 1, f);

    fclose(f);

    // Check if reading was actually successful
    if(out != 1)
        throw OSError("Cannot read from /dev/urandom");

    return data;
}
#else
static uint64_t _getrandom() { return 0; }
#endif // __linux__


namespace Random
{
    /// @brief Return random uint64_t that can be used as a seed
    ///
    /// The seed is generated by xor'ing different sources of entropy. These
    /// sources are time, addresses to a local variable and a library function (the
    /// addresses should be random due to ASLR), the thread id, and on Linux from
    /// the kernel using the getrandom syscall. See also Ref. [1] for more information.
    ///
    /// As long as one source remains uncompromised plus statistically random on
    /// all bits, and all sources remain independent, then a xor of different
    /// entropies is fine, see also Ref. [2].
    ///
    /// This function might not be suitable for cryptographic purposes.
    ///
    /// References:
    ///   [1] http://www.pcg-random.org/posts/simple-portable-cpp-seed-entropy.html
    ///   [2] https://crypto.stackexchange.com/questions/17658/mixing-entropy-sources-by-xor
    ///
    /// @retval seed 64bit random number
    uint64_t getSeed()
    {
        uint64_t seed = 0;

        // Use clock as source of entropy
        seed ^= std::chrono::high_resolution_clock::now().time_since_epoch().count();

        // Use address of local variable as source of entropy (should work due
        // to ASLR)
        seed ^= reinterpret_cast<uint64_t>((void*)&seed);

        // Use address of a library function as source of entropy (should work
        // due to ASLR)
        seed ^= reinterpret_cast<uint64_t>((void*)&_Exit);

        // Use thread id as source of entropy
        seed ^= std::hash<std::thread::id>()(std::this_thread::get_id());

        // Use random entropy from the OS (if not available function returns 0)
        seed ^= _getrandom();

        return seed;
    }

    /// @brief Return pointer to std::mt19937 generator
    ///
    /// This function returns a pointer to a std::mt19937 generator. Per thread
    /// always the same generator is returned. For different threads, different
    /// generators are returned.
    ///
    /// @retval pGenerator pointer to std::mt19937 generator
    std::mt19937 *getMersenneGenerator()
    {
        static thread_local std::mt19937 *generator = nullptr;

        if(!generator)
            // create generator
            generator = new std::mt19937(getSeed());

        return generator;
    }

    /// @brief Return pointer to std::mt19937_64 generator
    ///
    /// This function returns a pointer to a std::mt19937_64 generator. Per
    /// thread always the same generator is returned. For different threads,
    /// different generators are returned.
    ///
    /// @retval pGenerator pointer to std::mt19937_64 generator
    std::mt19937_64 *getMersenne64Generator()
    {
        static thread_local std::mt19937_64 *generator = nullptr;

        if(!generator)
            // Create generator
            generator = new std::mt19937_64(getSeed());

        return generator;
    }

    /// Return a random uint64
    uint64_t getRandomU64()
    {
        return (*getMersenne64Generator())();
    }
}