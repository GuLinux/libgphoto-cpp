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

using namespace GPhotoCPP;
using namespace std;

DPTR_CLASS(Driver) {
public:
  Private(const LoggerPtr &logger, Driver *q);
  GPhotoDriverPtr driver;
  LoggerPtr logger;
  class CameraFactoryImpl : public CameraFactory {
  public:
    CameraFactoryImpl(const string &name, const string &port, const GPhotoDriverPtr &driver, const LoggerPtr &logger);
    virtual string name() const;
    virtual operator CameraPtr() const;
    virtual CameraPtr camera(const LoggerPtr& logger = {}) const;
  private:
    const string m_name, m_port;
    const GPhotoDriverPtr driver;
    const LoggerPtr logger;
  };
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
    d->driver << CTX_RUN(this, log_func, libgphotoLogLevel,gphoto_levels) { GPRET(gp_log_add_func(gphoto_levels.at(libgphotoLogLevel), log_func, d->logger.get()) ) };
  }
}

Driver::~Driver()
{

}

GPhotoCPP::CameraPtr Driver::autodetect(const LoggerPtr &logger) const
{
  ::Camera *camera;
  try {
    d->driver << CTX_RUN(&){ GPRET(gp_camera_new(&camera)) } << CTX_RUN(this, &camera) { GPRET(gp_camera_init(camera, gp_ctx)) };
    return make_shared<GPhotoCPP::Camera>(make_shared<GPhotoCamera>(camera, d->driver), logger ? logger : d->logger);
  } catch(GPhotoCPP::Exception &e) {
    lWarning(d->logger) << "Unable to connect to gphoto2 camera: " << e.what();
    return {};
  }
}

list< Driver::CameraFactory::ptr > Driver::cameras()
{
  CameraList *cameras;
  int cameras_size;
  list< Driver::CameraFactory::ptr > cameras_factories;
  try {
    d->driver << CTX_RUN(&) { GPRET(gp_list_new(&cameras)) };
    d->driver << CTX_RUN(&) { GPRET(gp_camera_autodetect(cameras, gp_ctx)) };
    d->driver << CTX_RUN(&) { cameras_size = gp_list_count(cameras); GPRET(cameras_size) };
    for(int i=0; i<cameras_size; i++) {
      const char *name, *port;
      d->driver << CTX_RUN(&) { GPRET(gp_list_get_name(cameras, i, &name)) };
      d->driver << CTX_RUN(&) { GPRET(gp_list_get_value(cameras, i, &port)) };
      cameras_factories.push_back(make_shared<Private::CameraFactoryImpl>(name, port, d->driver, d->logger));
    }
  } catch(GPhotoCPP::Exception &e) {
    lWarning(d->logger) << "Unable to find gphoto cameras: " << e.what();
    return {};
  }
  return cameras_factories;
}

Driver::Private::CameraFactoryImpl::CameraFactoryImpl(const string& name, const string& port, const GPhotoDriverPtr& driver, const LoggerPtr& logger) 
  : m_name{name}, m_port{port}, driver{driver}, logger{logger}
{
}

string Driver::Private::CameraFactoryImpl::name() const
{
  return m_name;
}

CameraPtr Driver::Private::CameraFactoryImpl::camera(const LoggerPtr& logger) const
{
  // TODO: move to GPhotoCPP::Camera?
  ::Camera *camera;
  CameraAbilitiesList *abilities_list = nullptr;
  CameraAbilities abilities;
  int index;
  GPPortInfoList *portInfoList = nullptr;
  GPPortInfo portInfo;
  try {
    driver << CTX_RUN(&) { GPRET(gp_camera_new(&camera)) };
    driver << CTX_RUN(&) { GPRET(gp_abilities_list_new(&abilities_list)) };
    driver << CTX_RUN(&) { GPRET(gp_abilities_list_load(abilities_list, gp_ctx)) };
    driver << CTX_RUN(&) { index = gp_abilities_list_lookup_model(abilities_list, m_name.c_str()); GPRET(index) };
    driver << CTX_RUN(&) { GPRET(gp_abilities_list_get_abilities(abilities_list, index, &abilities)) };
    driver << CTX_RUN(&) { GPRET(gp_camera_set_abilities(camera, abilities)) };
    driver << CTX_RUN(&) { GPRET(gp_port_info_list_new(&portInfoList)) };
    driver << CTX_RUN(&) { GPRET(gp_port_info_list_load(portInfoList)) };
    driver << CTX_RUN(&) { GPRET(gp_port_info_list_count(portInfoList)) };
    driver << CTX_RUN(&) { index = gp_port_info_list_lookup_path(portInfoList, m_port.c_str()); GPRET(index) };
    driver << CTX_RUN(&) { GPRET(gp_port_info_list_get_info(portInfoList, index, &portInfo))  };
    driver << CTX_RUN(&) { GPRET(gp_camera_set_port_info(camera, portInfo)) };
    
    return make_shared<GPhotoCPP::Camera>(make_shared<GPhotoCamera>(camera, driver), logger ? logger : this->logger);
  } catch(GPhotoCPP::Exception &e) {
    lWarning(this->logger) << "Unable to connect to camera " << m_name << " on port " << m_port << ": " << e.what();
    return {};
  }
}


Driver::Private::CameraFactoryImpl::operator CameraPtr() const
{
  return camera();
}



