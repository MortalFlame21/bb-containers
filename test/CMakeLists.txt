Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.14.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(container_tests
  Vector.cpp
  List.cpp
  HashTable.cpp
)
target_link_libraries(container_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(container_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

include(CTest)
include(Catch)
catch_discover_tests(container_tests ADD_TAGS_AS_LABELS)