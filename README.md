# cwsw_lib_core
## Base-level (foundational) Subcomponent for CWSW's library suite

The Library component is designed to be placed at the Services level, and is designed to provide core, stateless, utility functions.

This repo implements the core sources for the component, and is intended to be inserted as a submodule in the parent project's sources folder tree.

## Prerequisites
### Environment Variables
Some environments create a compile-time symbol that relates to the "configuration" (Eclipse term) of the current build. This can also be known as a "build target" to some people.

The Library relies on this method to use compile-time selections of certain elements; this is done to enable building in different environments.

Within Eclipse, we rely on the following command-line symbols, defined within the project properties:

Symbol 	| Environment
:---:	|:---
XPRJ_Win_MinGW_Debug	| Eclipse using MinGW, running on Windows, Debug build target (configuration).
XPRJ_Win_Cygwin_Debug_UT	| Eclipse using Cygwin, running on Windows, Debug target w/ CUnit enabled.
XPRJ_Debug_Win_NB     | NetBeans using MinGW, running on Windows, Debug build target (configuration).
XPRJ_Debug_Linux_GCC	| Eclipse running on an embedded Linux. In many regards, this is very close to `XPRJ_Debug_Linux_GCC_Desktop`
XPRJ_Debug_Linux_GCC_Desktop	| Eclipse using GCC, running on Linux, Debug build target.


#### Eclipse, S32DS, and other IDEs built upon Eclipse
tbd

**Note**: Within Eclipse, the way to create the command-line define referenced in the table above, is to create a Define with the definition `XPRJ_${ConfigName}`
* Project _(menu)_ / Properties / C/C++ General / Paths and Symbols / [Symbols] tab / Add / `XPRJ_${ConfigName}=1` 

These definitions are found in Eclipse' `.cproject` file, as is the command-line define.

**Note**: To effect a command-line build using MinGW, you need to manually add both `MinGW/bin` and `msys/1.0/bin` directories to your path statement; without this, you'll need to build within the IDE. If you have Cygwin installed, be careful you don't insert these MinGW folders "in front of" the Cygwin folders that contain the common build tools.


#### NetBeans, MPLab X IDE, and other IDEs built upon NetBeans
tbd

**Note**: MPLAB X automatically creates the `XPRJ_${CONF}` command-line define referenced in the table above. Within NetBeans, you'll need to create it yourself, in the project properties.

**Note**: To effect a command-line build using MinGW, you need to manually add both `MinGW/bin` and `msys/1.0/bin` directories to your path statement; without this, you'll need to build within the IDE. If you have Cygwin installed, be careful you don't insert these MinGW folders "in front of" the Cygwin folders that contain the common build tools.

#### LabWindows/CVI v7 (ancient!)
tbd

#### Visual Studio 2005 (ancient!)
had to add a couple of custom replacements for some stdlib files, if i recall.


### Source file whitespace
I recognize I am not in the majority, but I strongly prefer to use embedded tab characters, rather than the appropriate number of space characters. If you don't like this, then feel free to Select All / Reformat.
