#pragma once

#include "Property.hpp"

template <typename T>
class Property; // Forward declaration for PropertyTransaction.

template <typename T>
class PropertyTransaction{
public:
    explicit PropertyTransaction(Property<T> &property) : property { property }, data { property.data } { 
        if (property.presetter){
            property.presetter(data);
        }
    }
    ~PropertyTransaction() { 
        if (property.postsetter){
            property.postsetter(data);
        }
    }

    T &data; // Access this member variable to modify the property data.

    // This copy/move construcrors and assign operator are deleted.
    PropertyTransaction(const PropertyTransaction<T>&) = delete;
    PropertyTransaction(PropertyTransaction<T>&&) = delete;
    PropertyTransaction<T> &operator=(const PropertyTransaction<T>&) = delete;

private:
    Property<T> &property;
};