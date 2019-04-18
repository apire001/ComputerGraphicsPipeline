# cs130-Simplified OpenGL 3D Rendering Pipeline

CS 130 OpenGL Graphics Pipeline Project by Andrew Pirelli

## Introduction

This program performs various duties including but not limited to handling vertex and fragment shading calls, rasterization and interpolation, clipping, and using a z-buffer for hidden surfaces.

### Requirements For Compilation and Execution:

```
Access to bolt.cs.ucr.edu server
```

### Install Instructions

```
$ git clone https://github.com/apire001/ComputerGraphicsPipeline.git
$ cd proj-gl-files
$ scons
$ ./ray_tracer -i input.txt #Output is stored in output.png
```

### Known Bugs

* Rendering has not been fully optimized, heavy rendering may cause the program to time out.
