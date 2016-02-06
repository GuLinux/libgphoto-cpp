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

#include "camera.h"
#include "gphoto_wrapper.h"
#include "widget.h"

using namespace GPhoto;
using namespace std;

DPTR_CLASS(GPhoto::Camera) {
public:
  Private(const GPhotoCameraPtr &camera, const LoggerPtr &logger, Camera *q);
  GPhotoCameraPtr camera;
  LoggerPtr logger;
  string summary;
  shared_ptr< Widget > settings;
private:
  Camera *q;
};

GPhoto::Camera::Private::Private(const GPhotoCameraPtr& camera, const LoggerPtr& logger, GPhoto::Camera* q) : camera{camera}, logger{logger}, q{q}
{

}


GPhoto::Camera::Camera(const GPhotoCameraPtr &camera, const LoggerPtr &logger) : dptr(camera, logger, this)
{
  CameraText text;
  d->camera << CAM_RUN(this, &text) { return gp_camera_get_summary(gp_cam, &text, gp_ctx); };
  d->summary = {text.text};
  CameraWidget *w;
  d->camera << CAM_RUN(this, &w) { return gp_camera_get_config(gp_cam, &w, gp_ctx); };
  d->settings = make_shared<Widget>(w, *d->camera, d->logger);
}

GPhoto::Camera::~Camera()
{

}

WidgetPtr GPhoto::Camera::settings() const
{
  return d->settings;
}

string GPhoto::Camera::summary() const
{
  return d->summary;
}



ostream &operator<<(ostream &o, const GPhoto::Camera &c) {
  o << "GPhoto::Camera{ " << c.summary() << "}";
  return o;
}
