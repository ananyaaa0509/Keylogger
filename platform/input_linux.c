#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>

#include "input_linux.h"
#include "logger.h"
#define BITS_PER_LONG (sizeof(unsigned long) * 8)
#define NBITS(x) (((x) + BITS_PER_LONG - 1) / BITS_PER_LONG)
#define TEST_BIT(bit, array) \
    ((array[(bit) / BITS_PER_LONG] >> ((bit) % BITS_PER_LONG)) & 1)

static int find_keyboard_device(char *out_path, size_t path_len) {
    struct dirent *ent;
    DIR *dir = opendir("/dev/input");
    if (!dir) return 0;

    while ((ent = readdir(dir)) != NULL) {
        if (strncmp(ent->d_name, "event", 5) == 0) {
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "/dev/input/%s", ent->d_name);

            int fd = open(filepath, O_RDONLY | O_NONBLOCK);
            if (fd >= 0) {
                unsigned long keybit[NBITS(KEY_MAX)] = {0};
                // Check if device supports key events and specifically KEY_A
                if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit) >= 0) {
                    if (TEST_BIT(KEY_A, keybit)) {
                        snprintf(out_path, path_len, "%s", filepath);
                        close(fd);
                        closedir(dir);
                        return 1;
                    }
                }
                close(fd);
            }
        }
    }
    closedir(dir);
    return 0;
}
static char keycode_to_char(unsigned short code, int shift) {
    if (code >= KEY_A && code <= KEY_Z) {
        char key = 'a' + (code - KEY_A);

        if (shift) {
            key = 'A' + (code - KEY_A);
        }

        return key;
    }

    return 0;
}


InputResult linux_start_platform_input(void) {
    char dev_path[512];
    if (!find_keyboard_device(dev_path, sizeof(dev_path))) {
        strncpy(dev_path, "/dev/input/event0", sizeof(dev_path));
    }

    int fd = open(dev_path, O_RDONLY);
    if (fd < 0) {
        return INPUT_ERROR;
    }

    struct input_event ev_raw;
    int shift = 0, ctrl = 0, alt = 0;

    while (read(fd, &ev_raw, sizeof(ev_raw)) > 0) {
        if (ev_raw.type != EV_KEY) continue;

        unsigned short code = ev_raw.code;
        int value = ev_raw.value; 

        if (code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT) {
            shift = (value != 0);
        } else if (code == KEY_LEFTCTRL || code == KEY_RIGHTCTRL) {
            ctrl = (value != 0);
        } else if (code == KEY_LEFTALT || code == KEY_RIGHTALT) {
            alt = (value != 0);
        }

        
        if (value == 1) {
            KeyEvent ev;
            int valid_event = 1;

            if (code == KEY_SPACE) {
                ev = create_event(KEY_SPACE, 0, ctrl, shift, alt);
            } else if (code == KEY_ENTER || code == KEY_KPENTER) {
                ev = create_event(KEY_ENTER, 0, ctrl, shift, alt);
            } else if (code == KEY_BACKSPACE) {
                ev = create_event(KEY_BACKSPACE, 0, ctrl, shift, alt);
            } else if (code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT) {
                ev = create_event(KEY_SHIFT, 0, ctrl, shift, alt);
            } else if (code == KEY_ESC) {
                ev = create_event(KEY_ESCAPE, 0, ctrl, shift, alt);
                process_event(ev);
                close(fd);
                return INPUT_EXIT_REQUESTED;
            } else {
                char key = keycode_to_char(code, shift);

                if (key != 0) {
                    ev = create_event(KEY_NORMAL, key, ctrl, shift, alt);
                } else {
                    valid_event = 0;
                }

            }

            if (valid_event) {
                process_event(ev);
            }
        }
    }

    close(fd);
    return INPUT_ERROR;
}