#pragma once

template <typename T>
class ReadonlyProperty{
private:
    T data;

public:
    //////////////////////////////////////////////////
    /// Constructors
    //////////////////////////////////////////////////

    ReadonlyProperty() { }
    template <typename... TPP>
    ReadonlyProperty(TPP&&... value) : data { std::forward<TPP>(value)... } { }

    //////////////////////////////////////////////////
    /// Getter
    //////////////////////////////////////////////////

    const T &get() const noexcept{
        return data;
    }
};