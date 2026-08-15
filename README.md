# f.ixx - C++23 模块化工具库

> 一个基于 C++23 Modules 的现代化 C++ 工具库，提供内存管理、容器、类型擦除等核心功能。

## 📋 项目简介

`f.ixx` 是一个使用 C++23 标准构建的模块化工具库项目。项目充分利用 C++23 的模块特性（Modules），提供了一系列高性能、类型安全的工具组件，包括：

- **智能内存管理** - 基于 `std::pmr` 的内存池和智能指针
- **多态容器** - 支持 heterogeneous 的动态 vector 和 deque
- **类型擦除** - 运行时类型信息与类型擦除容器
- **字符串处理** - 格式化与编码转换工具
- **工具函数** - 断言、哈希、异常处理等实用工具

## 🚀 快速开始

### 前置要求

- **编译器**: MSVC（目前唯一完全支持 C++23 Modules 的编译器）
- **CMake**: 版本 4.1 或更高
- **IDE**: 推荐使用 CLion

### CMake 集成

**推荐使用我们的[项目脚手架](https://github.com/f-collection-mcxx/scaffolding.ixx)进行开发**

克隆分发分支
```bash
git clone --branch module-f --single-branch https://github.com/f-collection-mcxx/f.ixx.git 3rdparty/f
```
在cmake中注册
```cmake
# 添加子目录
add_subdirectory(3rdparty/f)

# 链接库
target_link_libraries(your_target PRIVATE f)
```

## 📁 项目结构

```
f.ixx/
├── CMakeLists.txt              # 根 CMake 配置
├── cmake/
│   └── f.scaffolding.cmake     # 模块构建辅助函数
├── modules/
│   └── f/
│       ├── CMakeLists.txt      # f 模块构建配置
│       └── src/
│           ├── f.ixx           # 主模块（导出所有子模块）
│           ├── assert.ixx      # 断言工具
│           ├── deque.ixx       # 多态 deque 容器
│           ├── exception.ixx   # 异常处理
│           ├── hash.ixx        # 哈希工具
│           ├── memory.ixx      # 内存管理
│           ├── pack.ixx        # 类型列表工具
│           ├── rtt.ixx         # 运行时类型与类型擦除
│           ├── string.ixx      # 字符串处理
│           ├── utilities.ixx   # 工具函数
│           └── vector.ixx      # 多态 vector 容器
└── README.md
```

## 🔧 技术特性

- **C++23 Modules** - 使用模块化编译，提升编译速度和代码组织
- **PMR (Polymorphic Memory Resource)** - 灵活的内存管理策略
- **Concepts** - 使用 C++20/23 Concepts 提供编译时类型约束
- **现代 C++ 最佳实践** - RAII、移动语义、完美转发等

## 📝 许可证

本项目采用 [Boost](./LICENSE) 许可证。

## 🎯 核心模块

### `f::string` - 字符串处理

- **PMR 格式化** - 支持自定义内存资源的字符串格式化
- **编码转换** - 多字节与宽字符之间的安全转换

```cpp
// 编码转换
auto wide = f::cvt(narrow_string, std::wstring{});
auto narrow = f::cvt(wide_string, std::string{});
```

### `f::hash` - 哈希工具

提供透明比较的字符串哈希器，适用于 heterogeneous 查找：

```cpp
std::unordered_map<std::string, int, f::hash<std::string>, std::equal_to<>> map;
map["key"] = 42;
auto it = map.find("key"sv);  // 无需构造 string
```

### `f::exception` - 异常处理

基于 `std::format` 的异常创建工具：

```cpp
throw f::exception("Expected {}, but got {}", expected, actual);
```

### `f::assert` - 断言

简单的断言工具，失败时抛出异常：

```cpp
f::assert >> (ptr != nullptr);
```

### `f::utilities` - 工具函数

提供基于管道语法的值验证工具：

```cpp
f::equal_or_throw{expected} >> actual;
f::not_equal_or_throw{nullptr} >> ptr;
```



## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

## 📧 联系方式

- 创建者: MuXin
- 项目日期: 2025-2026
