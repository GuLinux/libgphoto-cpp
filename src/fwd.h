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

#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H
#include <list>
#include <chrono>
#include <ratio>
#include "utils/dptr.h"
extern "C" {
typedef struct _GPContext GPContext;
typedef struct _Camera Camera;
typedef struct _CameraWidget CameraWidget;
typedef struct _CameraList CameraList;
}

#define fwd_class(name) class name; typedef std::shared_ptr<name> name##Ptr;
namespace GPhoto {
  typedef std::chrono::duration<double> seconds;
  typedef std::chrono::duration<double, std::milli> milliseconds;
  fwd_class(GPhotoWrapper)
  fwd_class(GPhotoDriver)
  fwd_class(GPhotoCamera)
  fwd_class(CameraFile)
  fwd_class(CameraFileInfo)
  fwd_class(Widget)
  fwd_class(Logger)
  fwd_class(Driver)
  fwd_class(Camera)
  fwd_class(CameraFolder)
  fwd_class(Shooter)
  fwd_class(Exposure)
  typedef std::list<WidgetPtr> Widgets;
};

#endif