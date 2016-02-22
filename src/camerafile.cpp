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

#include "utils/logger.h"
#include "camerafile.h"
#include "backend/gphoto_wrapper.h"
#include "backend/exceptions.h"
#include <gphoto2/gphoto2.h>
#include <vector>

using namespace GPhotoCPP;
using namespace std;

DPTR_CLASS(GPhotoCPP::CameraFile) {
public:
  Private(const string& folder, const string& file, const GPhotoCameraPtr& camera, const LoggerPtr &logger, GPhotoCPP::CameraFile* q);
  string folder;
  string file;
  GPhotoCameraPtr camera;
  ::CameraFile *camera_file;
  const LoggerPtr logger;
private:
  GPhotoCPP::CameraFile *q;
};

GPhotoCPP::CameraFile::Private::Private(const string &folder, const string &file, const GPhotoCameraPtr &camera, const LoggerPtr &logger, CameraFile* q)
  : folder{folder}, file{file}, camera{camera}, logger{logger}, q{q}
{
}



GPhotoCPP::CameraFile::CameraFile(const string &folder, const string &file, const GPhotoCameraPtr &camera, const LoggerPtr &logger)
 : dptr(folder, file, camera, logger, this)
{
  d->camera << CAM_RUN(this) { GPRET(gp_file_new(&d->camera_file)) }
	    << CAM_RUN(this,&file,&folder) { GPRET(gp_camera_file_get(gp_cam, folder.c_str(), file.c_str(), GP_FILE_TYPE_NORMAL, d->camera_file, gp_ctx) != GP_OK) };
}

GPhotoCPP::CameraFile::~CameraFile()
{
  d->camera << CAM_RUN(this) { GPRET(gp_file_free(d->camera_file)) };
}


void GPhotoCPP::CameraFile::save(const string& path)
{
  d->camera << CAM_RUN(this, path) { GPRET(gp_file_save(d->camera_file, path.c_str())) };
}


void GPhotoCPP::CameraFile::copy(vector< uint8_t >& data)
{
  size_t size;
  const char *cdata;
  d->camera << CAM_RUN(this, &cdata, &size) { GPRET(gp_file_get_data_and_size(d->camera_file, &cdata, &size)) };
  data.resize(size);
  std::copy(cdata, cdata + size, begin(data));
}

vector< uint8_t > GPhotoCPP::CameraFile::data()
{
  vector<uint8_t> data;
  copy(data);
  return data;
}

GPhotoCPP::CameraFile::Info GPhotoCPP::CameraFile::info() const
{
  ::CameraFileInfo info;
  try {
    d->camera << CAM_RUN(this,&info) { GPRET(gp_camera_file_get_info (gp_cam, d->folder.c_str(), d->file.c_str(), &info, gp_ctx)) };
  } catch(GPhotoCPP::Exception &e) {
    lWarning(d->logger) << "Unable to retrieve information for file " << d->folder << "/" << d->file << ": " << e.what();
    return {};
  }
  return { info.file.size, info.file.width, info.file.height };
}

void GPhotoCPP::CameraFile::delete_on_camera()
{
  d->camera << CAM_RUN(this) { GPRET(gp_camera_file_delete(gp_cam, d->folder.c_str(), d->file.c_str(), gp_ctx)) };
}

string GPhotoCPP::CameraFile::path() const
{
  return d->folder + "/" + d->file;
}

string GPhotoCPP::CameraFile::file() const
{
  return d->file;
}
string GPhotoCPP::CameraFile::folder() const
{
  return d->folder;
}


ostream& operator<<(ostream& o, const GPhotoCPP::CameraFile::Info& info)
{
  return o << "size: " << info.size << " bytes, width: " << info.width << ", height: " << info.height;
}

ostream& operator<<(ostream& o, const GPhotoCPP::CameraFile& camera_file)
{
  return o << "CameraFile { " << camera_file.path() << ", " << camera_file.info() << " }";
}

