#ifndef LIBGPHOTO_CPP_EXPOSURE_H
#define LIBGPHOTO_CPP_EXPOSURE_H
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
 * 
 */

#include "fwd.h"
#include <chrono>
#include <ratio>
#include <list>
namespace GPhotoCPP {

class Exposure
{
public:
  struct Value {
    std::string text;
    milliseconds duration() const;
    bool bulb() const;
  };
  typedef std::list< Exposure::Value > Values;
  Exposure(const WidgetPtr &widget);
  Values values() const;
  Value value() const;
  void set_bulb();
  void set(const milliseconds &duration, double tolerance = 0.25);
  void set(const Value &value);
  ~Exposure();

private:
    DPTR
};
}
#endif // LIBGPHOTO_CPP_EXPOSURE_H
