#pragma once
// Include guard: ensures this header is compiled only once per translation unit.
// Prevents redeclaration of TokenBucket when multiple source files include this header.
#include <chrono>
// Provides std::chrono::steady_clock and duration types for measuring elapsed time.
// steady_clock is monotonic (never goes backward), making it safe for rate calculations.
#include <mutex>
// Provides std::mutex and std::lock_guard for thread-safe token accounting.
#include <cstddef>
// Provides size_t, used for the capacity parameter and token-count arguments.

// blank line: separates includes from the namespace block.
namespace DME {
// Opens the DME project namespace; TokenBucket is accessed externally as DME::TokenBucket.

// blank line: separates namespace opening from the class declaration.
class TokenBucket {
// Implements the token-bucket rate-limiting algorithm.
// Tokens accumulate over time up to a capacity ceiling; each request consumes one or more tokens.
public:
    // Access specifier: constructor and the three public operations callers use.
    TokenBucket(double rate, size_t capacity);
    // Constructs the bucket with the given refill rate (tokens/sec) and maximum burst capacity.
    // Starts with tokens_ == capacity so a fresh bucket allows an immediate full burst.

    // blank line: separates the constructor from the public operation methods.
    bool consume(size_t tokens = 1);
    // Attempts to consume the requested number of tokens; returns true on success, false if insufficient.
    // Default argument = 1 covers the common "one token per request" pattern without extra typing.
    void refill();
    // Manually triggers a refill calculation without consuming tokens (useful for testing).
    // Normally refill happens lazily inside consume(); this exposes it for explicit refresh scenarios.
    double available_tokens() const;
    // Returns the current token count as a double; const because it does not logically mutate state.
    // mutable mtx_ allows the internal lock to be acquired even inside this const method.

    // blank line: separates public methods from private implementation details.
private:
    // Private data members: all state needed for the token-bucket algorithm.
    double   rate_;       // tokens per second
    // Token refill speed set at construction time; governs how quickly the bucket recovers.
    size_t   capacity_;
    // Maximum tokens the bucket can hold at once; controls the maximum allowed burst size.
    double   tokens_;
    // Current token count; starts at capacity_, decremented by consume(), refilled over time.
    std::chrono::steady_clock::time_point last_refill_;
    // Timestamp of the last refill calculation; compared against now() to compute elapsed seconds.
    // steady_clock::time_point is monotonic — immune to system clock adjustments or NTP jumps.
    mutable std::mutex mtx_;
    // Protects all mutable state (tokens_, last_refill_) from concurrent access by multiple threads.
    // mutable allows the mutex to be locked inside const methods like available_tokens().

    // blank line: separates data members from the private helper method declaration.
    void refill_internal();
    // Internal helper: computes elapsed time since last_refill_ and adds proportional tokens.
    // Must only be called while mtx_ is already held — it is NOT independently thread-safe.
};
// Closes the TokenBucket class definition.

// blank line: separates the class definition from the closing namespace line.
} // namespace DME
// Closes the DME namespace; the trailing comment names it for readability in large files.
