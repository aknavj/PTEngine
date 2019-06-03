# PTEngine
Planet Engine - Award winning project now GNU/GPL

The Planet Engine is designed to dynamicly generate spherical terrain (planet) using staticly defined configuration via *.conf file.
Process of 3D spherical terrain is based using Perilin noise with Fractal of Brown Movement (fBm).

## Features

- Math library
- Task system
- Image loading (PNG, DDS)
- Built-in profiler
- Video recording via AVI
- Terrain based generation using Perilin noise and Fractal of Brown Movement (fBm)
- Dynamic texture streaming and generation
- Advanced Level of Detail
- Frustum culling
- Simplistic collision model (Camera Based)
- Effects: - Atmosphere glow


## Controls

Manipulating insinde aplication is defined by control scheme below

W,S,A,D - Orbit movement
Space - Stop
'c' - Capture video
'h' - Hdr On/Off
'p' - Polygon mode
'u' - Pause / resume update
'+ / -' - Split factor

## Description of Config file

planet.[planet_id].position                       = x , y , z   - Planet position in 3D space
planet.[planet_id].radius                         = value       - Planet size
planet.[planet_id].frustum.radius                 = value       - Frustum culling radius
planet.[planet_id].max.height                     = value       - Planet maximum size 
planet.[planet_id].atmosphere.radius              = value       - Planet atmosphere radius
planet.[planet_id].atmosphere.wavelength          = x , y , z (ustanovení velikosti vlnové délky atmosféry) 
planet.[planet_id].atmosphere.Esun                = value       - Sun intensity (in atmosphere, on planet)
planet.[planet_id].atmosphere.Kr                  = value 
planet.[planet_id].atmosphere.Km                  = value
planet.[planet_id].atmosphere.g                   = value       - Planet's gravity definition (Earth -9,81)
planet.[planet_id].factory.[factory_id].type      = CLASS_NAME 
planet.[planet_id].factory.[factory_id.seed       = VALUE       - Seed number used for terrain generation
planet.[planet_id].factory.[factory_id.octaves    = VALUE       - Number of octaves used for terrain generation (Perilin noise)
planet.[planet_id].factory.[factory_id.roughness  = VALUE      - Surface roughness used for terrain generation
planet.[planet_id].factory.[factory_id.color.1 = 0, 0, 74, 255 - 
