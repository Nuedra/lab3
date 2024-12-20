#ifndef LAB3_IITERATOR_HPP
#define LAB3_IITERATOR_HPP

template<typename TElement>
class IIterator {
public:
    virtual ~IIterator() = default;

    virtual TElement get_current_item() const = 0;
    virtual bool has_next() const = 0;
    virtual bool next() = 0;
    virtual bool try_get_current_item(TElement& element) const = 0;
};

#endif //LAB3_IITERATOR_HPP
