#include <initializer_list>
#include <list>
#include <stdexcept>
#include <vector>
#include <iostream>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>> 
class HashMap {
public:
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType>>::const_iterator;
    using iterator = typename std::list<std::pair<const KeyType, ValueType>>::iterator;
private:
    const static size_t CAPACITY = 5e5;
    size_t sz = 0;
    Hash hash;
    std::list<std::pair<const KeyType, ValueType>> data;
    std::vector<std::vector<iterator>> table = std::vector<std::vector<iterator>>(CAPACITY);
public:
    HashMap(Hash hash = Hash()): hash(hash) {}

    template<typename It>
    HashMap(It begin,
            const It end,
            Hash hash = Hash()): hash(hash) {
        while (begin != end) {
            insert(*begin);
            ++begin;
        }
    }

    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> list,
            Hash hash = Hash()): HashMap(list.begin(), list.end(), hash) {}

    HashMap<KeyType, ValueType, Hash>& operator=(const HashMap<KeyType, ValueType, Hash>& other) {
        hash = other.hash;
        auto to_copy = other.data;

        clear();
        for (auto element : to_copy) {
            insert(element);
        }
        return *this;
    }

    uint32_t size() const {
        return sz;
    }

    bool empty() const {
        return sz == 0;
    }

    Hash hash_function() const {
        return hash;
    }

    bool has_key(const KeyType& key) const {
        size_t bucket = hash(key) % CAPACITY;
        for (auto it : table[bucket]) {
            if (it->first == key) return true;
        }
        return false;
    }

    void insert(const std::pair<KeyType, ValueType>& element) {
        if (!has_key(element.first)) {
            ++sz;
            size_t bucket = hash(element.first) % CAPACITY;

            data.push_front(element);
            table[bucket].push_back(data.begin());
        }
    }

    void erase(const KeyType& key) {
        size_t bucket = hash(key) % CAPACITY;

        for (size_t i = 0; i < table[bucket].size(); ++i) {
            auto it = table[bucket][i];

            if (it->first == key) {
                --sz;

                data.erase(it);
                table[bucket].erase(table[bucket].begin() + i);
                break;
            }
        }
    }

    const_iterator begin() const {
        return data.cbegin();
    }

    const_iterator end() const {
        return data.cend();
    }

    iterator begin() {
        return data.begin();
    }

    iterator end() {
        return data.end();
    }

    const_iterator find(const KeyType& key) const {
        size_t bucket = hash(key) % CAPACITY;

        for (auto it : table[bucket]) {
            if (it->first == key) {
                return it;
            }
        }

        return data.end();
    }

    iterator find(const KeyType& key) {
        size_t bucket = hash(key) % CAPACITY;

        for (auto it : table[bucket]) {
            if (it->first == key) {
                return it;
            }
        }

        return data.end();
    }

    ValueType& operator[](const KeyType& key) {
        if (!has_key(key)) {
            insert({ key, ValueType() });
        }

        return find(key)->second;
    }

    const ValueType& at(const KeyType& key) const {
        if (!has_key(key)) {
            throw std::out_of_range("");
        }

        return find(key)->second;
    }

    void clear() {
        auto to_erase = data;
        for (auto node : to_erase) {
            erase(node.first);
        }
    }
};
