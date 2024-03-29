# qtjack_midi_debugger
a small Qt programm to debug midi msgs send through Jack Server
- WIP but running, see history
![Alt text](documentation/screenshot.png?raw=true "qtjack_midi_debugger with patchage in background")
- simple example of a Qt GUI program to see information of Midi inputs (timestamp, channel,value...)
- CLI program is planned (WIP)

## Info
Dependencies:

* Qt 5.4 or greater
* QJack, which itself depends on Jack2

On Ubuntu, you need to install dependencies:
```
sudo apt-get install libjack-jackd2-dev
```
QtJack: https://github.com/majorx234/qtjack
- need to be built and installed (follow install section in project's README)

On Arch Linux:
```
sudo pacman -S jack2 qt5-base
```
- QtJack: https://github.com/majorx234/qtjack
- easy to build as package: https://github.com/majorx234/qtjack_PKGBUILD

## Usage:
- build project:
```
git clone https://github.com/majorx234/qtjack_midi_debugger.git
cd qtjack_midi_debugger
mkdir build
cd build
cmake ..
make
```
- on Arch Linux:
- use PKGBUILD script die create package:
- https://github.com/majorx234/qtjack_midi_debugger_PKGBUILD

- run:
```
./qtjack_midi_debugger_app
```

- connect midi devices or other programs to inpput via tools like patchage, gladish...

## History:
- 2023-02-08 Add support for Midi SysEx Messages (Send & Receive)
- 2022-06-01 Add timestamp in ms
- 2022-06-01 Version 1.0.0 with install section
- 2022-05-30 toggle via GUI elements printing of midi messages
- 2022-05-28 First running Version of GUI Programm

## ToDo:
- support for longer SysEx Midi messages >16 Byte (need rt save memory alloc)
- GUI elements like start/stop recording
- save history in Midi file 

# References:
- inspired by '''qmidimonitor''':
  - git://repo.or.cz/gmidimonitor.git
  - https://github.com/nedko/gmidimonitor
