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

#include "camera_control.h"
#include "camera_settings.h"
#include "exposure.h"

using namespace GPhotoCPP;
using namespace std;

class GPhotoCPP::Camera::Control::Private {
public:
  Private(const weak_ptr<GPhotoCPP::Camera> &camera, GPhotoCPP::Camera::Settings& settings, const LoggerPtr& logger);
  weak_ptr<GPhotoCPP::Camera> camera;
  Settings &settings;
  const LoggerPtr logger;
};

GPhotoCPP::Camera::Control::Private::Private(const weak_ptr< GPhotoCPP::Camera >& camera, GPhotoCPP::Camera::Settings& settings, const LoggerPtr& logger)
  : camera{camera}, settings{settings}, logger{logger}
{
}


GPhotoCPP::Camera::Control::Control(const weak_ptr<GPhotoCPP::Camera> &camera, Settings& settings, const LoggerPtr& logger) : dptr(camera, settings, logger)
{
}

GPhotoCPP::Camera::Control::~Control()
{
}


GPhotoCPP::Camera::ShotPtr GPhotoCPP::Camera::Control::shoot(const milliseconds& exposure, bool mirror_lock, seconds mirror_lock_duration)
{
  Camera::MirrorLock mirror_lock_trigger;
  if(mirror_lock && d->settings.shooter() )
    mirror_lock_trigger = Camera::MirrorLock{mirror_lock_duration, d->settings.shooter()};
  if(exposure < seconds{2} || ! d->settings.shooter() ) {
    d->settings.exposure()->set(exposure);
    d->camera.lock()->save_settings();
    return d->camera.lock()->shoot_preset(mirror_lock_trigger);
  }
  d->settings.exposure()->set_bulb();
  d->camera.lock()->save_settings();
  return d->camera.lock()->shoot_bulb(exposure, d->settings.shooter(), mirror_lock_trigger);
}
