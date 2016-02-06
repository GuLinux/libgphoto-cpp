/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef GPHOTO_CAMERAFILE_H
#define GPHOTO_CAMERAFILE_H

#include "fwd.h"
#include <vector>

namespace GPhoto {

class CameraFile
{
public:
  struct Info {
    size_t size, width, height;
  };
  CameraFile(const std::string &folder, const std::string &file, const GPhotoCameraPtr &camera);
  ~CameraFile();
  void save(const std::string &path);
  void copy(std::vector<uint8_t> &data);
  std::vector<uint8_t> data();
  Info info() const;
  void delete_on_camera();
private:
    DPTR
};
}

#endif // GPHOTO_CAMERAFILE_H
