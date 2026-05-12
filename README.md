# `bb` containers

A header only library aiming to reimplement and mimic the C++ standard named requirements, [Container](https://en.cppreference.com/cpp/named_req#Container), and as such the C++ standard container library.

## Requirements

- C++20 compatible compiler.

## Containers

The following containers have been implemented.

| Container   | Description          | Standard library equivalent |
| ----------- | -------------------- | --------------------------- |
| `Vector`    | A resizable array    | `std::vector`.              |
| `HashTable` | A hash-table         | `std::unordered_map`.       |
| `List`      | A doubly linked-list | `std::list`                 |

## Testing

[Catch2](https://github.com/catchorg/Catch2) is used for the tests.

```shell
# once built run:
ctest --test-dir build
```
