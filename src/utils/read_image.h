/*
 *     Driver type: GPhoto Camera INDI Driver
 * 
 *     Copyright (C) 2016 Marco Gulino (marco AT gulinux.net)
 * 
 *     This library is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU Lesser General Public License as published
 *     by the Free Software Foundation; either version 2.1 of the License, or
 *     (at your option) any later version.
 * 
 *     This library is distributed in the hope that it will be useful, but
 *     WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *     or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *     License for more details.
 * 
 *     You should have received a copy of the GNU Lesser General Public License
 *     along with this library; if not, write to the Free Software Foundation,
 *     Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 * 
 */

#ifndef GPHOTO_READ_IMAGE_H
#define GPHOTO_READ_IMAGE_H

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