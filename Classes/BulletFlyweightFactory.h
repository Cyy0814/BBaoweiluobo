#ifndef __BULLET_FLYWEIGHT_FACTORY_H__
#define __BULLET_FLYWEIGHT_FACTORY_H__

#include <unordered_map>
#include <string>
#include "Flyweight.h"
#include "Bullet.h"

//----------------------------refactored with flyweight----------------------------//
class BulletFlyweightFactory {
public:
    static Flyweight* getBullet(const std::string& texture, float speed, int damage);
    static void cleanup() {
        for (auto& pair : bulletPool) {
            delete pair.second;
        }
        bulletPool.clear();
    }

private:
    static std::unordered_map<std::string, Flyweight*> bulletPool;
};

#endif // __BULLET_FLYWEIGHT_FACTORY_H__ 