#include "skill.h"

// 获取技能名属性
Element elemOfSkill(SkillName skillName){
    Element result;
    switch (skillName){
        case Tackle:
            result = NORMAL;
            break;
        case Claw:
            result = NORMAL;
            break;
        case Quick:
            result = NORMAL;
            break;
        case Slap:
            result = NORMAL;
            break;
        case Sing:
            result = NORMAL;
            break;
        case Paint:
            result = NORMAL;
            break;
        case Leaf:
            result = GRASS;
            break;
        case Synthesis:
            result = GRASS;
            break;
        case Flame:
            result = FIRE;
            break;
        case Sunny:
            result = FIRE;
            break;
        case Stream:
            result = WATER;
            break;
        case Rainy:
            result = WATER;
            break;
        case Volt:
            result = ELECTRICITY;
            break;
        case Thunder:
            result = ELECTRICITY;
            break;
        default:
            assert(false && "Not a valid element of skill.");
            break;
    }
    return result;
}

// 获取伤害倍率
Multiplier getMul(SkillName skillName, const Slime& target){
    Element sElem = elemOfSkill(skillName);
    if ((sElem == GRASS && target.getElem() == WATER) ||
        (sElem == FIRE && target.getElem() == GRASS) ||
        (sElem == WATER && target.getElem() == FIRE) ||
        (sElem == ELECTRICITY && target.getElem() == WATER)){
        return TWO;
    }else if (sElem == NORMAL || target.getElem() == NORMAL ||
              (sElem != ELECTRICITY && target.getElem() == ELECTRICITY) ||
              (sElem == ELECTRICITY && target.getElem() == FIRE)){
        return ONE;
    }
    return HALF;
}

// 将小数四舍五入取整的函数,要求 x > 0
int toInt(double x){
    if (x <= 0) {return -1;}
    return int(x + 0.5);
}

// 根据技能获取基础的技能威力
int basicPow(SkillName skillName){
    switch (skillName){
        case Tackle:
            return 20;
        case Claw:
            return 15;
        case Quick:
            return 15;
        case Slap:
            return 15;
        case Leaf:
            return 20;
        case Flame:
            return 20;
        case Stream:
            return 20;
        case Volt:
            return 25;
        case Thunder:
            return 50;
        default:
            assert(false && "The skill is not to attack and fail to get basic power.");
    }
    return 0;
}

// 攻击类技能的伤害计算函数
int calDamage(const Slime& user, const Slime& target, SkillName skillName, const BattleField battleField){
    double damage;
    int power = basicPow(skillName);
    switch (getMul(skillName, target)){
        case HALF:
            damage = 0.5 * power * user.getATK() / target.getDef();
            break;
        case ONE:
            damage = 1.0 * power * user.getATK() / target.getDef();
            break;
        case TWO:
            damage = 2.0 * power * user.getATK() / target.getDef();
            break;
        default:
            assert(false && "No matched multiplier and fail to calculate damage.");
    }
    // 天气加成
    if (battleField.getWeather() == SUNNY){
        if (skillName == Flame){
            damage *= 1.5;
        }else if (skillName == Stream){
            damage *= 0.5;
        }
    }else if (battleField.getWeather() == RAINY){
        if (skillName == Flame){
            damage *= 0.5;
        }else if (skillName == Stream){
            damage *= 1.5;
        }
    }

    // 拍打技能对睡眠状态的加成
    if (skillName == Slap && target.getSleepRound() > 0){
        damage *= 2;
    }

    return toInt(damage);
}


int priority(SkillName skillName){
    // 在当前游戏版本中只有三个技能的优先级是1
    if (skillName == Quick || skillName == Sunny || skillName == Rainy){
        return 1;
    }
    return 0;
}


