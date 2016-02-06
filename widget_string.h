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

#ifndef GPHOTO_WIDGET_STRING_H
#define GPHOTO_WIDGET_STRING_H
#include "widget.h"
namespace GPhoto {
class Widget::StringValue : public Widget::Value<std::string, char*, char*>{
private:
  friend class Widget;
  StringValue(Widget* widget);
};
}

#endif
