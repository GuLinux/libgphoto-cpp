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

#ifndef GPHOTO_SERIALSHOOTER_H
#define GPHOTO_SERIALSHOOTER_H

#include "shooter.h"
#include "dptr.h"

namespace GPhoto {
class SerialShooter : public Shooter {
public:
  class error : public std::runtime_error {
  public:
    error(const std::string& message);
  };
  SerialShooter(const std::string &device_path);
  ~SerialShooter();
  virtual ShootPtr shoot() const;
  void set_device_path(const std::string &device_path);
private:
    DPTR
};
}

#endif // GPHOTO_SERIALSHOOTER_H
