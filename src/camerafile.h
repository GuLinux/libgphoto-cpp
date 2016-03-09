#pragma once
#ifndef LIBGPHOTO_CPP_CAMERAFILE_H
#define LIBGPHOTO_CPP_CAMERAFILE_H
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


#include "fwd.h"
#include <vector>
#include <ostream>

namespace GPhotoCPP {

class CameraFile
{
public:
  struct Info {
    size_t size, width, height;
  };
  CameraFile(const std::string &folder, const std::string &file, const GPhotoCameraPtr &camera, const LoggerPtr &logger);
  ~CameraFile();
  void save(const std::string &path);
  void copy(std::vector<uint8_t> &data);
  std::vector<uint8_t> data();
  std::string path() const;
  std::string folder() const;
  std::string file() const;
  Info info() const;
  void delete_on_camera();
private:
    DPTR
};
}

std::ostream &operator<<(std::ostream &o, const GPhotoCPP::CameraFile::Info &info);
std::ostream &operator<<(std::ostream &o, const GPhotoCPP::CameraFile &camera_file);
#endif // LIBGPHOTO_CPP_CAMERAFILE_H
