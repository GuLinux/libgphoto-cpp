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

#include "read_jpeg_image.h"
 #include <cstdio>
 #include <jpeglib.h>
 #include <jerror.h>
#define cimg_plugin "CImg/plugins/jpeg_buffer.h"

//#include <CImg/plugins/jpeg_buffer.h>
#include <CImg.h>
#include <fstream>
using namespace GPhotoCPP;
using namespace cimg_library;
using namespace std;


DPTR_CLASS(ReadJPEGImage) {
public:
  typedef function<void(CImg<uint8_t> &)> LoadImage;
  ReadImage::Image read_from(LoadImage loader, const std::string &filename);
};

ReadJPEGImage::ReadJPEGImage() : dptr()
{
}

ReadJPEGImage::~ReadJPEGImage()
{
}

ReadImage::Image ReadJPEGImage::read(const vector< uint8_t >& data, const string& filename)
{
  return d->read_from([&](CImg<uint8_t> &img){
    img.load_jpeg_buffer(data.data(), data.size());
  }, filename);
}


ReadImage::Image ReadJPEGImage::read(const string& file_path)
{
  return d->read_from([&](CImg<uint8_t> &img) { img.load_jpeg(file_path.c_str()); }, file_path);
}

ReadImage::Image ReadJPEGImage::Private::read_from(ReadJPEGImage::Private::LoadImage loader, const string& filename)
{
  CImg<uint8_t> cimg_image;
  loader(cimg_image);
  auto axis = cimg_image.spectrum() == 3 ? 3 : 2;
  Image image{axis, cimg_image.width(), cimg_image.height(), 8, vector<uint8_t>(cimg_image.width()*cimg_image.height()*cimg_image.spectrum()), filename};
  copy(begin(cimg_image), end(cimg_image), begin(image.data));
  return image;

}
