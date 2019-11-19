# LibHand Generator

A data generation tool for hand pose estimation research.

# Dependencies

- [Libhand-Public](https://github.com/jsupancic/libhand-public.git)
- Ogre 1.9.0 (TODO: Support latest version.)
- OpenCV 3.2.0
- OpenGL
- GLUT

# Installation

```
mkdir build && cd build
cmake -DLIBHAND_PATH="/path/to/libhand-public" -DOPENCV_PATH="/path/to/opencv" ..
```

If you have multiple versions of OpenCV configured on your system, you can specify the path for version 3.2 using the `OPENCV_PATH` variable. Otherwise, CMake's `find_package` will automatically detect it.

# Troubleshooting

If you installed OGRE 1.9 by using `apt install libogre-1.9-dev`, the library files are not actually located in `/usr/lib/OGRE`.

```
Loading library /usr/lib/OGRE/RenderSystem_GL.so
terminate called after throwing an instance of 'Ogre::InternalErrorException'
  what():  OGRE EXCEPTION(7:InternalErrorException): Could not load dynamic library /usr/lib/OGRE/RenderSystem_GL.so.  System Error: /usr/lib/OGRE/RenderSystem_GL.so: cannot open shared object file: No such file or directory in DynLib::load at /build/ogre-1.9-mqY1wq/ogre-1.9-1.9.0+dfsg1/OgreMain/src/OgreDynLib.cpp (line 109)
Aborted (core dumped)
```

This is easily remedied with a symbolic link:

```
sudo ln -s /usr/lib/x86_64-linux-gnu/OGRE-1.9.0 /usr/lib/OGRE
```

# TODO

- [ ] Add usage section.
- [ ] Develop Python wrapper.
