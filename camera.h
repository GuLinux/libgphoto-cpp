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

#ifndef GPHOTO_CAMERA_H
#define GPHOTO_CAMERA_H

#include "fwd.h"
#include "dptr.h"
#include <functional>
#include <future>
#include <ostream>
namespace GPhoto {

class Camera
{
public:
    Camera(const GPhotoCameraPtr &camera, const LoggerPtr &logger = {});
    ~Camera();
    WidgetPtr settings() const;
    std::string summary() const;
    std::future<CameraFilePtr> shoot_preset() const;
    std::future<CameraFilePtr> shoot_bulb(double exposure_msec, const ShooterPtr &shooter) const;
private:
  DPTR
};
}

std::ostream &operator<<(std::ostream &o, const GPhoto::Camera &c);
inline std::ostream &operator<<(std::ostream &o, const GPhoto::CameraPtr &c) { return o << *c; }

#endif // GPHOTO_CAMERA_H
