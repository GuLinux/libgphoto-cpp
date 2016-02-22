#pragma once
/*
 * libgphoto++ - modern c++ wrapper library for gphoto2
 * Copyright (C) 2016 Marco Gulino <marco AT gulinux.net>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <functional>
#include <chrono>
namespace GPhotoCPP {
struct Scope {
  std::function<void()> on_exit;
  ~Scope() { on_exit(); }
};

template<typename T, typename clock_type = std::chrono::high_resolution_clock>
class count_elapsed {
public:
  count_elapsed(std::function<void(T)> on_finish) : on_finish{on_finish} { start = clock_type::now(); }
  ~count_elapsed() {
    auto end = clock_type::now();
    auto elapsed = std::chrono::duration_cast<T>(end-start);
    on_finish(elapsed);
  }
private:
  std::chrono::time_point<clock_type> start;
  std::function<void(T)> on_finish;
};

}