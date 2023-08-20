# cmake 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 项目名称
project(chat)

set(CMAKE_CXX_STANDARD 11)
# 配置编译选项
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} -g)

# 指定可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 指定头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/MysqlConnectionPool)
include_directories(${PROJECT_SOURCE_DIR}/include/model)
include_directories((${PROJECT_SOURCE_DIR}/include/redis))
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)


# 加载子目录
add_subdirectory(src)
