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

#include "serialshooter.h"
#include <sstream>

using namespace GPhoto;
using namespace std;

class SerialShooter::Private {
public:
  Private(const string &device_path, SerialShooter *q);
  string device_path;
  class Shoot;
private:
  SerialShooter *q;
};

class SerialShooter::Private::Shoot : public Shooter::Shoot {
public:
  Shoot(const string &device_path);
  ~Shoot();
private:
  int bulb_fd;
};

SerialShooter::Private::Private(const string& device_path, SerialShooter* q) : device_path{device_path}, q{q}
{
}

SerialShooter::SerialShooter(const string &device_path)
 : dptr(device_path, this)
{
}

SerialShooter::~SerialShooter()
{
}

Shooter::ShootPtr SerialShooter::shoot() const
{
  return make_shared<Private::Shoot>(d->device_path);
}


void SerialShooter::set_device_path(const string& device_path)
{
  d->device_path = device_path;
}

SerialShooter::error::error(const string& message): runtime_error(message)
{
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

SerialShooter::Private::Shoot::Shoot(const string& device_path)
{
    bulb_fd = open(device_path.c_str(), O_RDWR, O_NONBLOCK);
    if(bulb_fd == -1) {
      stringstream ss;
      ss << "Error opening serial port " << device_path << ": " << strerror(bulb_fd);
      throw error(ss.str());
    }
}

SerialShooter::Private::Shoot::~Shoot()
{
  if(bulb_fd > -1)
    close(bulb_fd);
}


