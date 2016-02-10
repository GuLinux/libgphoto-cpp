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

#ifndef GPHOTO_CAMERA_FILESYSTEM_H
#define GPHOTO_CAMERA_FILESYSTEM_H

#include "fwd.h"
#include "utils/dptr.h"

namespace GPhoto {

class CameraFolder : public std::enable_shared_from_this<CameraFolder>
{
public:
  CameraFolder(const std::string &path, const GPhotoCameraPtr &gphoto_camera, const CameraFolderPtr &parent = {});
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
  CameraFileInfo(const std::string &name, const CameraFolderPtr &folder, const GPhotoCameraPtr &gphoto_camera);
  CameraFolderPtr parent() const;
  std::string path() const;
  std::string name() const;
  CameraFilePtr camera_file() const;
private:
  DPTR
};
}

#endif // GPHOTO_CAMERAFOLDER_H
