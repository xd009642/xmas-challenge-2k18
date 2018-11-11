#include "generic_lock.h"

#include <iostream>

glp::generic_lock::generic_lock(std::function<void()> unlock) noexcept:
    lock_fn({}),
    unlock_fn(unlock) {
}

glp::generic_lock::generic_lock(std::function<void()> lock,
        std::function<void()> unlock):
    lock_fn(lock),
    unlock_fn(unlock) {
        lock();
}

glp::generic_lock::~generic_lock() {
    unlock_fn();
}
