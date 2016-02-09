#ifndef SAMPLES_COMMONS_H
#define SAMPLES_COMMONS_H

#include <iostream>
#include "camera.h"
#include "driver.h"
#include "widgets.h"
#include <sstream>
#include <iomanip>
#include <map>
#include "camerafile.h"
#include "serialshooter.h"
#include "gphotowidgetshooter.h"
#include <list>
#include <algorithm>
using namespace std;
using namespace GPhoto;

static vector<string> args;

void init_options(int argc, char **argv) {
  copy(argv+1, argv+argc, begin(args));
}

bool has_option(const string &option) {
  return find(begin(args), end(args), option) != end(args);
};

LoggerPtr make_logger(Logger::Level max_level = Logger::DEBUG) {
  return make_shared<Logger>([max_level](const string &message, Logger::Level level){
    static map<Logger::Level, string> levels {
      {Logger::DEBUG, "DEBUG"},
      {Logger::ERROR, "ERROR"},
      {Logger::INFO, "INFO"},
      {Logger::TRACE, "TRACE"},
      {Logger::WARNING, "WARNING"},
    };
    if(level < max_level)
      return;
    cerr << "[" << setfill(' ') << setw(8) << levels[level] << "] " << message << endl;
  });
}

CameraPtr init_camera(const LoggerPtr &logger) {
  GPhoto::Driver driver(logger);
  GPhoto::CameraPtr camera = driver.autodetect();
  if(!camera) {
    cerr << "Error finding camera" << endl;
    return {};
  }
  cout << "Found camera: " << camera << endl;
  return camera;
}
#endif