# f.ixx - C++23 Modular Utility Library

> A modern C++ utility library built with C++23 Modules, providing core functionalities for memory management, containers, type erasure, and more.

[中文版本](./README.md)

## 📋 Introduction

`f.ixx` is a modular utility library built using the C++23 standard. The project fully leverages C++23's module features to provide a series of high-performance, type-safe utility components, including:

- **Smart Memory Management** - Memory pools and smart pointers based on `std::pmr`
- **Polymorphic Containers** - Dynamic vector and deque supporting heterogeneous types
- **Type Erasure** - Runtime type information and type-erasure containers
- **String Processing** - Formatting and encoding conversion utilities
- **Utility Functions** - Assertions, hashing, exception handling, and more

## 🎯 Core Modules

### `f::memory` - Memory Management

Advanced memory management tools based on `std::pmr`:

```cpp
// Size-based memory pool
f::sync_sized_pool<1024> pool;  // Thread-safe sized pool
f::sized_pool<2048> pool;       // Non-thread-safe sized pool

// PMR smart pointers
auto ptr = f::make_unique<MyClass>(&resource, arg1, arg2);
auto arr = f::make_unique_array<int>(100, &resource);

// Type-safe upcast/downcast
auto base_ptr = f::unique_upcast<Base>(std::move(derived_ptr));
auto derived = f::unique_downcast<Derived>(base_ptr);
```

### `f::vector` & `f::deque` - Polymorphic Containers

Dynamic containers that support polymorphic objects with automatic lifecycle management:

```cpp
f::dynamic_vector<Shape> shapes;
shapes.emplace_back<Circle>(radius);
shapes.emplace_back<Rectangle>(width, height);

// Automatic release, type-safe
auto shape = shapes.release_back();
```

### `f::rtt` - Runtime Type & Type Erasure

- **`type_t`** - Runtime type information supporting default construction, copy, move, destruct, etc.
- **`any`** - Type-erasure container supporting storage of any type with type-safe access
- **Type conversion tools** - `is_instance<U>` and `as<D>(obj)` for safe dynamic type casting

```cpp
// Runtime type information
auto type_info = f::type_t::of<MyClass>();
type_info.make(obj);           // Default construct
type_info.copy(dst, src);      // Copy
type_info.destroy(obj);        // Destruct

// Type erasure
f::any container;
auto& ref = container.emplace<MyType>(args...);
auto& value = container.as<MyType>();
```

### `f::string` - String Processing

- **PMR Formatting** - String formatting with custom memory resource support
- **Encoding Conversion** - Safe conversion between multibyte and wide characters

```cpp
// Format with default resource
auto str = f::format("Hello, {}!", name);

// Format with custom memory resource
auto str2 = f::format_with_resource("Value: {}", &resource, value);

// Encoding conversion
std::wstring wide = f::cvt(narrow_string);
std::string narrow = f::cvt(wide_string);
```

### `f::hash` - Hash Utilities

Transparent string hasher for heterogeneous lookups:

```cpp
std::unordered_map<std::string, int, f::string_hasher, std::equal_to<>> map;
map["key"] = 42;
auto it = map.find("key"sv);  // No need to construct string
```

### `f::exception` - Exception Handling

Exception creation utility based on `std::format`:

```cpp
throw f::exception("Expected {}, but got {}", expected, actual);
```

### `f::assert` - Assertions

Simple assertion utility that throws on failure:

```cpp
f::assert(ptr != nullptr);
```

### `f::utilities` - Utility Functions

Value validation tools using pipe syntax:

```cpp
f::equal_or_throw{expected} >> actual;
f::not_equal_or_throw{nullptr} >> ptr;
```

### `f::pack` - Type List Utilities

Template metaprogramming tools for accessing type packs:

```cpp
using pack = f::pack<int, double, std::string>;
using first = pack::first;           // int
using last = pack::last;             // std::string
using second = pack::at<1>;          // double
```

## 🚀 Quick Start

### Prerequisites

- **Compiler**: MSVC (currently the only compiler with full C++23 Modules support)
- **CMake**: Version 4.1 or higher
- **IDE**: CLion recommended

### Build Steps

1. **Clone the repository**

```bash
git clone <repository-url>
cd f.ixx
```

2. **Open project in CLion**

   - Select `File -> Open`, choose the `f.ixx` directory

3. **Configure MSVC Toolchain** (if not configured)

   - `File -> Settings -> Build, Execution, Deployment -> Toolchains`
   - Click `+` to add a new toolchain, select `Visual Studio`
   - Enter your local Visual Studio installation path

4. **Configure CMake**

   - `File -> Settings -> Build, Execution, Deployment -> CMake`
   - Set `Generator` to `Let CMake decide`
   - Click `Reset Cache and Reload CMake Project`

5. **Build the project**

   - Select `Build -> Build Project`

### CMake Integration

Integrate `f.ixx` into your project:

```cmake
# Add subdirectory
add_subdirectory(path/to/f.ixx/modules/f)

# Link library
target_link_libraries(your_target PRIVATE f)
```

## 📁 Project Structure

```
f.ixx/
├── CMakeLists.txt              # Root CMake configuration
├── cmake/
│   └── f.scaffolding.cmake     # Module build helper functions
├── modules/
│   └── f/
│       ├── CMakeLists.txt      # f module build configuration
│       └── src/
│           ├── f.ixx           # Main module (exports all submodules)
│           ├── assert.ixx      # Assertion utilities
│           ├── deque.ixx       # Polymorphic deque container
│           ├── exception.ixx   # Exception handling
│           ├── hash.ixx        # Hash utilities
│           ├── memory.ixx      # Memory management
│           ├── pack.ixx        # Type list utilities
│           ├── rtt.ixx         # Runtime type & type erasure
│           ├── string.ixx      # String processing
│           ├── utilities.ixx   # Utility functions
│           └── vector.ixx      # Polymorphic vector container
└── README.md
```

## 🔧 Technical Features

- **C++23 Modules** - Modular compilation for improved compile times and code organization
- **PMR (Polymorphic Memory Resource)** - Flexible memory management strategies
- **Concepts** - C++20/23 Concepts for compile-time type constraints
- **Modern C++ Best Practices** - RAII, move semantics, perfect forwarding, etc.

## 📝 License

This project is licensed under the terms specified in the [LICENSE](./LICENSE) file.

## 🤝 Contributing

Issues and Pull Requests are welcome!

## 📧 Contact

- Creator: MuXin
- Project Period: 2025-2026
