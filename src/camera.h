#pragma once
#ifndef LIBGPHOTO_CPP_CAMERA_H
#define LIBGPHOTO_CPP_CAMERA_H
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
#include "utils/dptr.h"
#include <functional>
#include <future>
#include <ostream>
#include <chrono>
#include <ratio>
namespace GPhotoCPP {

class Camera : public std::enable_shared_from_this<Camera>
{
public:
    class Settings;
    class Control;
    typedef std::shared_ptr<Settings> SettingsPtr;
    typedef std::shared_ptr<Control> ControlPtr;
    struct MirrorLock {
      milliseconds duration;
      ShooterPtr shooter;
      MirrorLock(const milliseconds &duration = {}, const ShooterPtr shooter = {});
      operator bool() const;
    };
    class Shot;
    typedef std::shared_ptr<Shot> ShotPtr;
    Camera(const GPhotoCameraPtr &camera, const LoggerPtr &logger = {});
    ~Camera();
    WidgetPtr settings() const;
    void save_settings();
    std::string summary() const;
    std::list<std::string> folders(const std::string &folder) const;
    struct FileInfo {
      std::string folder;
      std::string name;
      std::string path() const;
    };
    std::list<FileInfo> files(const std::string &folder) const;
    
    CameraFolderPtr root(const std::string &root_path="/");
    ShotPtr shoot_preset(const MirrorLock &mirror_lock = {}) const;
    ShotPtr shoot_bulb(const milliseconds &exposure, const ShooterPtr &shooter, const MirrorLock &mirror_lock = {}) const;
private:
  DPTR
};

class Camera::Shot {
public:
  CameraFileFuture& camera_file() const;
  CameraFileFuture &operator*() const;
  milliseconds elapsed() const;
  milliseconds duration() const;
private:
  Shot(const milliseconds &duration);
  friend class Camera;
  friend class Camera::Private;
  DPTR
};

}

std::ostream &operator<<(std::ostream &o, const GPhotoCPP::Camera &c);
inline std::ostream &operator<<(std::ostream &o, const GPhotoCPP::CameraPtr &c) { return o << *c; }

#include "camera_settings.h"
#include "camera_control.h"

#endif // LIBGPHOTO_CPP_CAMERA_H
