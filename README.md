# Color Management Disabler
Disable color management awareness in applications that support it.

The reason you may want that is because using a color profile might cause the colors on some applications (for example Adobe Photoshop) to appear wrong.

### Examples

* [HELP! Why does Photoshop ruin my colors when I load a display color profile??](https://www.youtube.com/watch?v=4XyIFbll9sA)
* [Why do my Illustrator linked files look desaturated on Photoshop?](https://graphicdesign.stackexchange.com/questions/96164/why-do-my-illustrator-linked-files-look-desaturated-on-photoshop)

## Preview
| Before | After |
| --- | --- |
| ![Color management awareness enabled](https://i.imgur.com/lX7v7Je.png) | ![Color management awareness disabled](https://i.imgur.com/MM1xNLx.png) |

## How it works
This tool makes the [GetICMProfileW](https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-geticmprofilew) function always return false. This effectively disables color management in applications that support it.

## Installation
1. Download the [latest release](https://github.com/Stonos/color-management-disabler/releases)
2. Put version.dll in the same folder as the .exe of the program you want to disable color management
3. Launch your app

In case you want to uninstall, just delete version.dll.

## Acknowledgments
* [MinHook](https://github.com/TsudaKageyu/minhook)
* [DLL Wrapper Generator](https://github.com/SeanPesce/DLL_Wrapper_Generator)
