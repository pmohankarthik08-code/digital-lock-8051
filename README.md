# digital-lock-8051
digital lock system using c with pin verification, security lockout ,and automated tests.
# Digital Lock System Using C

## Project Overview
This project demonstrates a password-based digital lock using C programming. It checks a four-digit PIN and grants or denies access.

## Features
- Four-digit PIN verification
- Correct PIN: 1234
- Five-second simulated unlock duration
- Three incorrect attempts trigger a ten-second lockout
- Automated tests for PIN verification and lockout logic

## Technologies Used
- C programming language
- Programiz Online C Compiler
- GitHub

## Project Files
- digital_lock.c - Main digital lock demonstration
- test_lock_logic.c - Automated test program
- README.md - Project documentation

## How to Run
1. Open digital_lock.c in an online C compiler.
2. Run the program.
3. Enter 1234 to test successful access.
4. Enter an incorrect PIN three times to test lockout.

To run the automated tests, open test_lock_logic.c in the online C compiler and run it.

## Limitations
This is a software-only prototype. It does not control a physical door or verify actual 8051 hardware timing.

## Future Improvements
- Implement 8051-specific firmware
- Simulate keypad and LCD hardware
- Automate tests using GitHub Actions
-
