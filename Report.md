Database System Implementation Report

This report summarizes the implementation and efficiency analysis of the KV-Store database system comprising four key components: Memtable, SortedStringTable (SST), BufferPool, and EvictionPolicy. The database system is designed to store and manage key-value pairs efficiently.

Memtable:
The Memtable is an in-memory data structure that temporarily stores key-value pairs in a balanced search tree. It supports insert, get, and delete operations.

Efficiency Analysis:

a) Insert: O(log N), where N is the number of key-value pairs in the Memtable. This operation is efficient, as the search tree enables the insertion of elements in logarithmic time.

b) Get: O(log N), where N is the number of key-value pairs in the Memtable. This operation is efficient, as the search tree allows for fast lookups in logarithmic time.

c) Delete: O(log N), where N is the number of key-value pairs in the Memtable. This operation is efficient, as the search tree enables the deletion of elements in logarithmic time.

When the Memtable reaches its maximum size, its contents are flushed to an SST file.

SortedStringTable (SST):
The SortedStringTable (SST) is a persistent storage mechanism that stores key-value pairs in a sorted manner. When a Memtable is full, its contents are flushed to an SST file using the store() function. The SST class also supports binarySearchFile() for searching a key within an SST file.

Database System:
The database system provides the following functionalities: Open, Close, and Scan.

Efficiency Analysis:

a) Open: The efficiency of the 'open' operation mainly depends on the time taken to load existing SST files, which is proportional to the number of SST files and their sizes. However, this operation is performed only once during the initialization of the database system.

b) Put: The 'put' operation has an average-case time complexity of O(log N) for inserting or updating a key-value pair in the Memtable, where N is the number of entries in the Memtable. However, if the Memtable becomes full, the operation's efficiency is affected by the time taken to flush the Memtable to an SST file, which is O(N log N) due to the sorting step. This cost is amortized over multiple 'put' operations.

c) Get: The 'get' operation's efficiency depends on whether the requested key-value pair is found in the Memtable, BufferPool, or SST files. Searching in the Memtable takes O(log N) time, while searching in the BufferPool depends on the eviction policy (LRU or Clock) and the number of entries in the BufferPool. Searching in the SST files takes O(log M) time, where M is the number of entries in the SST file. The overall efficiency depends on the distribution of key-value pairs across these data structures.

d) Scan: The 'scan' operation's efficiency depends on the range of key-value pairs being requested and the distribution of these pairs across the Memtable, BufferPool, and SST files. The time complexity of scanning through the Memtable is O(log N + R), where R is the number of key-value pairs in the requested range. For the BufferPool and SST files, the efficiency depends on the eviction policy and the number of entries in these data structures.

e) Close: The 'close' operation's efficiency mainly depends on the time taken to flush the remaining data in the Memtable to an SST file and close all open SST files. The time complexity of flushing the Memtable is O(N log N) due to the sorting step. Closing the SST files takes time proportional to the number of open files.

BufferPool:
The BufferPool is a memory-efficient data structure that caches frequently accessed pages from SST files. It supports insertion, deletion, and lookup of pages, and can grow or shrink based on the EvictionPolicy chosen (LRU or Clock).

Efficiency Analysis:

a) Insert: O(1) - This operation is efficient, as it involves inserting a page into a specific bucket in the directory.

b) Delete: O(1) - This operation is efficient, as it involves deleting a page from a specific bucket in the directory.

c) Lookup: O(1) - This operation is efficient, as it involves searching for a page within a specific bucket in the directory.

EvictionPolicy:
The BufferPool supports two eviction policies: LRU (Least Recently Used) and Clock. These policies determine which pages should be evicted from the BufferPool when it reaches its maximum size.

Efficiency Analysis:

a) LRU: O(N), where N is the number of pages in the BufferPool. This operation has linear complexity, as it involves searching for the least recently used page within the entire BufferPool.

b) Clock: O(N), where N is the number of pages in the BufferPool. This operation has linear complexity, as it involves iterating through pages in the BufferPool with a clock hand to determine which page should be evicted.
