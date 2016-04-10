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

#include "backend/gphoto_wrapper.h"
#include "backend/exceptions.h"
#include <gphoto2/gphoto2-version.h>
#include <sstream>
using namespace GPhotoCPP;
using namespace std;
DPTR_CLASS(GPhotoWrapper) {
public:
  Version version;
};

string GPhotoReturn::describe() const
{
  stringstream s;
  s << gp_result_as_string(result);
  if(file) {
    s << " at " << string{file}.replace(0, string{SOURCES_ROOT}.size() +1, "") << ":" << line;
  }
  return s.str();
}

GPhotoReturn::operator string() const
{
  return describe();
}

GPhotoReturn::operator int() const
{
  return result;
}

#include <iostream>

GPhotoWrapper::GPhotoWrapper() : dptr()
{
  (*this)(GP2_RUN(&) {
    auto versions = gp_library_version(GP_VERSION_SHORT);
    string library_version{versions[0]};
    stringstream ss(library_version);
    char dot;
    ss >> d->version.major >> dot >> d->version.minor >> dot >> d->version.patch;
    GPRET(GP_OK);
  });
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

GPhotoReturn GPhotoWrapper::operator()(GPhotoRun run)
{
  GPhotoReturn result = run();
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

GPhotoReturn GPhotoDriver::operator()(ContextRun run)
{
  return d->wrapper->operator()(GP2_RUN(this, run) { return run(d->context); });
}

#include <mutex>

DPTR_CLASS(GPhotoCamera) {
public:
  Private(::Camera* camera, const GPhotoDriverPtr& driver);
  ::Camera* camera;
  mutex _mutex;
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

GPhotoReturn GPhotoCamera::operator()(CameraRun run)
{
  unique_lock<mutex> lock(d->_mutex);
  return d->driver->operator()(CTX_RUN(this, run) { return run(gp_ctx, d->camera); } );
}


GPhotoCPP::GPhotoWrapperPtr operator<<(const GPhotoCPP::GPhotoWrapperPtr&c, GPhotoCPP::GPhotoWrapper::GPhotoRun r)
{
  c->operator()(r);
  return c;
}


GPhotoCPP::GPhotoDriverPtr operator<<(const GPhotoCPP::GPhotoDriverPtr &c, GPhotoCPP::GPhotoDriver::ContextRun r)
{
  c->operator()(r);
  return c;
}


GPhotoCPP::GPhotoCameraPtr operator<<(const GPhotoCPP::GPhotoCameraPtr &c, GPhotoCPP::GPhotoCamera::CameraRun f)
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

bool GPhotoWrapper::Version::operator<(const GPhotoWrapper::Version& other) const
{
  if(major != other.major)
    return major < other.major;
  if(minor != other.minor)
    return minor < other.minor;
  return patch < other.patch;
}

bool GPhotoWrapper::Version::operator==(const GPhotoWrapper::Version& other) const
{
  return major == other.major && minor == other.minor && patch == other.patch;
}

GPhotoWrapper::Version GPhotoWrapper::version() const
{
  return d->version;
}

ostream& operator<<(ostream& o, const GPhotoWrapper::Version& v)
{
  return o << v.major << "." << v.minor << "." << v.patch;
}


