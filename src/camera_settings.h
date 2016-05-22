#ifndef LIBGPHOTO_CPP_CAMERA_SETTINGS_H
#define LIBGPHOTO_CPP_CAMERA_SETTINGS_H
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
#include "camera.h"
#include "utils/dptr.h"
#include <vector>
namespace GPhotoCPP {
class Camera::Settings
{
public:
  ~Settings();
  bool needs_serial_port() const;
  
  std::vector<std::string> iso_choices() const;
  std::vector<std::string> format_choices() const;
  std::string iso() const;
  std::string format() const;
  
  WidgetPtr iso_widget() const;
  WidgetPtr format_widget() const;
  
  void set_iso(const std::string &iso);
  void set_format(const std::string &format);
  void set_serial_port(const std::string &port);

  ExposurePtr exposure() const;
  ShooterPtr shooter() const;
  // TODO: private again?
  Settings(const std::weak_ptr<iCamera> &camera, const LoggerPtr &logger);
private:
  DPTR
};
}

#endif // LIBGPHOTO_CPP_CAMERA_SETTINGS_H