cmake_minimum_required(VERSION 3.24.4)

project(MyProgram)

# Thêm thư mục chứa các file header
include_directories(include)

# Lấy các cờ và thư viện từ root-config
execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)

# Thêm các cờ và thư viện từ root-config vào biến CMAKE_CXX_FLAGS và CMAKE_EXE_LINKER_FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ROOT_LD_FLAGS}")

# Tìm tất cả các file cpp trong thư mục src
file(GLOB SOURCES "src/*.cpp")

# Tạo target cho chương trình từ file main.cpp và các file cpp khác
add_executable(MyProgram ${SOURCES} src/main.cpp)


