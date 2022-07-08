#pragma once

#include <functional>

#include "PropertyTransaction.hpp"

template <typename T>
class PropertyTransaction;

template <typename T>
class Property{
private:
    std::function<void(const T&)> presetter;
    std::function<void(const T&)> postsetter;
    T data;

    friend PropertyTransaction<T>;

public:
    //////////////////////////////////////////////////
    /// Typedefs
    //////////////////////////////////////////////////

    using Transaction = PropertyTransaction<T>;

    //////////////////////////////////////////////////
    /// Constructors
    //////////////////////////////////////////////////

    Property(const std::function<void(const T&)>& presetter = {}, const std::function<void(const T&)>& postsetter = {}) : presetter { presetter }, postsetter { postsetter} { }
    template <typename TPP> requires std::is_convertible_v<TPP, T>
    explicit Property(TPP &&value, const std::function<void(const T&)>& presetter = {}, const std::function<void(const T&)>& postsetter = {}) : data { std::forward<const T>(value) }, presetter { presetter }, postsetter { postsetter } { 
        if (postsetter){
            postsetter(data);
        }
    }

    //////////////////////////////////////////////////
    /// Getter and Setters
    //////////////////////////////////////////////////

    const T &get() const noexcept{
        return data;
    }

    template <typename TPP> requires std::is_convertible_v<TPP, T>
    void set(TPP &&value) noexcept { 
        if (presetter){
            presetter(data);
        }
        data = std::forward<const T>(value); 
        if (postsetter){
            postsetter(data);
        }
    }

    template <typename TPP>
    Property<T> &operator=(TPP &&value) noexcept { set(std::forward<T>(value)); return *this; }
};