# Keylogger
## Overview
Keylogger is a C-based cybersecurity and typing-analysis project developed as part of a cybersecurity course.

The project demonstrates how keyboard events can be received through operating-system-specific interfaces, processed into readable key events, recorded with timestamps, analyzed for typing statistics, and encrypted using a custom XOR-based cipher.

The project is intended strictly for educational purposes, authorized security testing, and local input diagnostics.

Ethical Use Notice: This software should only be executed on systems and with keyboard input for which the tester has explicit authorization. Do not use it to monitor another person's activity or capture passwords, credentials, or other sensitive information.

## Features
Local background keyboard event monitoring

Timestamped keystroke logging

Human-readable representation of special keys:

[SPACE]
[ENTER]
[BACKSPACE]
[SHIFT]

Escape key support for clean session termination

Typing-session statistics:
  Total keystrokes
  Active runtime
  Average Words Per Minute (WPM)
  Backspace error rate

Top 5 most frequently pressed keys

Custom XOR-based file encryption

###Platform-specific keyboard input implementations for:

Windows

Linux

macOS

## Generated diagnostic files excluded from version control

## Project Structure
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

## Components
### mainn.c
Controls the overall application lifecycle.

It:
Initializes the logging session
Starts keyboard event monitoring
Handles session termination
Calculates or triggers session processing
Performs log encryption after the session ends

###logger.c / logger.h
Responsible for session logging and typing analysis.

Functions include:
Session initialization
Timestamped keystroke logging
Keystroke frequency tracking
WPM calculation
Backspace error-rate calculation
Top-key analysis
Buffer flushing
Session cleanup

### input.c / input.h
Provides the common input-processing layer shared by the platform-specific implementations.

It defines the common KeyEvent representation and provides a consistent interface for processing keyboard events.

platform/
Contains operating-system-specific implementations for receiving keyboard events.

input_windows.c  → Windows
input_linux.c    → Linux
input_macos.c    → macOS

The platform abstraction allows the rest of the application to use a common input interface.

###cipher.c / cipher.h
Implements the project's custom XOR-based encryption mechanism for the diagnostic log.

The encryption logic is implemented directly in C for educational purposes.

### Typing Statistics
The program calculates several metrics at the end of a typing session.

Words Per Minute
WPM is estimated using the standard typing-analysis approximation:

WPM = (characters / 5) / minutes

The calculation uses the number of characters recorded during the active session.

### Backspace Error Rate
The project estimates the typing error rate using the number of backspace events:

Error Rate = (backspaces / characters) × 100

This provides an approximate measure of corrections made during the session.

### Top Keys
The program maintains frequency counters for keyboard events and displays the five most frequently pressed keys at the end of the session.

## Encryption
After a session ends, the diagnostic log is encrypted using the project's XOR-based encryption implementation.

The workflow is:

Keyboard Input
      ↓
Event Processing
      ↓
Diagnostic Log
      ↓
Typing Statistics
      ↓
XOR Encryption
      ↓
Encrypted Log

The generated encrypted file is: enc_keystrokes.log

The original plaintext log is removed after successful encryption.

Security Note: XOR encryption with a static key is implemented for educational purposes only. It is not considered secure modern cryptography and should not be used to protect sensitive information in real-world applications.

## Building
### Windows
The Windows implementation uses the Windows low-level keyboard hook API.

Compile the common source files together with the Windows-specific implementation using a suitable C compiler such as MinGW or Visual Studio.

Example source files:

mainn.c
logger.c
input.c
cipher.c
platform/input_platform.c
platform/input_windows.c

### Linux
The Linux implementation uses the Linux input-event interface.

Compile with:

gcc mainn.c logger.c input.c cipher.c \
    platform/input_platform.c platform/input_linux.c \
    -o keylogger

Run:

sudo ./keylogger

Access to /dev/input devices may require elevated permissions depending on the system configuration.

### macOS
The macOS implementation uses the macOS Quartz/Application Services event-tap API.

Compilation must be performed on macOS with the appropriate system frameworks and permissions.

## Usage
Build the project for the target operating system.
Start the program in an authorized testing environment.
The program begins receiving keyboard events.
Keyboard events are processed and recorded with timestamps.
Special keys are represented using readable labels such as [SPACE], [ENTER], and [BACKSPACE].
Use normal test input to generate keyboard events.
Press Escape to terminate the monitoring session.
The program closes the logging session and calculates typing statistics.
The diagnostic log is encrypted after the session ends.
The plaintext log is removed after successful encryption.

## Safe Testing
For testing:
Use only your own keyboard input.
Do not enter passwords or authentication credentials while the program is running.
Do not monitor another person's keyboard activity without explicit authorization.

## Platform Testing

Windows: Tested
Linux: Testing in an Ubuntu virtual machine
macOS: Implementation included; not tested on macOS hardware

## Security and Ethical Scope

Keyboard-monitoring software can potentially capture highly sensitive information. Therefore, this project is intended only for educational use, authorized security testing, and local input diagnostics.

The developer does not intend for the software to be used for unauthorized surveillance, credential theft, privacy violations, or other malicious activities.

Users are responsible for ensuring that their use of the software complies with applicable laws, institutional policies, and authorization requirements.

## Generated Files
The following files may be generated during execution:

keystrokes.log
enc_keystrokes.log
decrypted.log

These files may contain sensitive keyboard input and are therefore excluded from version control using .gitignore.

Generated logs should never be committed to the repository.

## Technologies
C
Linux input-event interface
Windows low-level keyboard hooks
macOS Quartz/Application Services event taps
Standard C libraries
Custom XOR file encryption
Platform-specific C APIs

## Learning Objectives
This project was developed to explore:
Low-level keyboard event handling
Cross-platform system programming in C
Modular C project architecture
Header/source-file organization
File I/O and buffered logging
Runtime statistics and data analysis
Basic encryption concepts
Platform abstraction
Secure and ethical cybersecurity development

## Disclaimer
This project is provided for educational and authorized testing purposes only. Do not use it to monitor systems, accounts, or individuals without explicit permission. Never use it to capture passwords, authentication tokens, financial information, or other sensitive data.
