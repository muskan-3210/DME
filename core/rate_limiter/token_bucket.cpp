#include "token_bucket.h"
// Includes the TokenBucket class declaration from the same directory.
// The header already pulls in <chrono>, <mutex>, and <cstddef> — no additional includes needed here.

// blank line: separates the include from the namespace block.
namespace DME {
// Re-enters the DME namespace so these definitions are associated with DME::TokenBucket.

// blank line: separates the namespace opening from the constructor definition.
// Purpose: initializes the bucket to full capacity and records the starting timestamp.
TokenBucket::TokenBucket(double rate, size_t capacity)
// Constructor signature: takes the refill rate (tokens/sec) and the maximum burst capacity.
    : rate_(rate), capacity_(capacity), tokens_(static_cast<double>(capacity)),
// Member initializer list (part 1): sets rate_, capacity_, and pre-fills tokens_ to full capacity.
// static_cast<double>(capacity) converts the size_t argument to double for fractional arithmetic.
      last_refill_(std::chrono::steady_clock::now()) {}
// Captures the construction time as the baseline for the first elapsed-time calculation.
// Empty body {} — all initialization is complete in the initializer list above.

// blank line: separates the constructor from consume().
// Purpose: thread-safe token consumption — deducts tokens if available, rejects the request otherwise.
bool TokenBucket::consume(size_t tokens) {
// Core rate-limiting check: grants or denies each incoming request based on available tokens.
    std::lock_guard<std::mutex> lock(mtx_);
    // RAII mutex lock: acquired here and released automatically when lock goes out of scope.
    // Serializes all concurrent consume() calls to prevent double-spending of the same tokens.
    refill_internal();
    // Adds any tokens earned since the last call before checking availability.
    // Must be called while holding the lock — refill_internal() is not independently thread-safe.
    if (tokens_ < static_cast<double>(tokens)) return false;
    // Guard: if the bucket holds fewer tokens than requested, reject the call immediately.
    // static_cast converts the size_t argument to double for a consistent type comparison.
    tokens_ -= static_cast<double>(tokens);
    // Deducts the requested number of tokens from the running balance.
    // static_cast again to match the double type of tokens_.
    return true;
    // Signals to the caller that the request was granted and the tokens have been deducted.
}
// Closes consume(); the lock_guard releases mtx_ automatically here.

// blank line: separates consume() from refill().
// Purpose: public method to trigger a refill without consuming any tokens.
void TokenBucket::refill() {
// Exposes the refill calculation publicly; useful for testing or explicit forced-refresh scenarios.
    std::lock_guard<std::mutex> lock(mtx_);
    // Acquires the mutex to safely update tokens_ and last_refill_ without data races.
    refill_internal();
    // Delegates all refill logic to the shared internal helper while the lock is held.
}
// Closes refill(); the lock_guard releases mtx_ automatically here.

// blank line: separates refill() from available_tokens().
// Purpose: read-only snapshot of the current token count for monitoring or diagnostic use.
double TokenBucket::available_tokens() const {
// Declared const in the header; reads tokens_ without consuming or refilling the bucket.
    std::lock_guard<std::mutex> lock(mtx_);
    // mtx_ is mutable, so it can be locked inside a const method without violating const-correctness.
    // Prevents a torn read of tokens_ if another thread is concurrently running consume().
    return tokens_;
    // Returns the current fractional token count; represents the balance at the last refill moment.
}
// Closes available_tokens(); the lock_guard releases mtx_ automatically here.

// blank line: separates available_tokens() from refill_internal().
// Purpose: internal helper that adds tokens proportional to elapsed time since the last refill.
void TokenBucket::refill_internal() {
// Called by consume() and refill() while mtx_ is already held — must NOT re-acquire the mutex.
    auto now     = std::chrono::steady_clock::now();
    // Captures the current monotonic clock time; auto deduces std::chrono::steady_clock::time_point.
    double secs  = std::chrono::duration<double>(now - last_refill_).count();
    // Computes the elapsed seconds since the last refill as a double (e.g., 0.1 = 100 ms).
    // duration<double> converts the raw clock tick difference to fractional wall-clock seconds.
    tokens_      = std::min(static_cast<double>(capacity_), tokens_ + secs * rate_);
    // Adds earned tokens (elapsed_seconds * rate) to the balance but caps the total at capacity_.
    // std::min prevents the bucket from exceeding its maximum burst size no matter how much time passed.
    last_refill_ = now;
    // Updates the baseline timestamp so the next call measures elapsed time from this moment forward.
}
// Closes refill_internal().

// blank line: separates refill_internal() from the closing namespace brace.
} // namespace DME
// Closes the DME namespace block; the trailing comment identifies the namespace for readability.
