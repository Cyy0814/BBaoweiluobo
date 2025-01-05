#include "CarrotState.h"
#include "Carrot.h"

//---------------------------------refactored with state pattern---------------------------------//
// 健康状态的状态转换
void HealthyState::handleHealthChange(Carrot* carrot, int health) {
    if (health == 9) {
        carrot->changeState(new SlightlyDamagedState());
    }
}

// 轻伤状态的状态转换
void SlightlyDamagedState::handleHealthChange(Carrot* carrot, int health) {
    if (health >= 10) {
        carrot->changeState(new HealthyState());
    }
    else if (health <= 8) {
        carrot->changeState(new ModeratelyDamagedState());
    }
}

// 中度受伤状态的状态转换
void ModeratelyDamagedState::handleHealthChange(Carrot* carrot, int health) {
    if (health == 9) {
        carrot->changeState(new SlightlyDamagedState());
    }
    else if (health <= 6) {
        carrot->changeState(new SeverelyDamagedState());
    }
}

// 严重受伤状态的状态转换
void SeverelyDamagedState::handleHealthChange(Carrot* carrot, int health) {
    if (health >= 7) {
        carrot->changeState(new ModeratelyDamagedState());
    }
    else if (health == 4) {
        carrot->changeState(new CriticalState());
    }
}

// 危急状态的状态转换
void CriticalState::handleHealthChange(Carrot* carrot, int health) {
    if (health >= 5) {
        carrot->changeState(new SeverelyDamagedState());
    }
    else if (health == 3) {
        carrot->changeState(new DyingState());
    }
}

// 濒死状态的状态转换
void DyingState::handleHealthChange(Carrot* carrot, int health) {
    if (health == 4) {
        carrot->changeState(new CriticalState());
    }
    else if (health == 2) {
        carrot->changeState(new NearDeathState());
    }
}

// 垂死状态的状态转换
void NearDeathState::handleHealthChange(Carrot* carrot, int health) {
    if (health == 3) {
        carrot->changeState(new DyingState());
    }
    else if (health == 1) {
        carrot->changeState(new LastBreathState());
    }
}

// 最后一口气状态的状态转换
void LastBreathState::handleHealthChange(Carrot* carrot, int health) {
    if (health == 2) {
        carrot->changeState(new NearDeathState());
    }
    else if (health <= 0) {
        // 可以在这里处理萝卜死亡的逻辑
    }
} 