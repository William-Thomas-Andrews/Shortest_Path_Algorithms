# Shortest Path Project

## Overview
This project implements a **shortest path algorithm** in C++ with support for efficient data structures and utilities provided by [Folly](https://github.com/facebook/folly).  
The goal is to explore high-performance implementations of graph routing algorithms.

## Features
- Implements shortest path algorithms (e.g., Dijkstra, A*).  
- Optimized for performance using Folly containers/utilities.  
- Supports weighted graphs (directed/undirected).  
- Configurable input graph format (e.g., adjacency list, edge list).  
- Includes test cases and benchmarks.  

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