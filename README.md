# LSM Storage System

This project implements a Log-Structured Merge-tree (LSM-tree) based storage system. LSM-trees are a storage architecture designed to provide high write throughput and low read latency, making them suitable for write-intensive workloads.

## Overview

The storage system is composed of three main components: `Memtable`, `SST`, and `BufferPool`. 

- `Memtable` is an in-memory data structure that stores key-value pairs. It uses a balanced search tree (AVL tree) as its underlying data structure. This allows for efficient insertion, deletion, and retrieval operations.
- `SST` (Sorted String Table) is an on-disk data structure that stores key-value pairs in sorted order. It provides efficient range scan and binary search capabilities.
- `BufferPool` is a cache that stores recently accessed key-value pairs in memory to reduce the number of disk accesses required for read operations.

## Algorithms and Data Structures

- Memtable: The Memtable uses an AVL tree, which is a self-balancing binary search tree. This ensures that the tree remains balanced after every insertion or deletion, leading to efficient logarithmic-time operations.
- SST: The SST component uses a binary search algorithm to locate a specific key within a sorted file. This allows for efficient key-value pair retrieval with logarithmic-time complexity.
- BufferPool: The BufferPool can be implemented using various cache eviction policies, such as Least Recently Used (LRU) or First In, First Out (FIFO), to manage the storage of key-value pairs in memory.

## Functionalities

The storage system provides the following functionalities:

1. Put a key-value pair in the Memtable.
2. Get the value for a key from the Memtable, SST, or BufferPool.
3. Scan a range of keys in the Memtable or SST.
4. Store key-value pairs from the Memtable to an SST file on disk.
5. Manage the cache of key-value pairs in the BufferPool.

To demonstrate these functionalities, refer to the `demonstrateFeatures` function in the `run.cpp` file.

## Usage

To compile and run the project, use the following commands:

g++ -std=c++17 -o run run.cpp Memtable.cpp SST.cpp BufferPool.cpp
./run

This will execute the `run.cpp` file, which demonstrates the features of the LSM storage system.
