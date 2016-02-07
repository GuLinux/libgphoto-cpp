/*
 * libgphoto++ - modern c++ wrapper library for gphoto2
 * Copyright (C) 2016 Marco Gulino <marco AT gulinux.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "eosremotereleaseshooter.h"
#include "camera.h"
#include "widget.h"
#include "widget_range.h"
using namespace GPhoto;
using namespace std;

DPTR_CLASS(EOSRemoteReleaseShooter) {
public:
  Private(const GPhoto::CameraPtr &camera, EOSRemoteReleaseShooter *q);
  GPhoto::CameraPtr camera;
  class Shoot;
private:
  EOSRemoteReleaseShooter *q;
};

class EOSRemoteReleaseShooter::Private::Shoot : public Shooter::Shoot {
public:
  Shoot(const WidgetPtr &eosremoterelease, const GPhoto::CameraPtr &camera);
  ~Shoot();
private:
  WidgetPtr eosremoterelease;
  GPhoto::CameraPtr camera;
};

EOSRemoteReleaseShooter::Private::Shoot::Shoot(const WidgetPtr& eosremoterelease, const GPhoto::CameraPtr& camera) : eosremoterelease{eosremoterelease}, camera{camera}
{
  eosremoterelease->get<Widget::RangeValue>()->set(2);
  camera->save_settings();
}

EOSRemoteReleaseShooter::Private::Shoot::~Shoot()
{
  eosremoterelease->get<Widget::RangeValue>()->set(4);
  camera->save_settings();
}



EOSRemoteReleaseShooter::Private::Private(const GPhoto::CameraPtr& camera, EOSRemoteReleaseShooter* q)
{

}


EOSRemoteReleaseShooter::EOSRemoteReleaseShooter(const GPhoto::CameraPtr &camera) : dptr(camera, this)
{

}

EOSRemoteReleaseShooter::~EOSRemoteReleaseShooter()
{

}

GPhoto::Shooter::ShootPtr EOSRemoteReleaseShooter::shoot() const
{
  return make_shared<Private::Shoot>(d->camera->settings()->child_by_name("eosremoterelease"), d->camera);
}
