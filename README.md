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