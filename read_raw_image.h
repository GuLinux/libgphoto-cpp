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


#ifndef GPHOTO_READRAWIMAGE_H
#define GPHOTO_READRAWIMAGE_H

#include "read_image.h"
#include "dptr.h"

namespace GPhoto {

class ReadRawImage : public GPhoto::ReadImage
{
public:
    class error : public std::runtime_error {
    public:
        explicit error(const char*);
    };
    ReadRawImage();
    ~ReadRawImage();
    virtual GPhoto::ReadImage::Image read(const std::string& filename);
private:
  DPTR
};
}

#endif // GPHOTO_READRAWIMAGE_H
