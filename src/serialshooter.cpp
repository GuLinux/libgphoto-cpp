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

#include "serialshooter.h"
#include <sstream>
#include "utils/logger.h"

using namespace GPhotoCPP;
using namespace std;

class SerialShooter::Private {
public:
  Private(const string &device_path, const LoggerPtr &logger, SerialShooter *q);
  string device_path;
  LoggerPtr logger;
  class Shoot;
private:
  SerialShooter *q;
};

class SerialShooter::Private::Shoot : public Shooter::Shoot {
public:
  Shoot(const string &device_path, const LoggerPtr &logger = {});
  ~Shoot();
private:
  string device_path;
  LoggerPtr logger;
  int bulb_fd;
};

SerialShooter::Private::Private(const string& device_path, const LoggerPtr &logger, SerialShooter* q) : device_path{device_path}, logger{logger}, q{q}
{
}

SerialShooter::SerialShooter(const string &device_path, const LoggerPtr &logger)
 : dptr(device_path, logger, this)
{
}

SerialShooter::~SerialShooter()
{
}

Shooter::ShootPtr SerialShooter::shoot() const
{
  return make_shared<Private::Shoot>(d->device_path, d->logger);
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

SerialShooter::Private::Shoot::Shoot(const string& device_path, const LoggerPtr& logger) : device_path{device_path}, logger{logger}
{
    bulb_fd = open(device_path.c_str(), O_RDWR, O_NONBLOCK);
    if(bulb_fd == -1) {
      stringstream ss;
      ss << "Error opening serial port " << device_path << ": " << strerror(bulb_fd);
      throw error(ss.str());
    }
    lDebug(logger) << "Serial port " << device_path << " opened";
}

SerialShooter::Private::Shoot::~Shoot()
{
  if(bulb_fd > -1) {
    close(bulb_fd);
    lDebug(logger) << "Serial port " << device_path << " closed";
  }
}


