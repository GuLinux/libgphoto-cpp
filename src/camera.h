/*
 * libgphoto++ - modern c++ wrapper library for gphoto2
 * Copyright (C) 2016 Marco Gulino <marco AT gulinux.net>
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
*/

#ifndef GPHOTO_CAMERA_H
#define GPHOTO_CAMERA_H

#include "fwd.h"
#include "dptr.h"
#include <functional>
#include <future>
#include <ostream>
#include <chrono>
#include <ratio>
namespace GPhoto {

class Camera
{
public:
    Camera(const GPhotoCameraPtr &camera, const LoggerPtr &logger = {});
    ~Camera();
    WidgetPtr settings() const;
    void save_settings();
    std::string summary() const;
    std::future<CameraFilePtr> shoot_preset() const;
    std::future<CameraFilePtr> shoot_bulb(const std::chrono::duration<double, std::milli> &exposure, const ShooterPtr &shooter) const;
private:
  DPTR
};
}

std::ostream &operator<<(std::ostream &o, const GPhoto::Camera &c);
inline std::ostream &operator<<(std::ostream &o, const GPhoto::CameraPtr &c) { return o << *c; }

#endif // GPHOTO_CAMERA_H
