/*
  █████████  ███████████    ███████       ███████    ██████████   █████      
 ███░░░░░███░█░░░███░░░█  ███░░░░░███   ███░░░░░███ ░░███░░░░███ ░░███       
░███    ░░░ ░   ░███  ░  ███     ░░███ ███     ░░███ ░███   ░░███ ░███       
░░█████████     ░███    ░███      ░███░███      ░███ ░███    ░███ ░███       
 ░░░░░░░░███    ░███    ░███      ░███░███      ░███ ░███    ░███ ░███       
 ███    ░███    ░███    ░░███     ███ ░░███     ███  ░███    ███  ░███      █
░░█████████     █████    ░░░███████░   ░░░███████░   ██████████   ███████████
 ░░░░░░░░░     ░░░░░       ░░░░░░░       ░░░░░░░    ░░░░░░░░░░   ░░░░░░░░░░░                                         
-----------------------------------------------------------------------------
                                An SDL Addon
-----------------------------------------------------------------------------               
*/

/*

STOODL (SDL Tools) provides basic input and time handling for games and engines made with SDL.
STOODL is split up into two modules, each of which can be enabled or disabled

IMPORTANT

Add this line in a .c file before including this file
    #define STOODL_IMPLEMENTATION
    #include <stoodl.h>

To use STOODL, call stoodl_init() after initialising SDL, and call stoodl_update() after SDL event handling and before your game logic
Don't forget to call stdool_free() at the end of your program!

== Time ==
    [Using]
        - Add this line before including this file
            #define TIME [tick_time]
        - Where [tick_time] is the time in seconds of a tick
            e.g #define TIME 0.5 
                defines a tick time of half a second

    [Features]
        - Delta time
        - Elapsed time
        - Tick system

    [Functions]
        - delta_time(void)
            - Returns the delta time as a double
        - elapsed_time(void)
            - Returns the elapsed time as a double
        - on_tick(void)
            - Returns true if the game ticked this frame

== Input ==
    [Using]
        - Add this line before including this file
            #define INPUT
        - To add custom keybinds, create a keybinds.ini file and specify its path with the following line before including this file
            #define KEYBIND_PATH [file_path]
        - Where [file_path] is the path in the project directory to keybinds.ini
            e.g #define KEYBIND_PATH "bin/data/keybinds.ini"
                defines a keybind path of bin/data/keybinds.ini from the project directory
        - The default config is a string in this file and is not stored on disc, therefor the KEYBIND_PATH definition is not necessary but recommended

    [Creating A Custom keybinds.ini File]
        - The format of the keybinds.ini file looks like this

                [Action] : [Code]
            
            - Where [Action] is a string you assign to the [Code]
                - [Action] can be anything you want. This is how you reference the keybind in your code
            - Where [Code] is an SDL_ScanCode from this list https://wiki.libsdl.org/SDL2/SDL_Scancode with the exception of mouse buttons
                - For mouse buttons the Codes are below
                    lmb - Left iMouse Button
                    rmb - Right Mouse Button
                    mmb - Middle Mouse Button
                    mx1 - X Mouse Button 1
                    mx2 - X Mouse Button 2
            - ONLY one pair of codes and actions per line
            - DO NOT use uppercase letters
            - DO NOT use spaces. Use '_' or '-' instead
            - DO NOT include anything else in the keybinds.ini file
            - NOTE There is no file existence checking, so make sure the KEYBIND_PATH points to a proper file location
            - NOTE You can use any file type with any file name, just make sure the KEYBIND_PATH reflects that

        - The default keybind settings look like this
            w       : up
            s       : down
            a       : left
            d       : right
            space   : jump
            lmb     : use
            rmb     : secondary

    [Features]
        - Action / Input system
            - Up, Pressed, and Held input states
        - Keyboard and Mouse support
        - Rebindable keys with config file

    [Functions]
        - is_button_up(char *action)
            - Returns true if the specified action is UP this frame
        - is_button_pressed(char *action)
            - Returns true if the specified action is PRESSED this frame
        - is_button_held(char *action)
            - Returns true if the specified action is HELD this frame
        - get_mouse_position(void)
            - Returns an int[2] containing the mouse positions (x, y)
*/

#ifndef STOODL_H
#define STOODL_H

#include <SDL2/SDL.h>
#include <string.h>

//---------------------------------------------------------------------------------------------------------------
// Util
//---------------------------------------------------------------------------------------------------------------

    void stoodl_util_str_strip(char *str, char ch);

#ifdef STOODL_IMPLEMENTATION
    inline void stoodl_util_str_strip(char *str, char ch) {
        int len = strlen(str);
        int i, j = 0;

        for (i = 0; i < len; i++) {
            if (str[i] != ch) {
                str[j] = str[i];
                j++;
            }
        }
        str[j] = '\0';
    }
#endif

//---------------------------------------------------------------------------------------------------------------
// Time
//---------------------------------------------------------------------------------------------------------------

