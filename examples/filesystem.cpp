/*
 * Copyright (C) 2016  Marco Gulino <marco@gulinux.net>
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
 *
 */
#include "commons.h"
using namespace std;
using namespace GPhoto;

int main(int argc, char **argv) {
  init_options(argc, argv);
//   if(has_option("-h") || has_option("--help")) {
//     cout << args[0] << " usage: " << endl
// 	 << "\t-m: enable mirror lock (default: disabled)" << endl
// 	 << "\t-s: download and save files to current directory (default: disabled)" << endl
// 	 << "\t-r: set raw format before shooting" << endl
// 	 << "\t-j: set jpeg format before shooting" << endl
// 	 << endl;
//     return 0;
//   }

  auto logger = make_logger();
  
  
  auto camera = init_camera(logger);
  if(!camera)
    return 1;
  string folder = "/";
  while(true) {
    auto folders = camera->folders(folder);
    auto files = camera->files(folder);
    cout << "Folders: " << endl;
    for(auto f: folders)
      cout << f << "\t";
    cout << endl << "Files: " << endl;
    for(auto f: files)
      cout << f.path() << "\t";
    cout << endl << "New folder to browse: ";
    cin >> folder;
  }
}