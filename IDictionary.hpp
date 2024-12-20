#ifndef LAB3_IDICTIONARY_HPP
#define LAB3_IDICTIONARY_HPP

template<typename TKey, typename TValue>
class IDictionary {
public:
    virtual ~IDictionary() = default;

    virtual void Add(const TKey& key, const TValue& value) = 0;
    virtual TValue Get(const TKey& key) const = 0;
    virtual void Remove(const TKey& key) = 0;
    virtual bool ContainsKey(const TKey& key) const = 0;
    virtual int GetCount() const = 0;
    virtual int GetCapacity() const = 0;
};

#endif //LAB3_IDICTIONARY_HPP
