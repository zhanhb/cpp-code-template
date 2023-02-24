cmake_minimum_required(VERSION 3.19)
project(cpp-code-template)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

macro(add_compiler_flags)
    string(REPLACE ";" " " _FLAGS "${ARGV}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_FLAGS}")
endmacro()

macro(add_linker_flags)
    string(REPLACE ";" " " _FLAGS "${ARGV}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${_FLAGS}")
endmacro()

add_compiler_flags(-fno-omit-frame-pointer)

if (NOT WIN32)
    add_compiler_flags(-fsanitize=address)
    add_compiler_flags(-fsanitize=undefined)
endif ()

add_definitions(-DVERBOSE=1)
add_definitions(-D_GLIBCXX_FILESYSTEM=1)
add_definitions(-D_LIBCPP_ABI_NO_BINDER_BASES)
add_definitions(-D_LIBCPP_ABI_NO_ITERATOR_BASES)

add_executable(leetcode src/leetcode.cpp)
add_executable(oj src/oj.cpp)
add_executable(debug-test tests/debug-test.cpp)
add_executable(debug-test-98 tests/debug-test-98.cpp)
add_executable(stability-test tests/stability-test.cpp)
add_executable(vector-test tests/vector-test.cpp)

target_compile_options(oj BEFORE PRIVATE -include ; "${CMAKE_CURRENT_SOURCE_DIR}/include/debug.h")

#add_compile_options()
#target_link_libraries(${PROJECT_NAME} -fsanitize=signed-integer-overflow)
#add_compile_options(-fsanitize=signed-integer-overflow)
#add_definitions(-fsanitize=signed-integer-overflow)
#add_definitions(-fsanitize=undefined)
#target_link_libraries(${PROJECT_NAME} -fsanitize=undefined)
#set(CMAKE_XCODE_SCHEME_UNDEFINED_BEHAVIOUR_SANITIZER_STOP ON)
#set(CMAKE_XCODE_SCHEME_UNDEFINED_BEHAVIOUR_SANITIZER ON)
