#ifndef LIBGPHOTO_CPP_WIDGETS_WIDGET_P_H
#define LIBGPHOTO_CPP_WIDGETS_WIDGET_P_H
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

#include "widgets.h"
#include "widget_string.h"
#include "backend/gphoto_wrapper.h"
#include <gphoto2/gphoto2.h>
#include <map>
using namespace GPhotoCPP;
using namespace std;


class Widget::Private {
public:
  Private(CameraWidget* widget, const GPhotoCPP::GPhotoWrapperPtr& gphoto, const Logger::ptr &log, Widget* q);
  CameraWidget* widget;
  GPhotoWrapperPtr gphoto;
  std::string name;
  std::string label;
  int id;
  Type type;
  Logger::ptr log;
  Access access;
  
  WidgetPtr find_by(function<GPhotoReturn(::CameraWidget*&)> run, const string &field_name) const;
  
  
private:
  Widget *q;
};
#endif // LIBGPHOTO_CPP_WIDGETS_WIDGET_P_H
