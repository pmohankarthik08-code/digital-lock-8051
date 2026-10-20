Digital Lock System

A software-based digital lock project developed to demonstrate password verification, access control, and security testing.

Project Components

1. 8051 Embedded C Firmware

- File: "firmware/digital_lock_8051.c"
- Target: 8051 microcontroller
- Password verification through UART serial input
- Access granted and wrong-password indications using LEDs
- Lockout after three incorrect attempts

Note: The 8051 firmware has not yet been verified on an actual 8051 compiler or simulator.

2. Arduino Digital Safe Simulation

- File: "electronic-safe.ino"
- Simulation: Wokwi Arduino Uno
- Components: 4×4 keypad, LCD, and servo motor
- Demonstrates password entry and electronic lock control

The Arduino simulation is separate from the 8051 firmware.

3. C Logic and Automated Tests

- "digital_lock.c" — digital lock logic
- "test_lock_logic.c" — automated tests
- ".github/workflows/ci.yml" — automated testing with GitHub Actions

Technologies Used

- Embedded C / C
- 8051 microcontroller firmware concepts
- Arduino and Wokwi simulation
- GitHub and GitHub Actions

Repository Structure

digital-lock-8051/
├── .github/
│   └── workflows/
│       └── ci.yml
├── firmware/
│   └── digital_lock_8051.c
├── digital_lock.c
├── test_lock_logic.c
├── electronic-safe.ino
├── diagram.json
├── SafeState.cpp
├── SafeState.h
├── icons.cpp
├── icons.h
├── libraries.txt
├── wokwi-project.txt
└── README.md

How to Test

1. Review the C logic and tests in the repository.
2. Check the GitHub Actions tab for automated test results.
3. Open the Wokwi Arduino project to explore the visual simulation.
4. Compile and validate the 8051 firmware with a compatible 8051 toolchain before claiming hardware-level verification.

Future Improvements

- Add a real 8051-compatible simulation or compiler test.
- Implement keypad input directly on the 8051.
- Add a resettable lockout and configurable password.
- Improve firmware and test documentation.
