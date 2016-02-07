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

#include "camera.h"
#include "gphoto_wrapper.h"
#include "widget.h"
#include "shooter.h"
#include <chrono>
#include <thread>
#include "logger.h"
#include "exceptions.h"
#include "camerafile.h"

using namespace GPhoto;
using namespace std;
using namespace std::chrono;

DPTR_CLASS(GPhoto::Camera) {
public:
  Private(const GPhotoCameraPtr &camera, const LoggerPtr &logger, Camera *q);
  CameraFilePtr wait_for_file(int timeout = 30000);
  void try_mirror_lock(MirrorLock mirror_lock);
  void wait_for(duration<double, milli> how_much);
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


future< CameraFilePtr > GPhoto::Camera::shoot_bulb(const duration< double, milli >& exposure, const ShooterPtr& shooter, const GPhoto::Camera::MirrorLock& mirror_lock) const
{
  return async([=]{
    {
      lDebug(d->logger) << "Shooting with bulb mode, duration: " << exposure.count();
      d->try_mirror_lock(mirror_lock);
      auto shoot = shooter->shoot();
      d->wait_for(exposure);
    }
    return d->wait_for_file();
  });
}

future< CameraFilePtr > GPhoto::Camera::shoot_preset(const MirrorLock &mirror_lock) const
{
  return async([=] {
    lDebug(d->logger) << "Shooting with preset mode...";
    d->try_mirror_lock(mirror_lock);
    CameraFilePath camera_file_path;
    lDebug(d->logger) << "Shooter thread";
    if(mirror_lock) {
      mirror_lock.shooter->shoot();
      return d->wait_for_file();
    } else {
      d->camera << CAM_RUN(this, &camera_file_path) { return gp_camera_capture(gp_cam, GP_CAPTURE_IMAGE, &camera_file_path, gp_ctx); };
      return make_shared<GPhoto::CameraFile>(camera_file_path.folder, camera_file_path.name, d->camera);
    }
  });
}

CameraFilePtr GPhoto::Camera::Private::wait_for_file(int timeout)
{
  CameraEventType event_type;
  void *event_data;
  CameraFilePath *camera_file;
  lDebug(logger) << "Waiting for file with timeout: " << timeout;
  while(true) {
    camera << CAM_RUN(this, &timeout, &event_type, &event_data) { return gp_camera_wait_for_event(gp_cam, timeout, &event_type, &event_data, gp_ctx); };
    switch(event_type) {
      case GP_EVENT_TIMEOUT:
	throw TimeoutError("Timeout waiting for file capture");
      case GP_EVENT_UNKNOWN:
	lDebug(logger) << "Unknown event received.";
	break;
      case GP_EVENT_CAPTURE_COMPLETE:
	lDebug(logger) << "Capture complete";
	break;
      case GP_EVENT_FOLDER_ADDED:
	camera_file = reinterpret_cast<CameraFilePath*>(event_data);
	lDebug(logger) << "Wait for file: folder <" << camera_file->folder << "/" << camera_file->name << "> returned instead. Error?";
	break;
      case GP_EVENT_FILE_ADDED:
	camera_file = reinterpret_cast<CameraFilePath*>(event_data);
	lDebug(logger) << "Wait for file: file <" << camera_file->folder << "/" << camera_file->name << "> added";
	return make_shared<GPhoto::CameraFile>(camera_file->folder, camera_file->name, camera);
    }
  }
}

void GPhoto::Camera::Private::wait_for(duration< double, milli > how_much)
{
    auto start = steady_clock::now();
    while( duration<double, milli>{steady_clock::now() - start} < how_much) {
      this_thread::sleep_for(nanoseconds(1));
    }
}


void GPhoto::Camera::Private::try_mirror_lock(GPhoto::Camera::MirrorLock mirror_lock)
{
  if(! mirror_lock) {
    lDebug(logger) << "No mirror lock set, skipping";
    return;
  }
  lDebug(logger) << "Starting mirror lock: " << mirror_lock.duration.count() << "ms";
  {
    auto shoot = mirror_lock.shooter->shoot();
    this_thread::sleep_for(milliseconds{100});
  }
  wait_for(mirror_lock.duration);
  lDebug(logger) << "Ending mirror lock";
}


void GPhoto::Camera::save_settings()
{
  d->camera << CAM_RUN(this) { return gp_camera_set_config(gp_cam, *d->settings, gp_ctx); };
}

GPhoto::Camera::MirrorLock::MirrorLock(const chrono::duration< double, milli >& duration, const ShooterPtr shooter)
  : duration{duration}, shooter{shooter}
{

}


GPhoto::Camera::MirrorLock::operator bool() const
{
  return duration.count() > 0 && shooter;
}



ostream &operator<<(ostream &o, const GPhoto::Camera &c) {
  o << "GPhoto::Camera{ " << c.summary() << "}";
  return o;
}
