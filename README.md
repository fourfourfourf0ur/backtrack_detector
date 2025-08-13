# Backtrack Detector ![C/C++ CI](https://github.com/fourfourfourf0ur/backtrack_detector/workflows/CMake/badge.svg)
A module for ReHLDS designed to detect players using the backtrack cheat (also known as fake latency)
## Requirements
* [ReHLDS](https://github.com/rehlds/ReHLDS/releases) version 3.10 or above 
## Downloads
* [Release builds](https://github.com/fourfourfourf0ur/backtrack_detector/releases)
## Installation
<details>
<summary>Click to expand</summary>

1. Go to `<gamedir>/addons/` and make new directory named `backtrack_detector`<br/>
`<gamedir>` - its a game directory (cstrike for Counter-Strike, valve for Half-Life, etc)

2. Copy `backtrack_detector_mm.dll` or `backtrack_detector_mm_i386.so` to `<gamedir>/addons/backtrack_detector/`

3. Go to metamod installation directory (usually its `<gamedir>/addons/metamod/`) and edit `plugins.ini`:<br/>
add this line at the beginning of the file<br/>
for windows<br/>
`win32 addons\backtrack_detector\backtrack_detector_mm.dll`<br/>
for linux<br/>
`linux addons/backtrack_detector/backtrack_detector_mm_i386.so`<br/>

4. Ready to use
</details>