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

#ifndef LIBGPHOTO_CPP_UTILS_READ_IMAGE_H
#define LIBGPHOTO_CPP_UTILS_READ_IMAGE_H

#include <vector>
#include <string>
#include <memory>
#include <map>

namespace GPhoto {
class ReadImage {
public:
  struct Image {
    int axis;
    int w,h;
    int bpp;
    std::vector<uint8_t> data;
    std::string filename;
    std::map<std::string, std::string> metadata;
  };
  virtual Image read(const std::string &file_path) = 0;
  virtual Image read(const std::vector<uint8_t> &data, const std::string &filename) = 0;
  typedef std::shared_ptr<ReadImage> ptr;
};
}
#endif