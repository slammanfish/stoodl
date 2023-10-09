# STOODL (SDL Tools)
A single header SDL extension library!

# Overview
STOODL (SDL Tools) provides basic input and time handling for games and engines made with SDL.
STOODL is split up into two modules, each of which can be enabled or disabled

# Features
* [x] Delta Time
* [x] Elapsed Time
* [x] Tick System
* [x] Input Handling
* [x] Rebindable Input Actions
* [x] Keyboard And Mouse Support
* [x] Mouse Position
* [ ] Saving keybinds to file
* [ ] Getting keycode when a key is pressed

# Getting Started
- Add the `stoodl.h` file to your project
- To use the time module, add this line before including `stoodl.h`
    ```c
    // Where [tick_time] is the time in seconds of a tick
    #define TIME [tick_time]
    ```

- To use the input module, add this line before including `stoodl.h`
    ```c
    #define INPUT
    ```
- (Optional) To add custom keybinds, create a `keybinds.ini` file and specify its path with the following line before including this file
    ```c
    // Where [file_path] is the path in the project directory to keybinds.ini
    #define KEYBIND_PATH [file_path]
    ```

- More indepth instructions can be found at the top of the `stoodl.h` file
