Keylogger — C Cybersecurity Project
Overview
This project is a C-based local keystroke monitoring and typing-analysis program developed for a cybersecurity course project.

The program listens for keyboard events at the operating-system level, converts them into readable key events, records them with timestamps, calculates typing statistics, and encrypts the resulting diagnostic log using a custom XOR-based cipher.

The project is intended for local input diagnostics, typing analysis, and educational cybersecurity use.

Features
Local background keyboard event monitoring

Timestamped keystroke logging

Human-readable special-key formatting:

[SPACE]

[ENTER]

[BACKSPACE]

[SHIFT]

Escape key for clean session termination

Session statistics:

Total keystrokes

Active runtime

Average WPM

Backspace error rate

Top-5 most frequently pressed keys

Custom XOR-based file encryption

Platform-specific input implementations for:

Windows

Linux

macOS

Project Structure
.
├── mainn.c
├── input.c
├── input.h
├── logger.c
├── logger.h
├── cipher.c
├── cipher.h
└── platform/
    ├── input_platform.c
    ├── input_platform.h
    ├── input_windows.c
    ├── input_windows.h
    ├── input_linux.c
    ├── input_linux.h
    ├── input_macos.c
    └── input_macos.h

Components
mainn.c

Controls the overall session lifecycle. It starts logging, starts keyboard input monitoring, ends the session, and performs log encryption.

logger.c / logger.h

Handles session initialization, timestamped logging, keystroke statistics, WPM calculation, error-rate calculation, top-key analysis, buffer flushing, and session cleanup.

input.c / input.h

Provides the common input-processing layer and KeyEvent representation used by the platform-specific implementations.

platform/

Contains operating-system-specific keyboard event handling.

cipher.c / cipher.h

Implements the custom XOR-based encryption of the diagnostic log.

Statistics
The program calculates several typing-analysis metrics.

Words Per Minute
WPM is estimated using the standard typing-analysis approximation:

WPM = (characters / 5) / minutes

Backspace Error Rate
The backspace rate is calculated as:

Error Rate = (backspaces / characters) × 100

Top Keys
The program maintains key-frequency counters and displays the five most frequently pressed keys at the end of the session.

Encryption
After a session ends, the program encrypts keystrokes.log into:

enc_keystrokes.log

The project implements the XOR operation directly in C rather than relying on a pre-built encryption package.

The original plaintext log is removed after successful encryption.

Note: XOR with a static key is implemented for educational purposes and should not be considered secure modern cryptography.

Building
Linux
The Linux implementation uses the Linux input-event interface.

Compile with:

gcc mainn.c logger.c input.c cipher.c \
    platform/input_platform.c platform/input_linux.c \
    -o keylogger

Run:

sudo ./keylogger

Access to /dev/input devices may require elevated permissions depending on the system configuration.

Windows
The Windows implementation uses the Windows low-level keyboard hook API.

Compile the Windows-specific source files together with the common source files using a suitable C compiler such as MinGW or Visual Studio.

macOS
The macOS implementation uses the macOS Quartz/Application Services event-tap API.

Compilation requires the appropriate macOS frameworks and must be performed on macOS.

Usage
Start the program.

The program begins monitoring keyboard events in the background.

Keyboard activity is recorded with timestamps and translated into readable key names where applicable.

During testing, use normal keys and supported special keys such as Space, Enter, Backspace, and Shift.

Press Escape to stop the monitoring session.

The program flushes and closes the log, calculates session statistics, and displays the results.

After the session ends, the diagnostic log is encrypted and the plaintext log is removed after successful encryption.

For testing, use only your own keyboard input and avoid entering passwords or other sensitive information.

Do not use the program to monitor other people's keyboard input without their knowledge and authorization.

Platform Testing
Windows: Tested

Linux: Being tested in an Ubuntu virtual machine

macOS: Platform implementation included; not tested on macOS hardware

Security and Ethical Scope
This project is designed as an educational cybersecurity exercise and for authorized local input diagnostics.

Keyboard-monitoring software can capture sensitive information. Therefore, testing should only be performed on systems and accounts for which the tester has explicit authorization.

Generated logs may contain sensitive keyboard input and should not be committed to version control.

Generated Files
The following files are generated during execution and are excluded from Git:

keystrokes.log
enc_keystrokes.log
decrypted.log

Technologies
C

Linux input-event interface

Windows low-level keyboard hooks

macOS Quartz/Application Services event taps

Custom XOR file encryption

Standard C libraries