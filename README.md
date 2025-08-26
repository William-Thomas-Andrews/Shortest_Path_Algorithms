# Shortest Path Project

## Overview
This project implements multiple **shortest path algorithm** in C++ with support for efficient data structures and utilities provided by [Folly](https://github.com/facebook/folly).  
The goal is to explore high-performance implementations of graph routing algorithms in a road transit-like simulation.

## Features
- Reads data from an easy-to-use json format with Folly.
- Implements shortest path algorithms (e.g., Dijkstra, A*, Parallel-Bidirectional A*).  
- The bidirectional A* algorithm uses **std::thread** to parallelize the normal A* algorithm to achieve better results than the seriel version with multithreading.
- Supports directed weighted graphs.
- Includes test cases and benchmarks.
- Includes a C-style array-based **Matrix** class that will soon implement an adjacency matrix in the **Graph** class.

## Requirements
- C++20 or later  
- [Folly](https://github.com/facebook/folly)  
- CMake (for build system)  

## Installation
```bash
# Clone the repository
git clone https://github.com/William-Thomas-Andrews/Shortest_Path_Algorithms.git
cd Shortest_Path_Algorithms

# Build
cd scripts/
./configure.sh
./build.sh
./run.sh