bool Skill::activate(Slime& user, Slime& target, SkillName skillName, BattleField& battleField, std::ostream& os, bool fromPlayer, bool prior){
    // 技能是否发动成功
    bool succeed = true;
    std::string owner = fromPlayer ? "Your " : "Enemy's ";
    // 若在睡觉，直接打印史莱姆正在睡觉
    if (user.getSleepRound() > 0){
        os << owner << user.getName() << " is sleeping" << std::endl;
        return false;
    }
    // 未在睡觉，发动技能
    switch (skillName){
        case Tackle:
            Skill::activateTackle(user, target, battleField, os, fromPlayer);
            break;
        case Claw:
            Skill::activateClaw(user, target, battleField, os, fromPlayer);
            break;
        case Quick:
            Skill::activateQuick(user, target, battleField, os, fromPlayer);
            break;
        case Slap:
            Skill::activateSlap(user, target, battleField, os, fromPlayer);
            break;
        case Sing:
            Skill::activateSing(user, target, os, fromPlayer);
            break;
        case Paint:
            succeed = Skill::activatePaint(user, target, os, fromPlayer, prior);
            break;
        case Leaf:
            Skill::activateLeaf(user, target, battleField, os, fromPlayer);
            break;
        case Synthesis:
            Skill::activateSynthesis(user, battleField, os, fromPlayer);
            break;
        case Flame:
            Skill::activateFlame(user, target, battleField, os, fromPlayer);
            break;
        case Sunny:
            Skill::activateSunny(user, battleField, os, fromPlayer);
            break;
        case Stream:
            Skill::activateStream(user, target, battleField, os, fromPlayer);
            break;
        case Rainy:
            Skill::activateRainy(user, battleField, os, fromPlayer);
            break;
        case Volt:
            Skill::activateVolt(user, target, battleField, os, fromPlayer);
            break;
        case Thunder:
            succeed = Skill::activateThunder(user, target, battleField, os, fromPlayer);
            break;
        default:
            assert(false && "No skill matched and fail to activate the skill.");
            break;
    }
    return succeed;
}

void Skill::activateTackle(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer){
    bool inSleep = target.getSleepRound() > 0;
    std::string owner = fromPlayer ? "Your " : "Enemy's ",
                 other = fromPlayer ? "Enemy's " : "Your ";
    int damage = calDamage(user, target, Tackle, battleField);
    os << owner << user.getName() << " uses Tackle! Damage: " << damage << std::endl;
    bool killed = target.hurt(damage);
    if (killed){
        os << other << target.getName() << " is beaten" << std::endl;
    }else if (inSleep){
        os << other << target.getName() << " wakes up" << std::endl;
    }
}

void Skill::activateClaw(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer){
    bool inSleep = target.getSleepRound() > 0;
    std::string owner = fromPlayer ? "Your " : "Enemy's ",
                 other = fromPlayer ? "Enemy's " : "Your ";
    int damage = calDamage(user, target, Claw, battleField);
    os << owner << user.getName() << " uses Claw! Damage: " << damage << std::endl;
    bool killed = target.hurt(damage);
    if (killed){
        os << other << target.getName() << " is beaten" << std::endl;
    }else if (inSleep){
        os << other << target.getName() << " wakes up" << std::endl;
    }
}

void Skill::activateQuick(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer){
    bool inSleep = target.getSleepRound() > 0;
    std::string owner = fromPlayer ? "Your " : "Enemy's ",
                 other = fromPlayer ? "Enemy's " : "Your ";
    int damage = calDamage(user, target, Quick, battleField);
    os << owner << user.getName() << " uses Quick! Damage: " << damage << std::endl;
    bool killed = target.hurt(damage);
    if (killed){
        os << other << target.getName() << " is beaten" << std::endl;
    }else if (inSleep){
        os << other << target.getName() << " wakes up" << std::endl;
    }
}

void Skill::activateSlap(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer){
    bool inSleep = target.getSleepRound() > 0;
    std::string owner = fromPlayer ? "Your " : "Enemy's ",
                 other = fromPlayer ? "Enemy's " : "Your ";
    int damage = calDamage(user, target, Slap, battleField);
    os << owner << user.getName() << " uses Slap! Damage: " << damage << std::endl;
    bool killed = target.hurt(damage);
    if (killed){
        os << other << target.getName() << " is beaten" << std::endl;
    }else if (inSleep){
        os << other << target.getName() << " wakes up" << std::endl;
    }
}

void Skill::activateSing(Slime& user, Slime& target, std::ostream& os, bool fromPlayer){
    bool inSleep = target.getSleepRound() > 0;
    std::string owner = fromPlayer ? "Your " : "Enemy's ",
                 other = fromPlayer ? "Enemy's " : "Your ";
    os << owner << user.getName() << " uses Sing! ";
    if (inSleep){
        os << "Waste" << std::endl;
    }else{
        target.fallAsleep(3);
        os << other << target.getName() << " falls asleep" << std::endl;
    }
}

bool Skill::activatePaint(Slime& user, Slime& target, std::ostream& os, bool fromPlayer, bool prior){
    bool succeed = true;
    std::string owner = fromPlayer ? "Your " : "Enemy's ",
                 other = fromPlayer ? "Enemy's " : "Your ";
    os << owner << user.getName() << " uses Paint! ";
    if (prior){
        target.getPainted();
        os << other << target.getName() << " is painted" << std::endl;
    }else{
        os << "Failure" << std::endl;
        succeed = false;
    }
    return succeed;
}

