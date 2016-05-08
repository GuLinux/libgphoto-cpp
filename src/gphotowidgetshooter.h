#ifndef LIBGPHOTO_CPP_GPHOTOWIDGETSHOOTER_H
#define LIBGPHOTO_CPP_GPHOTOWIDGETSHOOTER_H
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
#include "utils/dptr.h"
#include "shooter.h"
#include "utils/logger.h"
#include "widgets/widgets.h"
#include "camera.h"

namespace GPhotoCPP {
template<typename WidgetType, typename ValueType>
class GPhotoWidgetShooter : public GPhotoCPP::Shooter
{
public:
    class Shoot : public GPhotoCPP::Shooter::Shoot {
    public:
      Shoot(const WidgetPtr &widget, const iCamera::ptr &camera, const ValueType &on_value, const ValueType off_value, const LoggerPtr &logger) 
      : widget{widget}, camera{camera}, off_value{off_value}, logger{logger} {
	widget->get<WidgetType>()->set(on_value);
	camera->save_settings();
	lDebug(logger) << "widget " << widget->name() << " set to " << on_value;
      }
    ~Shoot() {}
    private:
      WidgetPtr widget;
      iCamera::ptr camera;
      const ValueType off_value;
      LoggerPtr logger;
    };
    GPhotoWidgetShooter(const iCamera::ptr &camera, const std::string &widget_name, const ValueType &on_value, const ValueType &off_value, const LoggerPtr &logger = {})
      : camera{camera}, widget_name{widget_name}, on_value{on_value}, off_value{off_value}, logger{logger} {
      }
    virtual GPhotoCPP::Shooter::ShootPtr shoot() const {
      WidgetPtr widget = camera->widgets_settings()->child_by_name(widget_name);
      return std::make_shared<GPhotoWidgetShooter::Shoot>(widget, camera, on_value, off_value, logger);
    }
private:
  iCamera::ptr camera;
  std::string widget_name;
  const ValueType on_value;
  const ValueType off_value;
  LoggerPtr logger;
};
 
class EOSRemoteReleaseShutter : public GPhotoWidgetShooter<Widget::MenuValue, int> {
public:
EOSRemoteReleaseShutter(const iCamera::ptr& camera, const LoggerPtr& logger = {})
  : GPhotoWidgetShooter< GPhotoCPP::Widget::MenuValue, int>(camera, "eosremoterelease", 2, 4) {}
};
 
class BulbSettingShutter : public GPhotoWidgetShooter<Widget::ToggleValue, Widget::ToggleValue::Type> {
public:
BulbSettingShutter(const iCamera::ptr& camera, const LoggerPtr& logger = {})
  : GPhotoWidgetShooter< GPhotoCPP::Widget::ToggleValue, Widget::ToggleValue::Type>(camera, "bulb", true, false) {}
};
}
#endif // LIBGPHOTO_CPP_GPHOTOWIDGETSHOOTER_H
