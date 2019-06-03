# PTEngine
Planet Engine - Award winning project now GNU/GPL

The Planet Engine is designed to dynamicly generate spherical terrain (planet) using staticly defined configuration via *.conf file.
Process of 3D spherical terrain is based using Perilin noise with Fractal of Brown Movement (fBm).

## Features

- [x] Math library
- [x] Task system
- [x] Image loading (PNG, DDS)
- [x] Built-in profiler
- [x] Video recording via AVI
- [x] Terrain based generation using Perilin noise and Fractal of Brown Movement (fBm)
- [x] Dynamic texture streaming and generation
- [x] Advanced Level of Detail
- [x] Frustum culling
- [x] Simplistic collision model (Camera Based)
- [x] Effects: - Atmosphere glow

## Controls

Manipulating insinde aplication is defined by control scheme below

Keyboard combination | Description
---------------------|-------------
W,S,A,D | Orbit movement
Space | Stop
'c' | Capture video
'h' | Hdr On/Off
'p' | Polygon mode
'u' | Pause / resume update
'+ / -' | Split factor

## Description of Config file

Planets can or might by modified/added via configuration file, using a following description. Before every configuration variable there is a <b>planet</b> name tag followed by <b>[identificator]</b> which is starting always from 0.

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
factory.[factory_id].type | CLASS_NAME |  
factory.[factory_id].seed | VALUE | Seed number used for terrain generation
factory.[factory_id].octaves | VALUE | Number of octaves used for terrain generation (Perilin noise)
factory.[factory_id].roughness |VALUE | Surface roughness used for terrain generation
factory.[factory_id].color.[color_id] | R, G, B, A | - 