void Skill::activateLeaf(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer){
    bool inSleep = target.getSleepRound() > 0;
    std::string owner = fromPlayer ? "Your " : "Enemy's ",
                 other = fromPlayer ? "Enemy's " : "Your ";
    int damage = calDamage(user, target, Leaf, battleField);
    os << owner << user.getName() << " uses Leaf! Damage: " << damage << std::endl;
    bool killed = target.hurt(damage);
    if (killed){
        os << other << target.getName() << " is beaten" << std::endl;
    }else if (inSleep){
        os << other << target.getName() << " wakes up" << std::endl;
    }
}

void Skill::activateSynthesis(Slime& user, const BattleField& battleField, std::ostream& os, bool fromPlayer){
    std::string owner = fromPlayer ? "Your " : "Enemy's ";
    // 回血量
    int recovery;
    Weather curWeather = battleField.getWeather();
    if (curWeather == SUNNY){
        recovery = int(110 * 0.4);
    }else if (curWeather == RAINY){
        recovery = int(110 * 0.2);
    }else{
        recovery = int(110 * 0.3);
    }
    user.heal(recovery);
    os << owner << user.getName() << " uses Synthesis! Recovery: " << recovery << std::endl;
}

void Skill::activateFlame(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer){
    bool inSleep = target.getSleepRound() > 0;
    std::string owner = fromPlayer ? "Your " : "Enemy's ",
                 other = fromPlayer ? "Enemy's " : "Your ";
    int damage = calDamage(user, target, Flame, battleField);
    os << owner << user.getName() << " uses Flame! Damage: " << damage << std::endl;
    bool killed = target.hurt(damage);
    if (killed){
        os << other << target.getName() << " is beaten" << std::endl;
    }else if (inSleep){
        os << other << target.getName() << " wakes up" << std::endl;
    }
}

void Skill::activateSunny(Slime& user, BattleField& battleField, std::ostream& os, bool fromPlayer){
    std::string owner = fromPlayer ? "Your " : "Enemy's ";
    battleField.setWeather(SUNNY, 5);
    os << owner << user.getName() << " uses Sunny! Weather becomes sunny" << std::endl;
}


void Skill::activateStream(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer){
    bool inSleep = target.getSleepRound() > 0;
    std::string owner = fromPlayer ? "Your " : "Enemy's ",
                 other = fromPlayer ? "Enemy's " : "Your ";
    int damage = calDamage(user, target, Stream, battleField);
    os << owner << user.getName() << " uses Stream! Damage: " << damage << std::endl;
    bool killed = target.hurt(damage);
    if (killed){
        os << other << target.getName() << " is beaten" << std::endl;
    }else if (inSleep){
        os << other << target.getName() << " wakes up" << std::endl;
    }
}


void Skill::activateRainy(Slime& user, BattleField& battleField, std::ostream& os, bool fromPlayer){
    std::string owner = fromPlayer ? "Your " : "Enemy's ";
    battleField.setWeather(RAINY, 5);
    os << owner << user.getName() << " uses Rainy! Weather becomes rainy" << std::endl;
}


void Skill::activateVolt(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer){
    bool inSleep = target.getSleepRound() > 0;
    std::string owner = fromPlayer ? "Your " : "Enemy's ",
                 other = fromPlayer ? "Enemy's " : "Your ";
    int damage = calDamage(user, target, Volt, battleField);
    os << owner << user.getName() << " uses Volt! Damage: " << damage << std::endl;
    bool killed = target.hurt(damage);
    if (killed){
        os << other << target.getName() << " is beaten" << std::endl;
    }else if (inSleep){
        os << other << target.getName() << " wakes up" << std::endl;
    }
}


bool Skill::activateThunder(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer){
    bool inSleep = target.getSleepRound() > 0;
    std::string owner = fromPlayer ? "Your " : "Enemy's ",
                 other = fromPlayer ? "Enemy's " : "Your ";
    bool succeed = (battleField.getWeather() == RAINY);
    os << owner << user.getName() << " uses Thunder! ";
    // 判定天气是否为雨天，只有雨天时技能发动成功
    if (succeed){
        int damage = calDamage(user, target, Thunder, battleField);
        bool killed = target.hurt(damage);
        os << "Damage: " << damage << std::endl;
        if (killed){
            os << other << target.getName() << " is beaten" << std::endl;
        }else if (inSleep){
            os << other << target.getName() << " wakes up" << std::endl;
        }
    }else{
        os << "Failure" << std::endl;
    }
    return succeed;
}

