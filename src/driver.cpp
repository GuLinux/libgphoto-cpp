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

#include "driver.h"
#include "backend/gphoto_wrapper.h"
#include "utils/logger.h"
#include "backend/exceptions.h"
#include "camera.h"
#include <map>

using namespace GPhoto;
using namespace std;

DPTR_CLASS(Driver) {
public:
  Private(const LoggerPtr &logger, Driver *q);
  GPhotoDriverPtr driver;
  LoggerPtr logger;
private:
  Driver *q;
};

Driver::Private::Private(const LoggerPtr &logger, Driver* q) : driver{make_shared<GPhotoDriver>()}, logger{logger}, q{q}
{

}


Driver::Driver(const LoggerPtr &logger, LibGPhotoLogLevel libgphotoLogLevel) : dptr(logger, this)
{
//  GPLogLevel level, const char *domain, const char *str, void *data)
  auto log_func = [](GPLogLevel level, const char *domain, const char *str, void *loggerp){
    Logger *logger = reinterpret_cast<Logger*>(loggerp);
    switch(level) {
      case GP_LOG_ERROR:
	lError(logger) << "gphoto2: " << domain << "-" << str;
	break;
      case GP_LOG_VERBOSE:
	lDebug(logger) << "gphoto2: " << domain << "-" << str;
	break;
      case GP_LOG_DEBUG:
	lTrace(logger) << "gphoto2: " << domain << "-" << str;
	break;
      default:
	return;
    }
  };
  map<LibGPhotoLogLevel, GPLogLevel> gphoto_levels {
    {Error, GP_LOG_ERROR}, {Verbose, GP_LOG_VERBOSE}, {Debug, GP_LOG_DEBUG}
  };
  if(libgphotoLogLevel != None) {
    d->driver << CTX_RUN(this, log_func, libgphotoLogLevel,gphoto_levels) {
      return gp_log_add_func(gphoto_levels.at(libgphotoLogLevel), log_func, d->logger.get());
    };
  }
}

Driver::~Driver()
{

}

GPhoto::CameraPtr Driver::autodetect() const
{
  ::Camera *camera;
  try {
    d->driver << CTX_RUN(this, &camera){ GPRET(gp_camera_new(&camera)) } << CTX_RUN(this, &camera) { GPRET(gp_camera_init(camera, gp_ctx)) };
    return make_shared<GPhoto::Camera>(make_shared<GPhotoCamera>(camera, d->driver), d->logger);
  } catch(GPhoto::Exception &e) {
    lWarning(d->logger) << "Unable to connect to gphoto2 camera: " << e.what();
    return {};
  }
}
