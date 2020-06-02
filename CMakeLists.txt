cmake_minimum_required(VERSION 3.5)

project(threadpool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt5 COMPONENTS Core REQUIRED)


add_executable(${PROJECT_NAME}
   "src/main.cpp" "src/threadpool.h" "src/threadpool.cpp" "src/stringsconstants.h"
   "src/interface/itask.h" "src/interface/itask.cpp" "src/workers/countertask.h" 
   "src/workers/countertask.cpp" "src/workers/filereadertask.h" "src/workers/filereadertask.cpp"
   "src/workers/filewrittertask.h" "src/workers/filewrittertask.cpp"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core)

