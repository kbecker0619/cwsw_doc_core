# cwsw_lib_core
## Base-level (foundational) Subcomponent for CWSW's library suite

The Library component is designed to be placed at the Services level, and is designed to provide core, stateless, utility functions.

This repo implements the core sources for the component, and is intended to be inserted as a submodule in the parent project's sources folder tree.

## Prerequisites
### Environment Variables
Some environments create a compile-time symbol that relates to the "configuration" (Eclipse term) of the current build. This can also be known as a "build target" to some people.

The Library relies on this method to use compile-time selections of certain elements; this is done to enable building in different environments.

Within Eclipse, we rely on the following symbols:

Symbol 	| Environment
:---:	|:---
XPRJ_Debug_Win_MinGW	| Eclipse using MinGW, running on Windows, Debug build target (configuration).
XPRJ_Debug_Linux_GCC	| Eclipse running on an embedded Linux. In many regards, this is very close to `XPRJ_Debug_Linux_GCC_Desktop`
XPRJ_Debug_Linux_GCC_Desktop	| Eclipse using GCC, running on Linux, Debug build target.

Within Eclipse, the easy way to make this command-line define is to create a Define with the definition `XPRJ_${ConfigName}`

Project / Properties / C/C++ General / Paths and Symbols / [Symbols] tab / Add / `XPRJ_${ConfigName}=1` 

These definitions are found in Eclipse' `.cproject` file, as is the command-line define.

#### Eclipse
tbd

#### NetBeans
tbd

#### MPLab IDE
tbd

#### LabWindows/CVI v7 (ancient!)
tbd

#### Visual Studio 2005 (ancient!)
had to add a couple of custom replacements for some stdlib files, if i recall.


### Source file whitespace
I recognize I am not in the majority, but I strongly prefer to use embedded tab characters, rather than the appropriate number of space characters. If you don't like this, then feel free to Select All / Reformat.
