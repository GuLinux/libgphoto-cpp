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

namespace GPhotoCPP {
class ReadImage {
public:
  struct Image {
    int w,h;
    int bpp;
    enum Channel { Red, Green, Blue, Grey };
    typedef std::vector<uint8_t> Pixels;
    std::string filename;
    std::map<std::string, std::string> metadata;
    std::map<Channel, Pixels> channels;
  private:
    friend class ReadImage;
    Pixels &init_channel(Channel channel);
  };
  virtual Image read(const std::string &file_path) = 0;
  virtual Image read(const std::vector<uint8_t> &data, const std::string &filename) = 0;
  typedef std::shared_ptr<ReadImage> ptr;
  
  struct FileInfo {
    std::string filename;
    std::string mimetype;
  };
  static ptr factory(const FileInfo &fileinfo);
protected:
  Image::Pixels &init_channel(Image &image, Image::Channel channel) { return image.init_channel(channel); }
};
}
#endif