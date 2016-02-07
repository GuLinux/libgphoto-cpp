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

#ifndef GPHOTO_EOSREMOTERELEASESHOOTER_H
#define GPHOTO_EOSREMOTERELEASESHOOTER_H

#include "fwd.h"
#include "dptr.h"
#include "shooter.h"

namespace GPhoto {

class EOSRemoteReleaseShooter : public GPhoto::Shooter
{
public:
    EOSRemoteReleaseShooter(const GPhoto::CameraPtr &camera, const LoggerPtr &logger = {});
    ~EOSRemoteReleaseShooter();
    virtual GPhoto::Shooter::ShootPtr shoot() const;
private:
  DPTR
};
}

#endif // GPHOTO_EOSREMOTERELEASESHOOTER_H
