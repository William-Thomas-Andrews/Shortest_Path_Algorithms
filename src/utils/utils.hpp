#pragma once
#include <vector>
#include <iostream>
#include <atomic>

std::vector<int> get_and_print_path(const std::vector<int>& prev, int start, int end);
std::vector<int> get_and_print_path(const std::vector<std::atomic<int>>& prev, int start, int end);