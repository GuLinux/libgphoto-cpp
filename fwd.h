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

#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H
#include <list>
#include "dptr.h"
extern "C" {
typedef struct _GPContext GPContext;
typedef struct _Camera Camera;
typedef struct _CameraWidget CameraWidget;
}

#define fwd_class(name) class name; typedef std::shared_ptr<name> name##Ptr;
namespace GPhoto {
  fwd_class(GPhotoWrapper)
  fwd_class(GPhotoDriver)
  fwd_class(GPhotoCamera)
  fwd_class(CameraFile)
  fwd_class(Widget)
  fwd_class(Logger)
  fwd_class(Driver)
  fwd_class(Camera)
  typedef std::list<WidgetPtr> Widgets;
};

#endif