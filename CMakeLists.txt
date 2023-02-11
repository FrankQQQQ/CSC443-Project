cmake_minimum_required(VERSION 3.16)
project(KV_STORE LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "-lstdc++fs")

# add the CMakeFile that defines catch2
add_subdirectory(3rd_party/catch2)

# Add an library for the example classes
add_library(kv_store
        KVStore.h
        Memtable.h
        KVPair.cpp
        KVPair.h
        SortedStringsTableDriver.cpp
        SortedStringsTableDriver.h)

add_executable(KVStoreCommandLine KVStoreCommandLine.cpp)
target_link_libraries(KVStoreCommandLine kv_store)

#############################################
# Unit tests
# enable CTest Testing
enable_testing()

# Add a testing executable
add_executable(SortedStringsTableDriverTest test/SortedStringsTableDriverTest.cpp)
target_link_libraries(SortedStringsTableDriverTest
        kv_store
        Catch2::Test)

add_test(test_SortedStringsTableDriverTest SortedStringsTableDriverTest)
