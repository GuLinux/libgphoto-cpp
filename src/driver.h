#ifndef LIBGPHOTO_CPP_DRIVER_H
#define LIBGPHOTO_CPP_DRIVER_H
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


namespace GPhotoCPP {

class Driver
{
public:
  enum LibGPhotoLogLevel { None, Error, Verbose, Debug };
    Driver(const LoggerPtr &logger = {}, LibGPhotoLogLevel libgphotoLogLevel = None);
    ~Driver();
    GPhotoCPP::CameraPtr autodetect(const LoggerPtr &logger = {}) const;
    class CameraFactory {
    public:
      virtual std::string name() const = 0;
      virtual operator CameraPtr() const = 0;
      virtual CameraPtr camera(const LoggerPtr &logger = {}) const = 0;
      typedef std::shared_ptr<CameraFactory> ptr;
    };
    std::list<CameraFactory::ptr> cameras();
private:
  DPTR
};
}
#endif // LIBGPHOTO_CPP_DRIVER_H
