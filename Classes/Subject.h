#ifndef __SUBJECT_H__
#define __SUBJECT_H__

#include <vector>
#include "Observer.h"

// Refactored with Observer Pattern
class Subject {
protected:
    std::vector<Observer*> observers;
    
public:
    virtual void addObserver(Observer* observer) {
        observers.push_back(observer);
    }
    
    virtual void removeObserver(Observer* observer) {
        auto it = std::find(observers.begin(), observers.end(), observer);
        if(it != observers.end()) {
            observers.erase(it);
        }
    }
    
    virtual void notifyObservers(const std::string& message, int value) {
        for(auto observer : observers) {
            observer->update(message, value);
        }
    }
    
    virtual ~Subject() {}
};

#endif
