# Property<T>

Use property in C++ (>11).

## How to use

### Definition

```c++
Property<int> myInt1 { 1 };
Property<int> myInt2 = 2;

int existingInt = 3;
Property<int> myInt3 { existingInt };
```

The construction is perfectly forwarded.

```c++
Property<std::string> myString1 { "My String 1" }; // Copy constructed
Property<std::string> myString2 { std::string { "My String 2" } }; // Move constructed

std::string existingString { "My String 3" };
Property<std::string> myString3 { std::move(existingString) }; // Move constructed
```

### Getter

```c++
std::cout << myInt1.get(); // Output: 1

/*
    the return type is reference type, therefore there is no overhead for copying something
*/
std::cout << myString1.get(); // Output: My String 1

// Error!
// myInt2.get() = 3;
```

The result type of `Property<T>.get()` is `const T&`, therefore the data cannot be modified with the function.

### Setter

```c++
myInt1.set(-1);
myInt2 = -2;
std::cout << myInt1.get() << ' ' << myInt2.get(); // Output: -1 -2

myString1.set("The string is modified");
std::cout << myString1.get(); // Output: The string is modified
```

Not only be able to change the data itself, but also change the data's inner state.

```c++
Property<std::vector<int>> myVector = { 1, 2, 3, 4, 5 };

// Open transaction
{
    decltype(myVector)::Transaction t { myVector };
    t.data.push_back(6);
    t.data.push_back(7);
}

for (const int e : myVector){
    std::cout << e << ' '; // Output: 1 2 3 4 5 6 7
}
```

### Presetter and Postsetter

You can define the action _before_ the setter (presetter) and _after_ the setter (postsetter). e.g.

```c++
std::string now(); // Get the current time in format 'HH:mm:ss'.

std::string serial_last_updated;
Property<int> serial {
    0, // the value
    { }, // presetter
    [&](int newValue) -> void { serial_last_updated = now(); std::cout << "The serial is updated to " << newValue << " at " << serial_last_updated; } // postsetter
};

// Postsetter executed after serial constructed.

serial.set(3);

/*
    [Final output]
    The serial is updated to 0 at [time].
    The serial is updated to 3 at [time].
*/
```

## ReadonlyProperty<T>

There is a nonmodifiable property, `ReadonlyProperty<T>`.

```c++
ReadonlyProperty<bool> nonmodifiable { false };
std::cout << std::boolalpha << nonmodifiable.get(); // Output: false
```

