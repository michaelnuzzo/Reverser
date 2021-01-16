<div  align="center">

# Reverser

<img width="400px" src="docs/imgs/reverser logo 3.png">

</div>

## Overview

The Reverser is a real-time VST plugin that splits incoming audio into chunks and reverses each chunk.

<div  align="center">

<img width="600px" src="docs/imgs/screenshot.png">

</div>



## Install

The plugin itself can be downloaded from the [Releases page](https://github.com/michaelnuzzo/Reverser/releases) in this repository. Once downloaded, the plugin can be installed by simply dragging the file into your plugins folder. 

```
VST3:
Macintosh HD/Library/Audio/Plug-Ins/VST3
AU (Component):
Macintosh HD/Library/Audio/Plug-Ins/Components

```
The .app can be run as a standalone without a DAW or further installation.

## Build

At present, the available binaries are AU, VST, and a macOS app. If you would like to generate your own binary for another file format or platform, you can compile your own plugin.

First, install [JUCE](https://juce.com/get-juce/download).

Then clone this repository to your local machine.

```
git clone https://github.com/michaelnuzzo/Reverser.git
```

Import into your JUCE project, then tweak your build settings as needed.
