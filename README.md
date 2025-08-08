# GameboyCore Emulator

Simple GameboyCore emulator written in C++

## Building

### Install Qt 6.8.3

https://doc.qt.io/qt-6/qt-online-installation.html

### Clone the repository

```bash
$ git clone https://github.com/collin-b1/gameboy-emulator.git
$ cd gameboy-emulator/
```

### Build the program

```bash
$ mkdir build/ && cd build/
$ cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/msvc2022_64"
```

## Using a bootrom

The program will work without a bootrom, but if you'd like to include one to see the intro logo sequence, follow the
below steps:

1. Create a `/roms` directory inside the project root folder
2. Put the `dmg_boot.bin` file inside of the `/roms` directory 