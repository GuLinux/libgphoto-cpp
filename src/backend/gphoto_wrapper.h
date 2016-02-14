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

#ifndef GPHOTO_CPP_WRAPPER_H
#define GPHOTO_CPP_WRAPPER_H
#include "fwd.h"
#include <functional>
#include <gphoto2/gphoto2.h>

namespace GPhoto {
  struct GPhotoReturn {
    int result;
    const char *file;
    int line;
    const char *function;
    operator int() const;
    operator std::string() const;
    std::string describe() const;
  };
  class GPhotoWrapper {
  public:
    typedef std::function<GPhotoReturn()> GPhotoRun;
    GPhotoReturn operator()(GPhotoRun run);
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
    typedef std::function<GPhotoReturn(::GPContext*)> ContextRun;
    GPhotoReturn operator() (ContextRun run);
    operator GPhotoWrapperPtr() const;
  private:
    DPTR
  };
  
  class GPhotoCamera {
  public:
    GPhotoCamera(::Camera *camera, const GPhotoDriverPtr &driver);
    ~GPhotoCamera();
    typedef std::function<GPhotoReturn(::GPContext*, ::Camera*)> CameraRun;
    GPhotoReturn operator() (CameraRun run);
    operator GPhotoDriverPtr() const;
    operator GPhotoWrapperPtr() const;
  private:
    DPTR
  };
};

GPhoto::GPhotoWrapperPtr operator<<(const GPhoto::GPhotoWrapperPtr &, GPhoto::GPhotoWrapper::GPhotoRun);
GPhoto::GPhotoDriverPtr operator<<(const GPhoto::GPhotoDriverPtr &, GPhoto::GPhotoDriver::ContextRun);
GPhoto::GPhotoCameraPtr operator<<(const GPhoto::GPhotoCameraPtr &, GPhoto::GPhotoCamera::CameraRun);

#define GPRET(f) return GPhotoReturn{f, __FILE__, __LINE__, __PRETTY_FUNCTION__};
#define GP2_RUN(...) [__VA_ARGS__]() -> GPhotoReturn
#define CTX_RUN(...) [__VA_ARGS__](::GPContext *gp_ctx) -> GPhotoReturn
#define CAM_RUN(...) [__VA_ARGS__](::GPContext *gp_ctx, ::Camera *gp_cam) -> GPhotoReturn

#endif
