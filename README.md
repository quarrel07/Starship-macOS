[comment]: <> (Todo: Make Light Mode Image)
[comment]: <> (Todo: Make Dark Mode Image)

# Starship

## Discord

Official Discord: https://discord.com/invite/shipofharkinian

If you're having any trouble after reading through this `README`, feel free ask for help in the Starship Support text channels. Please keep in mind that we do not condone piracy.

# Quick Start

Starship does not include any copyrighted assets.  You are required to provide a supported copy of the game.

### 1. Verify your ROM dump
You can verify you have dumped a supported copy of the game by using the MD5 File Checksum Online at https://webencrypt.org/onlinetoolsjs/md5_checksum.html. The MD5 hash for a US 1.1 ROM is 741a94eee093c4c8684e66b89f8685e8.

### 2. Download Starship from [Releases](https://github.com/HarbourMasters/starship/releases)

### 3. Launch the Game!
#### Windows
* Extract the zip
* Launch `starship.exe`

#### Linux
* Place your supported copy of the game in the same folder as the appimage.
* Execute `starship.appimage`. You may have to `chmod +x` the appimage via terminal.

#### macOS
* Run `starship.app`.
* When prompted, select your supported copy of the game.

### 4. Play!

Congratulations, you are now sailing with Starship! Have fun!

# Configuration

### Default keyboard configuration
| N64 | A | B | Z | Start | Analog stick | C buttons | D-Pad |
| - | - | - | - | - | - | - | - |
| Keyboard | X | C | Z | Space | WASD | Arrow keys | TFGH |

### Other shortcuts
| Keys | Action |
| - | - |
| F1 | Toggle menubar |
| F11 | Fullscreen |
| Tab | Toggle Alternate assets |
| Ctrl+R | Reset |

### Graphics Backends
Currently, there are three rendering APIs supported: DirectX11 (Windows), OpenGL (all platforms), and Metal (macOS). You can change which API to use in the `Settings` menu of the menubar, which requires a restart.  If you're having an issue with crashing, you can change the API in the `starship.json` file by finding the line `"Backend":{`... and changing the `id` value to `3` and set the `Name` to `OpenGL`. `DirectX 11` with id `2` is the default on Windows. `Metal` with id `4` is the default on macOS.

# Custom Assets

Custom assets are packed in `.o2r` or `.otr` files. To use custom assets, place them in the `mods` folder.

If you're interested in creating and/or packing your own custom asset `.o2r`/`.otr` files, check out the following tools:
* [**retro - OTR and O2R generator**](https://github.com/HarbourMasters64/retro)
* [**fast64 - Blender plugin (Note that MM is not fully supported at this time)**](https://github.com/HarbourMasters/fast64)

# Development
### Building

If you want to manually compile Starship, please consult the [building instructions](docs/BUILDING.md).


### Playtesting
If you want to playtest a continuous integration build, you can find them at the links below. Keep in mind that these are for playtesting only, and you will likely encounter bugs and possibly crashes. 

[comment]: <> (Todo: Make these...)

* [Windows](https://nightly.link/HarbourMasters/2ship2harkinian/workflows/main/develop/2ship-windows.zip)
* [Linux](https://nightly.link/HarbourMasters/2ship2harkinian/workflows/main/develop/2ship-linux.zip)
* [Mac](https://nightly.link/HarbourMasters/2ship2harkinian/workflows/main/develop/2ship-mac.zip)

<a href="https://github.com/Kenix3/libultraship/">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="./docs/poweredbylus.darkmode.png">
    <img alt="Powered by libultraship" src="./docs/poweredbylus.lightmode.png">
  </picture>
</a>
