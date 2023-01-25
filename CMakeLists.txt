cmake_minimum_required(VERSION 3.24)
project(KV_STORE)

set(CMAKE_CXX_STANDARD 20)

add_executable(KVStoreCommandLine KVStoreCommandLine.cpp KVStore.hpp Memtable.h)
