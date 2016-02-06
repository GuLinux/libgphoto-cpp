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

#ifndef GPHOTO_CPP_WRAPPER_H
#define GPHOTO_CPP_WRAPPER_H
#include "fwd.h"
#include <functional>
#include <gphoto2/gphoto2.h>

namespace GPhoto {
  class GPhotoWrapper {
  public:
    typedef std::function<int()> GPhotoRun;
    int operator()(GPhotoRun run);
    GPhotoWrapper();
    ~GPhotoWrapper();
  private:
    DPTR
  };
  class GPhotoDriver {
  public:
    GPhotoDriver();
    GPhotoDriver(::GPContext *context);
    ~GPhotoDriver();
    typedef std::function<int(::GPContext*)> ContextRun;
    int operator() (ContextRun run);
    operator GPhotoWrapperPtr() const;
  private:
    DPTR
  };
  
  class GPhotoCamera {
  public:
    GPhotoCamera(::Camera *camera, const GPhotoDriverPtr &driver);
    ~GPhotoCamera();
    typedef std::function<int(::GPContext*, ::Camera*)> CameraRun;
    int operator() (std::function< int(::GPContext*, ::Camera*)> run);
    operator GPhotoDriverPtr() const;
    operator GPhotoWrapperPtr() const;
  private:
    DPTR
  };
};

GPhoto::GPhotoWrapperPtr operator<<(const GPhoto::GPhotoWrapperPtr &, GPhoto::GPhotoWrapper::GPhotoRun);
GPhoto::GPhotoDriverPtr operator<<(const GPhoto::GPhotoDriverPtr &, GPhoto::GPhotoDriver::ContextRun);
GPhoto::GPhotoCameraPtr operator<<(const GPhoto::GPhotoCameraPtr &, GPhoto::GPhotoCamera::CameraRun);

#define GP2_RUN(...) [__VA_ARGS__]() -> int
#define CTX_RUN(...) [__VA_ARGS__](::GPContext *gp_ctx) -> int
#define CAM_RUN(...) [__VA_ARGS__](::GPContext *gp_ctx, ::Camera *gp_cam) -> int

#endif
