#pragma once

#include "Property.hpp"

template <typename T, bool CallbackAtConstruction>
class Property; // Forward declaration for PropertyTransaction.

template <typename T, bool CallbackAtConstruction>
class PropertyTransaction{
public:
    explicit PropertyTransaction(Property<T, CallbackAtConstruction> &property) : property { property }, data { property.data } { }
    ~PropertyTransaction() { 
        if (property.callback){
            property.callback(data);
        }
    }

    T &data; // Access this member variable to modify the property data.

    // This copy/move construcrors and assign operator are deleted.
    PropertyTransaction(const PropertyTransaction<T, CallbackAtConstruction>&) = delete;
    PropertyTransaction(PropertyTransaction<T, CallbackAtConstruction>&&) = delete;
    PropertyTransaction<T, CallbackAtConstruction> &operator=(const PropertyTransaction<T, CallbackAtConstruction>&) = delete;

private:
    Property<T, CallbackAtConstruction> &property;
};