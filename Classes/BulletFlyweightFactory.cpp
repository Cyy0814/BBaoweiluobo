#include "BulletFlyweightFactory.h"

std::unordered_map<std::string, Flyweight*> BulletFlyweightFactory::bulletPool;

//----------------------------refactored with flyweight----------------------------//
Flyweight* BulletFlyweightFactory::getBullet(const std::string& texture, float speed, int damage) {
    auto it = bulletPool.find(texture);
    if (it != bulletPool.end()) {
        return it->second;
    } else {
        Bullet* newBullet = new Bullet(texture);
        newBullet->initOptions(nullptr, speed, damage);
        bulletPool[texture] = newBullet;
        return newBullet;
    }
} 