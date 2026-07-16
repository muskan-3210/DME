#pragma once
// Compiler directive: ensures this header is processed only once per translation unit,
// preventing duplicate class and template definition errors.

#include <unordered_map>
// Provides std::unordered_map for O(1) average-case key lookup.
// Used twice here: key_map_ (key → list iterator) and freq_map_ (frequency → entry list).

#include <list>
// Provides std::list — a doubly-linked list allowing O(1) insertion and removal at any position.
// Each frequency bucket in freq_map_ is a std::list<Entry> ordered by recency within that frequency.

#include <cstddef>
// Provides the portable unsigned type size_t for capacity and size fields.

namespace DME {
// Opens the DSA-Master-Engine project namespace, shared with lru_cache.h and all other DME components.

template<typename K, typename V>
// Declares this as a class template with key type K and value type V.
// Any hashable K and any copyable V can be used; the compiler generates a concrete class per instantiation.

class LFUCache {
// Implements the Least Frequently Used cache eviction policy.
// When full, the entry with the lowest access count is evicted.
// Ties in frequency are broken by recency: the oldest (least recently promoted) entry is removed first.

public:
// Public access specifier: the declarations below form the external API of LFUCache.

    explicit LFUCache(size_t capacity);
// Constructor declaration. 'explicit' prevents accidental implicit conversions from size_t to LFUCache.

    bool  get(const K& key, V& out_value);
// Retrieve the value for 'key'; increment its frequency counter on a hit. Returns false on a miss.
// 'out_value' is an output parameter — the caller passes a reference that receives the found value.

    void  put(const K& key, const V& value);
// Insert a new entry or update an existing one. Evicts the LFU entry first if at capacity.

    bool  contains(const K& key) const;
// Non-mutating membership check: does not alter frequency counters.

    size_t size()     const;
// Returns the current number of entries stored in the cache.

    size_t capacity() const;
// Returns the maximum capacity set at construction time.

private:
// Private section: implementation details hidden from callers.

    struct Entry {
    // Inner struct bundling all per-slot data together so freq_map_ lists can store complete entries.
    // This avoids storing separate parallel containers for keys, values, and frequencies.

        K key;
        // The cache key stored inside the entry so eviction can remove it from key_map_ without
        // the caller needing to provide the key again.

        V value;
        // The cached payload — what callers retrieve via get().

        int freq;
        // Access count: how many times get() or put() has touched this entry.
        // Determines which frequency bucket (freq_map_[freq]) this entry belongs to.

    };
    // Closes the Entry struct body.

    size_t capacity_;
    // Maximum number of entries before eviction is triggered. Set once at construction and never changed.

    size_t size_;
    // Current number of live entries. Tracked manually because freq_map_ does not expose a total count.

    int    min_freq_;
    // The lowest frequency value present among all current entries.
    // evict() uses this to jump directly to freq_map_[min_freq_] in O(1) without scanning all buckets.

    std::unordered_map<K, typename std::list<Entry>::iterator> key_map_;
    // Maps each key to the iterator of its Entry node inside the appropriate freq_map_ list.
    // 'typename' is required because std::list<Entry>::iterator is a dependent type (depends on Entry which
    // depends on template params K and V). Enables O(1) lookup by key without scanning freq_map_.

    std::unordered_map<int, std::list<Entry>>                  freq_map_;
    // Maps each frequency value to a doubly-linked list of all entries at that frequency.
    // Within each list, front = most recently promoted, back = least recently touched (eviction target for ties).
    // Together with key_map_ and min_freq_ this achieves O(1) get, put, and evict operations.

    void increment_freq(typename std::list<Entry>::iterator it);
    // Private helper: moves entry 'it' from its current frequency bucket to the (freq+1) bucket.
    // Called by both get() and put() after a successful access to maintain accurate frequency counts.

    void evict();
    // Private helper: removes the LFU entry (back of freq_map_[min_freq_]) to make room for a new one.

};
// Closes the LFUCache class template body.

template<typename K, typename V>
// Template parameter list required before every out-of-line template method definition.

LFUCache<K,V>::LFUCache(size_t capacity)
// Constructor definition header; the initializer list follows on the next line.

