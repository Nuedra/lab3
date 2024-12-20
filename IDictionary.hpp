#ifndef LAB3_IDICTIONARY_HPP
#define LAB3_IDICTIONARY_HPP

template<typename TKey, typename TValue>
class IDictionary {
public:
    virtual ~IDictionary() = default;

    virtual void add(const TKey& key, const TValue& value) = 0;
    virtual TValue get(const TKey& key) const = 0;
    virtual void remove(const TKey& key) = 0;
    virtual bool contains_key(const TKey& key) const = 0;
    virtual int get_count() const = 0;
    virtual int get_capacity() const = 0;
};

#endif //LAB3_IDICTIONARY_HPP
