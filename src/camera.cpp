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

#include "camera.h"
#include "backend/gphoto_wrapper.h"
#include "widgets/widget.h"
#include "shooter.h"
#include <chrono>
#include <thread>
#include <list>
#include <algorithm>
#include "utils/logger.h"
#include "backend/exceptions.h"
#include "camerafile.h"
#include "backend/list.h"
#include "camera_filesystem.h"

using namespace GPhotoCPP;
using namespace std;
using namespace std::chrono;

DPTR_CLASS(GPhotoCPP::Camera) {
public:
  Private(const GPhotoCameraPtr &camera, const LoggerPtr &logger, Camera *q);
  CameraFilePtr wait_for_file(milliseconds timeout = seconds{120});
  void try_mirror_lock(MirrorLock mirror_lock);
  void wait_for(milliseconds how_much);
  GPhotoCameraPtr camera;
  LoggerPtr logger;
  string summary;
  shared_ptr< Widget > settings;
  list<string> downloaded_files;
private:
  Camera *q;
};


class GPhotoCPP::Camera::Shot::Private {
public:
  Private(const milliseconds &exposure);
  milliseconds exposure;
  chrono::time_point<chrono::steady_clock> started;
  chrono::time_point<chrono::steady_clock> finished;
  CameraFileFuture result;
  std::atomic_bool is_finished;
  class measure {
  public:
    measure(Private &d);
    ~measure();
  private:
    Private &d;
  };
  std::shared_ptr<measure> start();
};
GPhotoCPP::Camera::Shot::Private::Private(const milliseconds& exposure) : exposure{exposure}, is_finished{false}
{
}


shared_ptr< GPhotoCPP::Camera::Shot::Private::measure > GPhotoCPP::Camera::Shot::Private::start()
{
  return make_shared<measure>(*this);
}

GPhotoCPP::Camera::Shot::Private::measure::measure(GPhotoCPP::Camera::Shot::Private& d) : d{d}
{
  d.started = steady_clock::now();
}

GPhotoCPP::Camera::Shot::Private::measure::~measure()
{
  d.finished = steady_clock::now();
  d.is_finished = true;
}


GPhotoCPP::Camera::Shot::Shot(const milliseconds &exposure) : dptr(exposure)
{
}

GPhotoCPP::milliseconds GPhotoCPP::Camera::Shot::elapsed() const
{
  if(!d->is_finished)
    return steady_clock::now() - d->started;
  return d->finished - d->started;
}

CameraFileFuture &GPhotoCPP::Camera::Shot::camera_file() const
{
  return d->result;
}
CameraFileFuture& GPhotoCPP::Camera::Shot::operator*() const
{
  return camera_file();
}

GPhotoCPP::milliseconds GPhotoCPP::Camera::Shot::duration() const
{
  return d->exposure;
}




GPhotoCPP::Camera::Private::Private(const GPhotoCameraPtr& camera, const LoggerPtr& logger, GPhotoCPP::Camera* q) : camera{camera}, logger{logger}, q{q}
{

}


GPhotoCPP::Camera::Camera(const GPhotoCameraPtr &camera, const LoggerPtr &logger) : dptr(camera, logger, this)
{
  CameraText text;
  d->camera << CAM_RUN(&) { GPRET(gp_camera_get_summary(gp_cam, &text, gp_ctx)) };
  d->summary = {text.text};
  CameraWidget *w;
  d->camera << CAM_RUN(&) { GPRET(gp_camera_get_config(gp_cam, &w, gp_ctx)) };
  d->settings = make_shared<Widget>(w, *d->camera, d->logger);
}

GPhotoCPP::Camera::~Camera()
{

}

WidgetPtr GPhotoCPP::Camera::settings() const
{
  return d->settings;
}

string GPhotoCPP::Camera::summary() const
{
  return d->summary;
}


GPhotoCPP::Camera::ShotPtr GPhotoCPP::Camera::shoot_bulb(const milliseconds& exposure, const ShooterPtr& shooter, const GPhotoCPP::Camera::MirrorLock& mirror_lock) const
{
  shared_ptr<Shot> shot{new Shot{exposure}};
  shot->d->result = async([=]{
    {
      lDebug(d->logger) << "Shooting with bulb mode, duration: " << exposure.count();
      d->try_mirror_lock(mirror_lock);
      auto started = shot->d->start();
      auto shoot = shooter->shoot();
      d->wait_for(exposure);
    }
    return d->wait_for_file();
  });
  return shot;
}



