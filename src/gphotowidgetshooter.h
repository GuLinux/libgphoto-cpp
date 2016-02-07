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

#ifndef GPHOTO_EOSREMOTERELEASESHOOTER_H
#define GPHOTO_EOSREMOTERELEASESHOOTER_H

#include "fwd.h"
#include "dptr.h"
#include "shooter.h"
#include "logger.h"
#include "widgets.h"
#include "camera.h"

namespace GPhoto {
template<typename WidgetType>
class GPhotoWidgetShooter : public GPhoto::Shooter
{
public:
    typedef typename WidgetType::Type ValueType;
    class Shoot : public GPhoto::Shooter::Shoot {
    public:
      Shoot(const WidgetPtr &widget, const GPhoto::CameraPtr &camera, const ValueType &on_value, const ValueType off_value, const LoggerPtr &logger) 
      : widget{widget}, camera{camera}, off_value{off_value}, logger{logger} {
	widget->get<WidgetType>()->set(on_value);
	camera->save_settings();
	lDebug(logger) << "widget " << widget->name() << " set to " << on_value;
      }
    ~Shoot() {}
    private:
      WidgetPtr widget;
      CameraPtr camera;
      const ValueType off_value;
      LoggerPtr logger;
    };
    GPhotoWidgetShooter(const CameraPtr &camera, const std::string &widget_name, const ValueType &on_value, const ValueType &off_value, const LoggerPtr &logger = {})
      : camera{camera}, widget_name{widget_name}, on_value{on_value}, off_value{off_value}, logger{logger} {
      }
    virtual GPhoto::Shooter::ShootPtr shoot() const {
      WidgetPtr widget = camera->settings()->child_by_name(widget_name);
      return std::make_shared<GPhotoWidgetShooter::Shoot>(widget, camera, on_value, off_value, logger);
    }
private:
  CameraPtr camera;
  std::string widget_name;
  const ValueType on_value;
  const ValueType off_value;
  LoggerPtr logger;
};
 
class EOSRemoteReleaseShutter : public GPhotoWidgetShooter<Widget::RangeValue> {
public:
EOSRemoteReleaseShutter(const CameraPtr& camera, const LoggerPtr& logger = {})
  : GPhotoWidgetShooter< GPhoto::Widget::RangeValue >(camera, "eosremoterelease", 2, 4) {}
};
 
class BulbSettingShutter : public GPhotoWidgetShooter<Widget::ToggleValue> {
public:
BulbSettingShutter(const CameraPtr& camera, const LoggerPtr& logger = {})
  : GPhotoWidgetShooter< GPhoto::Widget::ToggleValue >(camera, "bulb", true, false) {}
};
}

#endif // GPHOTO_EOSREMOTERELEASESHOOTER_H