#ifdef TIME

    static struct {
        unsigned long long program_start, program_current, program_last;
        double delta, elapsed;
        int tick, tick_time, on_tick;
    } stoodl_time;

    int stoodl_time_init();
    void stoodl_time_update();
    void stoodl_time_free();
    double elapsed_time();
    double delta_time();
    int on_tick();
#ifdef STOODL_IMPLEMENTATION
    inline int stoodl_time_init() {
        stoodl_time.program_start = SDL_GetPerformanceCounter();
        stoodl_time.program_current = stoodl_time.program_start;
        stoodl_time.program_last = 0;
        stoodl_time.delta = 0;
        stoodl_time.elapsed = 0;

        return 1;
    }

    inline void stoodl_time_update() {
        // Update time
        stoodl_time.program_last = stoodl_time.program_current;
        stoodl_time.program_current = SDL_GetPerformanceCounter();
        stoodl_time.delta = (double) ((stoodl_time.program_current - stoodl_time.program_last) / (double) SDL_GetPerformanceFrequency());
        stoodl_time.elapsed = (double) ((stoodl_time.program_current - stoodl_time.program_start) / (double) SDL_GetPerformanceFrequency());
        
        // Update ticks
        stoodl_time.tick_time += stoodl_time.delta;
        stoodl_time.on_tick = 0;
        if (stoodl_time.tick_time >= TIME) {
            stoodl_time.tick_time -= TIME;
            stoodl_time.tick++;
            stoodl_time.on_tick = 1;
        }
    }

    inline void stoodl_time_free() {

    }

    inline double elapsed_time() {
        return stoodl_time.elapsed;
    }

    // Returns delta time
    inline double delta_time() {
        return stoodl_time.delta;
    }

    // Returns 1 if the game ticked this frame.
    // Tick rate is determined by the definition of the TIME macro
    inline int on_tick() {
        return stoodl_time.on_tick;
    }
#endif

#endif

//---------------------------------------------------------------------------------------------------------------
// Input
//---------------------------------------------------------------------------------------------------------------

#ifdef INPUT

#ifndef KEYBIND_PATH
    const char *stoodl_default_keybinds =
        "w      : up\n"
        "s      : down\n"
        "a      : left\n"
        "d      : right\n"
        "space  : jump\n"
        "lmb    : use\n"
        "rmb    : secondary\0";

#endif

    typedef enum {
        INPUT_STATE_UP = 0,
        INPUT_STATE_PRESSED = 1,
        INPUT_STATE_HELD = 2
    } stoodl_input_state;

    typedef struct {
        char code[128];
        char action[128];
        stoodl_input_state state;
        int mouse_button;
    } stoodl_key_map;

    static struct {
        int action_count;
        stoodl_key_map *key_maps;
        int mouse_position[2];

        unsigned char *keyboard_state;
        unsigned int mouse_state;
    } stoodl_input_controller;

    static void stoodl_keystate_keyboard_update(unsigned int state, stoodl_key_map *key);
    static void stoodl_keystate_mouse_update(unsigned state, stoodl_key_map *key);
    void stoodl_input_read_config();
    int stoodl_input_init();
    void stoodl_input_update();
    void stoodl_input_free();
    int is_button_up(char *action);
    int is_button_pressed(char *action);
    int is_button_held(char *action);
    int *get_mouse_position();

