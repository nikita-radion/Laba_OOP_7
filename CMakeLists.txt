cmake_minimum_required(VERSION 3.14)
project(BalagurFate3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_library(game_lib
    src/NPC.cpp
    src/Orc.cpp
    src/Druid.cpp
    src/Squirrel.cpp
    src/NPCFactory.cpp
    src/Game.cpp
    src/GameState.cpp
    src/ConsoleObserver.cpp
    src/FileObserver.cpp
    src/BattleVisitor.cpp
    src/CoroutineGame.cpp
)

target_include_directories(game_lib PUBLIC include)

add_executable(game src/main.cpp)
target_link_libraries(game 
    PRIVATE 
    game_lib
    Threads::Threads  
)

enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(game_tests
    tests/test_main.cpp
    tests/npc_tests.cpp
    tests/battle_tests.cpp
    tests/factory_tests.cpp
)

target_link_libraries(game_tests
    PRIVATE
    game_lib
    Threads::Threads  
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(game_tests) 

add_executable(coroutine_game src/coroutine_main.cpp)
target_link_libraries(coroutine_game 
    PRIVATE 
    game_lib
    Threads::Threads
)

target_compile_options(coroutine_game PRIVATE -fcoroutines) 