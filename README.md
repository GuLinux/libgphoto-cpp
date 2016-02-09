# libgphoto-cpp
libgphoto++ - a modern c++11 wrapper for libgphoto2


## Build
libgphoto++ requires only gphoto2 (ubuntu: sudo apt-get install libgphoto2-dev)

    cd libgphoto-cpp
    mkdir build
    cd build
    cmake ../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr
    
You might then try the applications in the "examples" folder to see if the library is properly working for you.
