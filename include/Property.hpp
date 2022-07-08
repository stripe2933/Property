#pragma once

#include <functional>

#include "PropertyTransaction.hpp"

template <typename T>
class PropertyTransaction;

template <typename T>
class Property{
public:
    //////////////////////////////////////////////////
    /// Typedefs
    //////////////////////////////////////////////////

    using Transaction = PropertyTransaction<T>;
    using CallbackType = std::function<void(const T&)>;

    //////////////////////////////////////////////////
    /// Constructors
    //////////////////////////////////////////////////

    template <typename... TPP, typename CallbackPP> requires std::is_convertible_v<CallbackPP, CallbackType>
    Property(CallbackPP &&callback, TPP&&... value) : callback { std::forward<CallbackPP>(callback) }, data { std::forward<TPP>(value)... } { 
        callback(data);
    }

    template <typename... TPP>
    Property(TPP&&... value) : data { std::forward<TPP>(value)... } { }

    //////////////////////////////////////////////////
    /// Getter and Setters
    //////////////////////////////////////////////////

    const T &get() const noexcept{
        return data;
    }

    template <typename TPP>
    void set(TPP &&value) noexcept { 
        data = std::forward<TPP>(value); 
        if (callback){
            callback(data);
        }
    }

    Transaction getTransaction() noexcept {
        return Transaction { *this };
    }

    template <typename TPP>
    Property<T> &operator=(TPP &&value) noexcept { set(std::forward<T>(value)); return *this; }

private:
    CallbackType callback;
    T data;

    friend PropertyTransaction<T>;
};