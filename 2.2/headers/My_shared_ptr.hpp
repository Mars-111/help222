#pragma once
#include <map>

template <typename T>
class My_shared_ptr
{
    T* pointer = nullptr;
    static std::map<T*, int> countPointer;
public:
    My_shared_ptr() noexcept;
    My_shared_ptr(T* pointer) noexcept;
    My_shared_ptr(const My_shared_ptr& other) noexcept;
    My_shared_ptr(My_shared_ptr&& other) noexcept;

    ~My_shared_ptr();
    
    My_shared_ptr<T>& operator=(const My_shared_ptr& other) noexcept;
    My_shared_ptr<T>& operator=(My_shared_ptr&& other) noexcept;
    T& operator*() const noexcept; //assert!!!!!!!!!!
    T* operator->() const noexcept; //assert!!!!!!!!!!

    inline T* get() const noexcept;
    inline void reset() noexcept;
    inline bool unique() const noexcept;
    unsigned int use_count() const noexcept;
    inline bool empty() const noexcept;

private:
    inline void add_map(T* pointer);
    inline void remove_map(T *pointer);
};

template<typename T, typename... Args>
My_shared_ptr<T> make_shared(Args&&... args); //непонятно почему принимает и обычные переменные хотя это тип для rvalue, тяжело понять "Идеальную передачу" и форвард

















#include <cassert>

template <typename T>
inline void My_shared_ptr<T>::add_map(T *pointer)
{
    if (pointer == nullptr) return;
    if (countPointer.find(pointer) == countPointer.end()) countPointer.insert({pointer, 1});
    else countPointer.find(pointer)->second++;
}

template <typename T>
inline void My_shared_ptr<T>::remove_map(T *pointer)
{
    if (countPointer.find(pointer) == countPointer.end())
    {
        if (countPointer.find(pointer)->second > 1) countPointer.find(pointer)->second--;
        else countPointer.erase(countPointer.find(pointer));
    }
}

template <typename T>
inline My_shared_ptr<T>::My_shared_ptr() noexcept
{
    pointer = nullptr;
}

template <typename T>
My_shared_ptr<T>::My_shared_ptr(T *pointer) noexcept
{
    this->pointer = pointer;
    add_map(pointer);
}

template <typename T>
My_shared_ptr<T>::My_shared_ptr(const My_shared_ptr &other) noexcept = default;

template <typename T>
My_shared_ptr<T>::My_shared_ptr(My_shared_ptr &&other) noexcept
{
    pointer = other.pointer;
    add_map(pointer);
}

template <typename T>
My_shared_ptr<T>::~My_shared_ptr()
{
    remove_map(pointer);
    delete pointer;
}

template <typename T>
My_shared_ptr<T> &My_shared_ptr<T>::operator=(const My_shared_ptr &other) noexcept
{
    remove_map(pointer);
    delete pointer;
    pointer = other.pointer;
    add_map(pointer);
    return *this;
}

template <typename T>
My_shared_ptr<T> &My_shared_ptr<T>::operator=(My_shared_ptr &&other) noexcept
{
    remove_map(pointer);
    delete pointer;
    pointer = other.pointer;
    add_map(pointer);
}

template <typename T>
T &My_shared_ptr<T>::operator*() const noexcept
{
    assert(pointer != nullptr);
    return *pointer;
}

template <typename T>
T *My_shared_ptr<T>::operator->() const noexcept
{
    assert(pointer != nullptr);
    return pointer;
}

template <typename T>
inline T *My_shared_ptr<T>::get() const noexcept
{
    return pointer;
}

template <typename T>
inline void My_shared_ptr<T>::reset() noexcept
{
    remove_map(pointer);
    delete pointer;
}

template <typename T>
inline bool My_shared_ptr<T>::unique() const noexcept
{
    return countPointer.find(pointer)->second == 1;
}

template <typename T>
unsigned int My_shared_ptr<T>::use_count() const noexcept
{
    return countPointer.find(pointer)->second;
}

template <typename T>
inline bool My_shared_ptr<T>::empty() const noexcept
{
    return pointer == nullptr;
}




template <typename T, typename... Args>
inline My_shared_ptr<T> make_shared(Args &&...args)
{
    T* t = new T(std::forward<Args>(args)...);
    return My_shared_ptr<T>(t);
}