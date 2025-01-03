#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <string>

// Refactored with Observer Pattern
class Observer {
public:
    virtual void update(const std::string& message, int value) = 0;
    virtual ~Observer() {}
};

#endif
