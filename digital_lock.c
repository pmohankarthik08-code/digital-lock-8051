#include <stdio.h>
#include <string.h>
#include <time.h>

#define CORRECT_PIN "1234"

typedef struct {
    int failed_attempts;
    time_t locked_until;
    time_t unlocked_until;
} DigitalLock;

enum {
    ACCESS_DENIED = 0,
    ACCESS_GRANTED = 1,
    SYSTEM_LOCKED = 2
};

void init_lock(DigitalLock *lock) {
    lock->failed_attempts = 0;
    lock->locked_until = 0;
    lock->unlocked_until = 0;
}

int submit_pin(DigitalLock *lock, const char *pin,
               time_t now) {
    if (now < lock->locked_until)
        return SYSTEM_LOCKED;

    if (strcmp(pin, CORRECT_PIN) == 0) {
        lock->failed_attempts = 0;
        lock->unlocked_until = now + 5;
        return ACCESS_GRANTED;
    }

    lock->failed_attempts++;

    if (lock->failed_attempts >= 3) {
        lock->locked_until = now + 10;
        lock->failed_attempts = 0;
        return SYSTEM_LOCKED;
    }

    return ACCESS_DENIED;
}

void wait_seconds(int seconds) {
    time_t start = time(NULL);

    while (difftime(time(NULL), start) < seconds) {
        /* Wait for the requested duration. */
    }
}

/* Automated tests */
void run_tests(void) {
    DigitalLock lock;
    int passed = 0;
    int total = 5;

    init_lock(&lock);

    if (submit_pin(&lock, "1234", 100) == ACCESS_GRANTED)
        passed++;
    printf("Test 1: Correct PIN - %s\n",
           passed == 1 ? "PASS" : "CHECK");

    init_lock(&lock);
    int result = submit_pin(&lock, "1111", 100);
    if (result == ACCESS_DENIED &&
        lock.failed_attempts == 1)
        passed++;
    printf("Test 2: Incorrect PIN - %s\n",
           result == ACCESS_DENIED &&
           lock.failed_attempts == 1 ? "PASS" : "FAIL");

    init_lock(&lock);
    submit_pin(&lock, "1111", 100);
    submit_pin(&lock, "1111", 101);
    result = submit_pin(&lock, "1111", 102);
    if (result == SYSTEM_LOCKED &&
        lock.locked_until == 112)
        passed++;
    printf("Test 3: Three failures trigger lockout - %s\n",
           result == SYSTEM_LOCKED &&
           lock.locked_until == 112 ? "PASS" : "FAIL");

    result = submit_pin(&lock, "1234", 105);
    if (result == SYSTEM_LOCKED)
        passed++;
    printf("Test 4: Reject PIN during lockout - %s\n",
           result == SYSTEM_LOCKED ? "PASS" : "FAIL");

    result = submit_pin(&lock, "1234", 112);
    if (result == ACCESS_GRANTED &&
        lock.unlocked_until == 117)
        passed++;
    printf("Test 5: Accept PIN after lockout - %s\n",
           result == ACCESS_GRANTED &&
           lock.unlocked_until == 117 ? "PASS" : "FAIL");

    printf("\nAutomated tests: %d/%d passed\n",
           passed, total);
}

int main(void) {
    DigitalLock lock;
    char pin[20];

    run_tests();

    init_lock(&lock);

    printf("\n=== DIGITAL LOCK DEMO ===\n");

    while (1) {
        time_t now = time(NULL);

        if (now < lock.locked_until) {
            int remaining = (int)difftime(
                lock.locked_until, now);
            printf("System locked. Wait %d seconds.\n",
                   remaining);
            wait_seconds(remaining);
            continue;
        }

        printf("\nEnter PIN (type exit to quit): ");

        if (scanf("%19s", pin) != 1)
            break;

        if (strcmp(pin, "exit") == 0)
            break;

        if (strlen(pin) != 4) {
            printf("Enter exactly 4 characters.\n");
            continue;
        }

        int result = submit_pin(&lock, pin, time(NULL));

        if (result == ACCESS_GRANTED) {
            printf("Access Granted! Door Unlocked.\n");
            printf("Unlock duration: 5 seconds.\n");
            wait_seconds(5);
            printf("Door Locked Again.\n");
        } else if (result == SYSTEM_LOCKED) {
            printf("Too many failures. Locked for 10 seconds.\n");
            wait_seconds(10);
            printf("Lockout ended.\n");
        } else {
            printf("Incorrect PIN! Attempts left: %d\n",
                   3 - lock.failed_attempts);
        }
    }

    printf("Digital Lock program ended.\n");
    return 0;
}
