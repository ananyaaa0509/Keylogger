#include <stdio.h>
#include <ApplicationServices/ApplicationServices.h>
#include "input_macos.h"
#include "logger.h"

static CFMachPortRef event_tap = NULL;
static CFRunLoopSourceRef run_loop_source = NULL;

static CGEventRef low_level_event_callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) {
    if (type == kCGEventKeyDown) {
        CGKeyCode vk = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        
        CGEventFlags flags = CGEventGetFlags(event);
        int ctrl  = (flags & kCGEventFlagMaskControl) != 0;
        int shift = (flags & kCGEventFlagMaskShift) != 0;
        int alt   = (flags & kCGEventFlagMaskAlternate) != 0;

        KeyEvent ev;
        int valid_event = 1;

        if (vk == 49) {
            ev = create_event(KEY_SPACE, 0, ctrl, shift, alt);
        }
        else if (vk == 36) {
            ev = create_event(KEY_ENTER, 0, ctrl, shift, alt);
        }
        else if (vk == 51) {
            ev = create_event(KEY_BACKSPACE, 0, ctrl, shift, alt);
        }
        else if (vk == 56 || vk == 60) {
            ev = create_event(KEY_SHIFT, 0, ctrl, shift, alt);
        }
        else if (vk == 53) {
            ev = create_event(KEY_ESCAPE, 0, ctrl, shift, alt);
            
            process_event(ev);
            valid_event = 0;

            CFRunLoopStop(CFRunLoopGetCurrent());
        }
        else {
            UniChar unicode_buffer[4];
            UniCharCount actual_length = 0;
            
            CGEventKeyboardGetUnicodeString(event, 4, &actual_length, unicode_buffer);

            if (actual_length > 0 && unicode_buffer[0] >= 32 && unicode_buffer[0] <= 126) {
                char key = (char)unicode_buffer[0];
                ev = create_event(KEY_NORMAL, key, ctrl, shift, alt);
            } else {
                valid_event = 0;
            }
        }

        if (valid_event) {
            process_event(ev);
        }
    }
    
    return event; 
}

InputResult macos_start_platform_input(void) {
    event_tap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionListenOnly,
        CGEventMaskBit(kCGEventKeyDown),
        low_level_event_callback,
        NULL
    );

    if (!event_tap) {
        return INPUT_ERROR;
    }

    run_loop_source = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event_tap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), run_loop_source, kCFRunLoopCommonModes);
    
    CGEventTapEnable(event_tap, true);

    CFRunLoopRun();

    CGEventTapEnable(event_tap, false);
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(), run_loop_source, kCFRunLoopCommonModes);
    CFRelease(run_loop_source);
    CFRelease(event_tap);
    event_tap = NULL;

    return INPUT_EXIT_REQUESTED;
}