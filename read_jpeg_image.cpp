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

#include "read_jpeg_image.h"
#include <CImg.h>
using namespace GPhoto;
using namespace cimg_library;
using namespace std;

ReadJPEGImage::ReadJPEGImage()
{

}

ReadJPEGImage::~ReadJPEGImage()
{

}

GPhoto::ReadImage::Image ReadJPEGImage::read(const std::string& filename)
{
  CImg<unsigned char> cimg_image(filename.c_str());
  auto axis = cimg_image.spectrum() == 3 ? 3 : 2;
  ReadImage::Image image{axis, cimg_image.width(), cimg_image.height(), 8, vector<uint8_t>(cimg_image.width()*cimg_image.height()*cimg_image.spectrum())};
  copy(begin(cimg_image), end(cimg_image), begin(image.data));
  return image;
}
