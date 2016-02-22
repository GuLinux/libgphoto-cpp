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

#ifndef GPHOTO_SERIALSHOOTER_H
#define GPHOTO_SERIALSHOOTER_H

#include "shooter.h"
#include "utils/dptr.h"

namespace GPhotoCPP {
class SerialShooter : public Shooter {
public:
  class error : public std::runtime_error {
  public:
    error(const std::string& message);
  };
  SerialShooter(const std::string &device_path, const LoggerPtr &logger = {});
  ~SerialShooter();
  virtual ShootPtr shoot() const;
  void set_device_path(const std::string &device_path);
private:
    DPTR
};
}

#endif // GPHOTO_SERIALSHOOTER_H
