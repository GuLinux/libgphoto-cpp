#pragma once
#ifndef LIBGPHOTO_CPP_CAMERA_FILESYSTEM_H
#define LIBGPHOTO_CPP_CAMERA_FILESYSTEM_H
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
 * 
 */


#include "fwd.h"
#include "utils/dptr.h"

namespace GPhotoCPP {

class CameraFolder : public std::enable_shared_from_this<CameraFolder>
{
public:
  CameraFolder(const std::string &path, const GPhotoCameraPtr &gphoto_camera, const LoggerPtr &logger, const CameraFolderPtr &parent = {});
  std::list<CameraFolderPtr> folders() const;
  std::list<CameraFileInfoPtr> files() const;
  std::string path() const;
  CameraFolderPtr parent() const;
  ~CameraFolder();

private:
    DPTR
};

class CameraFileInfo {
public:
  CameraFileInfo(const std::string &name, const CameraFolderPtr &folder, const GPhotoCameraPtr &gphoto_camera, const LoggerPtr &logger);
  CameraFolderPtr parent() const;
  std::string path() const;
  std::string name() const;
  void remove();
  CameraFilePtr camera_file() const;
private:
  DPTR
};
}
#endif // LIBGPHOTO_CPP_CAMERA_FILESYSTEM_H
