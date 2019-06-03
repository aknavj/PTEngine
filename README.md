# PTEngine

The Planet Engine is designed to generate spherical terrain (planets) using static configuration via a * .conf file.
The process of 3D spherical terrain generation is based on the implementation of methods like Perlin noise and Fractal of the Brown Movement (fBm).

## Features

- [x] Math library
- [x] Task system
- [x] Image loading (PNG, DDS)
- [x] Built-in profiler
- [x] Video recording via AVI
- [x] Terrain based generation using Perlin noise and the Fractal of Brown Movement (fBm)
- [x] Dynamic texture streaming and generation
- [x] Advanced Level of Detail
- [x] Frustum culling
- [x] Simplistic collision model (Camera Based)
- [x] Effects: - Atmosphere glow

## Compilation

For a successful compilation, you don't need any additional libraries, because solution already coming with external dependencies. So only what you need to worry about is to have a working C/C++ compiler's toolchain.

- [x] MICROSOFT WINDOWS: Microsoft Visual Studio 2013
- [ ] LINUX + MULTIPLATFORM : Sconstruct build system

## Controls

Manipulating inside the application is defined by the control scheme below.

Keyboard combination | Description
---------------------|-------------
Keyboard arrows | Orbit rotation
W,S,A,D | Orbit movement
Space | Stop
'c' | Capture video
'h' | Hdr On/Off
'p' | Polygon mode
'u' | Pause / resume update
'+ / -' | Split factor

## Description of Config file

Planets can or might be modified/added via a configuration file, using the following description. Before every configuration variable, there is a **planet** name tag followed by **[identificator]** which is starting always from 0. 

You can just see, edit and play around with the **"Generator.conf"** file to see what is happening in the application.

### Example:

Definition | Example | Description
---------- | ------- | ----------
planet.[planet_id].[variable] | planet.0.position = 0, 0, 0 | Will place first planet on the [0,0,0] world coordinates.

### Overview:

Variable | Value | Description
---------|-------|-------------
position | X , Y , Z | Planet position in 3D space
radius | VALUE | Planet size
frustum.radius | VALUE | Frustum culling radius
max.height | VALUE | Planet maximum size 
atmosphere.radius | VALUE | Planet atmosphere radius
atmosphere.wavelength | X , Y , Z | Atmosphere Wave lenght
atmosphere.Esun | VALUE | Sun intensity (in atmosphere, on planet)
atmosphere.Kr | VALUE |
atmosphere.Km | VALUE |
atmosphere.g | VALUE | Planet's gravity definition (Earth -9,81)
factory.[factory_id].type | CLASS_NAME |  Example CSimpleCraterFactory, CGrassFactory, CSimpleColorMapFactory
factory.[factory_id].seed | VALUE | Seed number used for terrain generation
factory.[factory_id].octaves | VALUE | Number of octaves used for terrain generation (Perilin noise)
factory.[factory_id].roughness |VALUE | Surface roughness used for terrain generation
factory.[factory_id].color.[color_id] | R, G, B, A | Color values
