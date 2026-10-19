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

int main(void) {
    DigitalLock lock;
    int passed = 0;

    init_lock(&lock);
    if (submit_pin(&lock, "1234", 100) == ACCESS_GRANTED)
        passed++;
    else
        printf("FAIL: Correct PIN\n");

    init_lock(&lock);
    if (submit_pin(&lock, "1111", 100) == ACCESS_DENIED)
        passed++;
    else
        printf("FAIL: Incorrect PIN\n");

    init_lock(&lock);
    submit_pin(&lock, "1111", 100);
    submit_pin(&lock, "1111", 101);
    if (submit_pin(&lock, "1111", 102) == SYSTEM_LOCKED &&
        lock.locked_until == 112)
        passed++;
    else
        printf("FAIL: Three-attempt lockout\n");

    if (submit_pin(&lock, "1234", 105) == SYSTEM_LOCKED)
        passed++;
    else
        printf("FAIL: Reject during lockout\n");

    if (submit_pin(&lock, "1234", 112) == ACCESS_GRANTED)
        passed++;
    else
        printf("FAIL: Accept after lockout\n");

    printf("Automated tests passed: %d/5\n", passed);

    return passed == 5 ? 0 : 1;
}
