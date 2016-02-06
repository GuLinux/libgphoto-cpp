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

#include "camerafile.h"
#include "gphoto_wrapper.h"
#include <gphoto2/gphoto2.h>
#include <vector>

using namespace GPhoto;
using namespace std;

DPTR_CLASS(GPhoto::CameraFile) {
public:
  Private(const string& folder, const string& file, const GPhotoCameraPtr& camera, GPhoto::CameraFile* q);
  string folder;
  string file;
  GPhotoCameraPtr camera;
  ::CameraFile *camera_file;
  CameraFileInfo info;
private:
  GPhoto::CameraFile *q;
};

GPhoto::CameraFile::Private::Private(const string &folder, const string &file, const GPhotoCameraPtr &camera, CameraFile* q)
  : folder{folder}, file{file}, camera{camera}, q{q}
{
}



GPhoto::CameraFile::CameraFile(const string &folder, const string &file, const GPhotoCameraPtr &camera)
 : dptr(folder, file, camera, this)
{
  d->camera << CAM_RUN(this) { return gp_file_new(&d->camera_file); }
	    << CAM_RUN(this,&file,&folder) { return gp_camera_file_get(gp_cam, folder.c_str(), file.c_str(), GP_FILE_TYPE_NORMAL, d->camera_file, gp_ctx) != GP_OK; }
	    << CAM_RUN(this,&file,&folder) { return gp_camera_file_get_info (gp_cam, folder.c_str(), file.c_str(), &d->info, gp_ctx); };
}

GPhoto::CameraFile::~CameraFile()
{
  d->camera << CAM_RUN(this) { return gp_file_free(d->camera_file); };
}


void GPhoto::CameraFile::save(const string& path)
{
  d->camera << CAM_RUN(this, path) { return gp_file_save(d->camera_file, path.c_str()); };
}


void GPhoto::CameraFile::copy(vector< uint8_t >& data)
{
  size_t size;
  const char *cdata;
  d->camera << CAM_RUN(this, &cdata, &size) { return gp_file_get_data_and_size(d->camera_file, &cdata, &size); };
  data.resize(size);
  std::copy(cdata, cdata + size, begin(data));
}

vector< uint8_t > GPhoto::CameraFile::data()
{
  vector<uint8_t> data;
  copy(data);
  return data;
}

GPhoto::CameraFile::Info GPhoto::CameraFile::info() const
{
  return { d->info.file.size, d->info.file.width, d->info.file.height };
}

void GPhoto::CameraFile::delete_on_camera()
{
  d->camera << CAM_RUN(this) { return gp_camera_file_delete(gp_cam, d->folder.c_str(), d->file.c_str(), gp_ctx); };
}

string GPhoto::CameraFile::path() const
{
  return d->folder + "/" + d->file;
}


ostream& operator<<(ostream& o, const GPhoto::CameraFile::Info& info)
{
  return o << "size: " << info.size << " bytes, width: " << info.width << ", height: " << info.height;
}

ostream& operator<<(ostream& o, const GPhoto::CameraFile& camera_file)
{
  return o << "CameraFile { " << camera_file.path() << ", " << camera_file.info() << " }";
}

