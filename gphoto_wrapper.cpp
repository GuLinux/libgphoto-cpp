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

#include "gphoto_wrapper.h"
using namespace GPhoto;
using namespace std;
DPTR_CLASS(GPhotoWrapper) {
};

GPhotoWrapper::GPhotoWrapper() : dptr()
{

}

GPhotoWrapper::~GPhotoWrapper()
{

}


DPTR_CLASS(GPhotoDriver) {
public:
  Private(GPContext *context);
  std::shared_ptr<GPhotoWrapper> wrapper;
  GPContext *context;
  bool to_delete = true; // TODO: remove after migration complete
};

GPhotoDriver::Private::Private(GPContext* context) : wrapper{new GPhotoWrapper()}, context{context}
{
}

Exception::Exception(int error_code): runtime_error(gp_result_as_string(error_code))
{
}

Exception::Exception(const std::string& __arg): runtime_error(__arg)
{
}

int GPhotoWrapper::operator()(GPhotoRun run)
{
  int result = run();
  if(result < GP_OK)
    throw Exception{result};
  return result;
}


GPhotoDriver::GPhotoDriver() : GPhotoDriver(gp_context_new())
{
}

GPhotoDriver::GPhotoDriver(GPContext* context) : dptr(context)
{
  d->to_delete = false; // TODO: remove after migration complete
}

GPhotoDriver::~GPhotoDriver()
{
  if(d->to_delete)
    gp_context_unref(d->context);
}

int GPhotoDriver::operator()(ContextRun run)
{
  return d->wrapper->operator()(GP2_RUN(this, run) { return run(d->context); });
}


DPTR_CLASS(GPhotoCamera) {
public:
  Private(::Camera* camera, const GPhotoDriverPtr& driver);
  ::Camera* camera;
  GPhotoDriverPtr driver;
};

GPhotoCamera::Private::Private(::Camera* camera, const GPhotoDriverPtr& driver) : camera{camera}, driver{driver}
{
}


GPhotoCamera::GPhotoCamera(::Camera* camera, const GPhotoDriverPtr& driver) : dptr(camera, driver)
{
}

GPhotoCamera::~GPhotoCamera()
{
}

int GPhotoCamera::operator()(CameraRun run)
{
  return d->driver->operator()(CTX_RUN(this, run) { return run(gp_ctx, d->camera); } );
}


GPhoto::GPhotoWrapperPtr operator<<(const GPhoto::GPhotoWrapperPtr&c, GPhoto::GPhotoWrapper::GPhotoRun r)
{
  c->operator()(r);
  return c;
}


GPhoto::GPhotoDriverPtr operator<<(const GPhoto::GPhotoDriverPtr &c, GPhoto::GPhotoDriver::ContextRun r)
{
  c->operator()(r);
  return c;
}


GPhoto::GPhotoCameraPtr operator<<(const GPhoto::GPhotoCameraPtr &c, GPhoto::GPhotoCamera::CameraRun f)
{
  c->operator()(f);
  return c;
}

GPhotoCamera::operator GPhotoDriverPtr() const
{
  return d->driver;
}

GPhotoCamera::operator GPhotoWrapperPtr() const
{
  return *d->driver;
}

GPhotoDriver::operator GPhotoWrapperPtr() const
{
  return d->wrapper;
}


