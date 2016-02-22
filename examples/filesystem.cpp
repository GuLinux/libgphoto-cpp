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
#include <camera_filesystem.h>
using namespace std;
using namespace GPhotoCPP;

struct Command {
  string name;
  vector<string> args;
  operator bool() const { return !name.empty(); }
};

Command get_cmd() {
  string line;
  getline(cin, line);
  stringstream ss{line};
  vector<string> tokens;
  while(!ss.str().empty()) {
    string token;
    ss >> token;
    if(token.empty())
      break;
    tokens.push_back(token);
  }
  Command command;
  if(!tokens.empty()) {
    command.name = *tokens.begin();
    tokens.erase(tokens.begin());
    command.args = tokens;
  }
  return command;
}

ostream &operator<<(ostream &o, const vector<string> &v) {
  for (auto s: v)
    o << s << ", ";
  return o;
}

void cd(CameraFolderPtr &folder, const vector<string> &args) {
  if(args.size()==0) {
    cerr << "Wrong number of arguments\n";
    return;
  }
  auto cd = args[0];
  if(cd == "..") {
    if(!folder->parent()) {
      cerr << "Error! already on root directory\n";
      return;
    }
    folder = folder->parent();
    return;
  }
  auto folders = folder->folders();
  auto new_folder = find_if(begin(folders), end(folders), [&](const CameraFolderPtr &f){ return f->path() == folder->path() + cd + "/"; });

  if(new_folder != end(folders)) {
    folder=*new_folder;
    return;
  }
  cerr << "Folder not found\n";
};

void ls(CameraFolderPtr &folder, const vector<string> &args) {
  for(auto f: folder->folders()) {
    cout << f->path() << endl;
  }
  for(auto f: folder->files()) {
    cout << f->path() << endl;
  }
};


void download(CameraFolderPtr &folder, const vector<string> &args) {
  if(args.size() < 1) {
    cerr << "Wrong number of arguments\n";
  }
  for(auto file: folder->files()) {
    if(count(begin(args), end(args), file->name()) > 0) {
      cout << "Downloading " << file->path() << "...";
      cout.flush();
      file->camera_file()->save(file->name());
      cout << "[OK]" << endl;
    }
  }
}

void rm(CameraFolderPtr &folder, const vector<string> &args) {
  if(args.size() < 1) {
    cerr << "Wrong number of arguments\n";
  }
  for(auto file: folder->files()) {
    if(count(begin(args), end(args), file->name()) > 0) {
      cout << "Removing " << file->path() << "...";
      cout.flush();
      file->remove();
      cout << "[OK]" << endl;
    }
  }
}

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
  CameraFolderPtr folder = camera->root();
  bool keepgoing = true;
  auto quit = [&keepgoing](const Command &) { keepgoing = false; };
  auto help = [](const Command &) {
    cout << "Available commands:\n\tcd <directory>\n\tls\n\tquit\n\tget <files>\n\trm <files>\nPaths are always relative to current directory." << endl;
  };
  map<string, function<void(const Command&)>> commands {
    {"cd", [&folder](const Command &c) { cd(folder, c.args); } },
    {"ls", [&folder](const Command &c) { ls(folder, c.args); } },
    {"quit", quit}, {"exit", quit},
    {"get",[&folder](const Command &c) { download(folder, c.args); } },
    {"rm",[&folder](const Command &c) { rm(folder, c.args); } },
    {"help", help }, {"?", help },
  };

  while(keepgoing) {
    cout << folder->path() << "$> ";
    auto command = get_cmd();
    if(!command|| !commands.count(command.name)) {
      cerr << "Command not recognized\n";
      continue;
    }
    commands[command.name](command);
  }
}