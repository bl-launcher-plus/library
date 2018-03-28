# BLoader

Library that is attached to Blockland.exe to enable easier access to TorqueEngine functionality. It is designed to work along with the [launcher].

## DISCLOSURE

INSTALLTING AND USING THIS LIBRARY *WILL* VIOLATE THE LICENSE OF THE [Blockland EULA]. WE HOLD NO RESPONSIBILITY OF EITHER DAMAGE TO YOUR PROPERTY OR REVOKE OF ANY NUMBER OF KEYS.

## Installation

Download the compiled DLL and put in your Blockland folder.

*(Insert possible ways to inject or modify this to work with Blockland)*

## Developing

Download the latest binary along with the lib file and [blibrary.h], [bloader.h], and eventually [bloader.hpp] if a C++ interface is more comfortable.

Implement the three functions shown in [blibrary.h]. Save the module variable for later, as that is used to identify your library. Also provide information in info about what your library is. This will be used later on to be displayed in the [launcher] and in-game.

If you are in need to add TorqueEngine functions, you do so by using `bloader_consolefunction_*`. Same goes for variables and `bloder_consolevariable_*`.

Get modules information through `bloader_module_*`.


[bloader.h]: https://github.com/bl-launcher-plus/library/blob/master/include/bloader.h
[bloader.h]: https://github.com/bl-launcher-plus/library/blob/master/include/bloader.hpp
[blibrary.h]: https://github.com/bl-launcher-plus/library/blob/master/project/blibrary.h
[launcher]: https://github.com/bl-launcher-plus/launcher
[Blockland EULA]: https://blockland.us/staticPages/EULA.txt