    : capacity_(capacity), size_(0), min_freq_(0) {}
// Initializes capacity_ from the argument; size_ and min_freq_ start at 0 (empty cache).
// Empty body '{}' because std::unordered_map members default-construct themselves.

template<typename K, typename V>
// Template parameter list for the get() method.

bool LFUCache<K,V>::get(const K& key, V& out_value) {
// Purpose: look up key, write value to out_value, increment frequency, return true on a cache hit.

    auto it = key_map_.find(key);
// Search the key map for 'key'. Returns key_map_.end() if the key is absent.

    if (it == key_map_.end()) return false;
// Cache miss: key not found. Return false immediately; out_value is left unchanged.

    out_value = it->second->value;
// it->second is the list iterator stored in key_map_; ->value is the Entry's payload field.
// Writes the found value into the caller's out_value reference.

    increment_freq(it->second);
// Promote this entry to the next frequency bucket because it was just accessed.
// increment_freq also updates min_freq_ if the old bucket becomes empty.

    return true;
// Reports a successful cache hit.

}
// Closes get() method body.

template<typename K, typename V>
// Template parameter list for the put() method.

void LFUCache<K,V>::put(const K& key, const V& value) {
// Purpose: insert a new entry or update an existing one; evict LFU entry first if at capacity.

    if (capacity_ == 0) return;
// Guard: a zero-capacity cache can never store anything; exit immediately to avoid undefined behavior.

    auto it = key_map_.find(key);
// Check whether 'key' already exists in the cache before deciding to insert or update.

    if (it != key_map_.end()) {
// Branch taken when the key is already present — update value and refresh frequency.

        it->second->value = value;
// Overwrite the value field of the existing Entry node directly via its stored list iterator.

        increment_freq(it->second);
// Count this put() as an access — increment the frequency of the existing entry.

        return;
// Early return: update complete, no eviction needed.

    }
// Closes the "key already exists" branch.

    if (size_ == capacity_) evict();
// If the cache is at maximum capacity, remove the LFU entry before inserting the new one.

    freq_map_[1].push_front({key, value, 1});
// Insert a new Entry into the frequency-1 bucket at the front (most recent among freq-1 entries).
// Brace initialization creates an Entry aggregate with freq initialized to 1.

    key_map_[key] = freq_map_[1].begin();
// Store the iterator to the newly inserted node so future get/put calls can locate it in O(1).

    min_freq_ = 1;
// A new entry always starts with frequency 1, which becomes the new global minimum.

    ++size_;
// Increment the live entry counter to reflect the successful insertion.

}
// Closes put() method body.

template<typename K, typename V>
// Template parameter list for the contains() method.

bool LFUCache<K,V>::contains(const K& key) const {
// Purpose: non-mutating membership test — does not alter any frequency counts or LFU order.

    return key_map_.find(key) != key_map_.end();
// Returns true if 'key' has an entry in key_map_ (and therefore is currently in the cache).

}
// Closes contains() method body.

template<typename K, typename V>
// Template parameter list for the size() method.

size_t LFUCache<K,V>::size() const { return size_; }
// Returns the manually tracked size_ counter rather than computing it from freq_map_.
// This is O(1) and avoids summing the sizes of all frequency bucket lists.

template<typename K, typename V>
// Template parameter list for the capacity() method.

size_t LFUCache<K,V>::capacity() const { return capacity_; }
// Returns the fixed maximum capacity stored at construction. No computation required.

template<typename K, typename V>
// Template parameter list for the increment_freq() helper method.

void LFUCache<K,V>::increment_freq(typename std::list<Entry>::iterator it) {
// Purpose: move entry 'it' from frequency bucket [old_freq] to [old_freq+1].
// Maintains min_freq_ so evict() always targets the correct bucket in O(1).

    int old_freq = it->freq;
// Save the entry's current frequency before modifying or erasing anything.

    Entry entry  = *it;
// Copy the Entry struct by value. Once we erase the node, the iterator 'it' is invalidated,
// but 'entry' preserves the key, value, and (about-to-be-incremented) freq data safely.

    entry.freq++;
// Increment the frequency counter on the local copy before re-inserting it.

    freq_map_[old_freq].erase(it);
// Remove the node from its current frequency bucket. This invalidates iterator 'it'.
// The entry data is safe in the local copy 'entry' made before the erase.

    if (freq_map_[old_freq].empty()) {
// If the old frequency bucket has no entries left, clean it up to avoid stale map entries.

        freq_map_.erase(old_freq);
// Remove the now-empty bucket entirely from freq_map_ to free memory.

        if (min_freq_ == old_freq) min_freq_++;
// If the empty bucket was the global minimum-frequency bucket, the new minimum is old_freq+1.
// This is safe because 'entry' is about to be inserted into old_freq+1, keeping it non-empty.

    }
// Closes the "old frequency bucket is now empty" branch.

    freq_map_[entry.freq].push_front(entry);
// Insert the entry at the front of the new (higher) frequency bucket.
// Front = most recently promoted, so within a bucket recency-based tie-breaking is preserved.

    key_map_[entry.key] = freq_map_[entry.freq].begin();
// Update key_map_ to point to the newly inserted node in the new bucket.
// The old iterator was invalidated by erase(); begin() gives the fresh valid iterator.

}
// Closes increment_freq() method body.

template<typename K, typename V>
// Template parameter list for the evict() helper method.

void LFUCache<K,V>::evict() {
// Purpose: remove the entry with the lowest access frequency (oldest among ties) to make room.

    auto& lst = freq_map_[min_freq_];
// Obtain a reference to the list at the minimum-frequency bucket.
// A reference avoids copying the list and allows direct in-place modification.

    key_map_.erase(lst.back().key);
// Remove the victim's key from key_map_. lst.back() is the oldest entry at the minimum frequency.
// Must erase from key_map_ first; after pop_back the Entry is destroyed and .key would be dangling.

    lst.pop_back();
// Remove the LFU victim node from the tail of the min-frequency bucket list.

    if (lst.empty()) freq_map_.erase(min_freq_);
// If the min-frequency bucket is now empty, remove it from freq_map_ to keep memory clean.
// min_freq_ will be reset to 1 by the next put() call (new entries always start at frequency 1).

    --size_;
// Decrement the live entry counter to reflect the eviction.

}
// Closes evict() method body.

} // namespace DME
// Closes the DME namespace block opened at the top of this file.