#ifdef STOODL_IMPLEMENTATION
    inline static void stoodl_keystate_keyboard_update(unsigned int state, stoodl_key_map *key) {
        if (state) {
            if (key->state > 0) {
                key->state = INPUT_STATE_HELD;
            } else {
                key->state = INPUT_STATE_PRESSED;
            }
        } else {
            key->state = INPUT_STATE_UP;
        }
    }

    inline static void stoodl_keystate_mouse_update(unsigned state, stoodl_key_map *key) {
        if (key->mouse_button != 0) {
            if (state & SDL_BUTTON(key->mouse_button)) {
                if ((int) key->state > 0) {
                    key->state = INPUT_STATE_HELD;
                } else {
                    key->state = INPUT_STATE_PRESSED;
                }
            } else {
                key->state = INPUT_STATE_UP;
            }
        }
    }

    inline void stoodl_input_read_config() {
        char *keybind_config;
        int keybind_count = 1;
        #ifdef KEYBIND_PATH
            FILE *file;
            file = fopen(KEYBIND_PATH, "r");
            if (file != NULL) {
                fseek(file, 0, SEEK_END);
                int length = ftell(file);
                fseek(file, 0, SEEK_SET);
                keybind_config = malloc(sizeof(char) * (length + 1));
                char c;
                int i = 0;
                while ((c = fgetc(file)) != EOF) {
                    keybind_config[i] = c;
                    i++;
                }
                keybind_config[i] = '\0';
            } else {
                printf("Could not open file: %s\nUsing default keybind settings\n", KEYBIND_PATH);
            }
            fclose(file);
        #endif

        #ifndef KEYBIND_PATH
            keybind_config = stoodl_default_keybinds;
        #endif

        // Count keybinds (Lines in the files)
        for (int i = 0; keybind_config[i]; i++)
            keybind_count += (keybind_config[i] == '\n');

        // Initialise keybind array
        stoodl_input_controller.action_count = keybind_count;
        stoodl_input_controller.key_maps = (stoodl_key_map*) malloc(sizeof(stoodl_key_map) * stoodl_input_controller.action_count);

        // Read in keybinds

        int i = 0;
        char *start = keybind_config;

        while (*start != '\0') {
            char left[128], right[128];
            if (sscanf(start, "%s : %s", left, right) == 2) {
                stoodl_util_str_strip(left, ' ');
                stoodl_util_str_strip(right, ' ');
                strcpy(stoodl_input_controller.key_maps[i].code, left);
                strcpy(stoodl_input_controller.key_maps[i].action, right);
                
                // Load mouse code
                // 0 if the button is not a mouse button
                stoodl_input_controller.key_maps[i].mouse_button = 0;
                if (!strcmp(left, "lmb")) {
                    stoodl_input_controller.key_maps[i].mouse_button = 1;
                } else if (!strcmp(left, "mmb")) {
                    stoodl_input_controller.key_maps[i].mouse_button = 2;
                } else if (!strcmp(left, "rmb")) {
                    stoodl_input_controller.key_maps[i].mouse_button = 3;
                } else if (!strcmp(left, "mx1")) {
                    stoodl_input_controller.key_maps[i].mouse_button = 4;
                } else if (!strcmp(left, "mx2")) {
                    stoodl_input_controller.key_maps[i].mouse_button = 5;
                }

                stoodl_input_controller.key_maps[i].state = INPUT_STATE_UP;
                i++;
            }

            while (*start != '\n' && *start != '\0') {
                start++;
            }

            if (*start == '\n') {
                start++;
            }
        }

        // Free the keybind config (Just incase)
        free(keybind_config);
    }

    inline int stoodl_input_init() {
        stoodl_input_read_config();
    }

    inline void stoodl_input_update() {
        stoodl_input_controller.keyboard_state = SDL_GetKeyboardState(NULL);
        stoodl_input_controller.mouse_state = SDL_GetMouseState(&stoodl_input_controller.mouse_position[0], &stoodl_input_controller.mouse_position[1]);

        for (int i = 0; i < stoodl_input_controller.action_count; i++) {
            if (stoodl_input_controller.key_maps[i].mouse_button != 0) {
                stoodl_keystate_mouse_update(stoodl_input_controller.mouse_state, &stoodl_input_controller.key_maps[i]);
            } else {
                stoodl_keystate_keyboard_update(stoodl_input_controller.keyboard_state[(int) SDL_GetScancodeFromName(stoodl_input_controller.key_maps[i].code)], &stoodl_input_controller.key_maps[i]);
            }
        }
    }

    inline void stoodl_input_free() {
        free(stoodl_input_controller.key_maps);
        stoodl_input_controller.key_maps = NULL;
    }

    inline int is_button_up(char *action) {
        for (int i = 0; i < stoodl_input_controller.action_count; i++) {
            if (!strcmp(action, stoodl_input_controller.key_maps[i].action)) {
                return stoodl_input_controller.key_maps[i].state == INPUT_STATE_UP ? 1 : 0;
            }
        }
        return 0;
    }

    inline int is_button_pressed(char *action) {
        for (int i = 0; i < stoodl_input_controller.action_count; i++) {
            if (!strcmp(action, stoodl_input_controller.key_maps[i].action)) {
                return stoodl_input_controller.key_maps[i].state == INPUT_STATE_PRESSED ? 1 : 0;
            }
        }
        return 0;
    }

    inline int is_button_held(char *action) {
        for (int i = 0; i < stoodl_input_controller.action_count; i++) {
            if (!strcmp(action, stoodl_input_controller.key_maps[i].action)) {
                return stoodl_input_controller.key_maps[i].state == INPUT_STATE_HELD ? 1 : 0;
            }
        }
        return 0;
    }

    inline int *get_mouse_position() {
        return stoodl_input_controller.mouse_position;
    }
#endif

#endif

//---------------------------------------------------------------------------------------------------------------
// Stoodl
//---------------------------------------------------------------------------------------------------------------

    void stoodl_init();
    void stoodl_update();
    void stoodl_free();

#ifdef STOODL_IMPLEMENTATION
    inline void stoodl_init() {
        #ifdef TIME
            if (stoodl_time_init()) {
                printf("Successfully Initialised STOODL :: Time\n");
            } else {
                printf("Failed to Initialise STOODL :: Time\n");
            }
        #endif

        #ifdef INPUT
            if (stoodl_input_init()) {
                printf("Successfully Initialised STOODL :: Input\n");
            } else {
                printf("Failed to Initialise STOODL :: Input\n");
            }
        #endif
    }

    inline void stoodl_update() {
        #ifdef TIME
            stoodl_time_update();
        #endif

        #ifdef INPUT
            stoodl_input_update();
        #endif
    }

    inline void stoodl_free() {
        #ifdef TIME
            stoodl_time_free();
        #endif

        #ifdef INPUT
            stoodl_input_free();
        #endif
    }
#endif

#endif