GPhotoCPP::Camera::ShotPtr GPhotoCPP::Camera::shoot_preset(const GPhotoCPP::Camera::MirrorLock& mirror_lock) const
{
  shared_ptr<Shot> shot{new Shot{{}}};
  shot->d->result = async([=] {
    lDebug(d->logger) << "Shooting with preset mode...";
    d->try_mirror_lock(mirror_lock);
    CameraFilePath camera_file_path;
    lDebug(d->logger) << "Shooter thread";
    if(mirror_lock) {
      mirror_lock.shooter->shoot();
      auto start = shot->d->start();
      return d->wait_for_file();
    } else {
      auto start = shot->d->start();
      d->camera << CAM_RUN(&) { GPRET(gp_camera_capture(gp_cam, GP_CAPTURE_IMAGE, &camera_file_path, gp_ctx)) };
      return make_shared<GPhotoCPP::CameraFile>(camera_file_path.folder, camera_file_path.name, d->camera, d->logger);
    }
  });
  return shot;
}

CameraFilePtr GPhotoCPP::Camera::Private::wait_for_file(milliseconds timeout)
{
  lDebug(logger) << "Waiting for file with timeout: " << timeout.count() << " milliseconds";
  auto start = chrono::steady_clock::now();
  while(chrono::steady_clock::now() - start < timeout) {
    CameraEventType event_type;
    void *event_data = nullptr;
    CameraFilePath *camera_file;
    camera << CAM_RUN(&) { GPRET(gp_camera_wait_for_event(gp_cam, 100, &event_type, &event_data, gp_ctx)) };
    switch(event_type) {
      case GP_EVENT_TIMEOUT:
	break;
// 	throw TimeoutError("Timeout waiting for file capture");
      case GP_EVENT_UNKNOWN:
	lDebug(logger) << "Unknown event received: " << event_data  << static_cast<const char*>(event_data);
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
	return make_shared<GPhotoCPP::CameraFile>(camera_file->folder, camera_file->name, camera, logger);
    }
  }
  throw TimeoutError("Timeout waiting for file capture");
}

void GPhotoCPP::Camera::Private::wait_for(milliseconds how_much)
{
    auto start = steady_clock::now();
    while( milliseconds{steady_clock::now() - start} < how_much) {
      this_thread::sleep_for(nanoseconds(1));
    }
}


void GPhotoCPP::Camera::Private::try_mirror_lock(GPhotoCPP::Camera::MirrorLock mirror_lock)
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


void GPhotoCPP::Camera::save_settings()
{
  d->camera << CAM_RUN(this) { GPRET(gp_camera_set_config(gp_cam, *d->settings, gp_ctx)) };
}

GPhotoCPP::Camera::MirrorLock::MirrorLock(const milliseconds& duration, const ShooterPtr shooter)
  : duration{duration}, shooter{shooter}
{

}


GPhotoCPP::Camera::MirrorLock::operator bool() const
{
  return duration.count() > 0 && shooter;
}



ostream &operator<<(ostream &o, const GPhotoCPP::Camera &c) {
  o << "GPhotoCPP::Camera{ " << c.summary() << "}";
  return o;
}

list<string> GPhotoCPP::Camera::folders(const string& folder) const
{
  List files_list{*d->camera};
  d->camera << CAM_RUN(&) { GPRET(gp_camera_folder_list_folders(gp_cam, folder.c_str(), files_list, gp_ctx)) };
  multimap<string,string> files_map = files_list;
  list<string> folders(files_map.size());;
  transform(begin(files_map), end(files_map), begin(folders), [&folder](const pair<string,string> &p){ return folder + "/" + p.first; });
  return folders;
}

list<GPhotoCPP::Camera::FileInfo> GPhotoCPP::Camera::files(const string& folder) const
{
  List files_list{*d->camera};
  d->camera << CAM_RUN(&) { GPRET(gp_camera_folder_list_files(gp_cam, folder.c_str(), files_list, gp_ctx)) };
  multimap<string,string> files_map = files_list;
  list<FileInfo> files(files_map.size());;
  transform(begin(files_map), end(files_map), begin(files), [&folder](const pair<string,string> &p){ return FileInfo{folder, p.first}; });
  return files;
}

string GPhotoCPP::Camera::FileInfo::path() const
{
  return folder + "/" + name;
}


CameraFolderPtr GPhotoCPP::Camera::root(const string& root_path)
{
  return make_shared<CameraFolder>(root_path, d->camera, d->logger);
}
