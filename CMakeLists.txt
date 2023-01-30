cmake_minimum_required(VERSION 3.16)
project(KV_STORE)

set(CMAKE_CXX_STANDARD 20)

add_executable(KVStoreCommandLine KVStoreCommandLine.cpp KVStore.h Memtable.h KVPair.cpp KVPair.h SortedStringsTableDriver.cpp SortedStringsTableDriver.h)
