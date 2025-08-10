#include "slime.h"

int maxHP(Element ele){
    int result;
    switch (ele){
        case GRASS:
            result = 110;
            break;
        case FIRE:
            result = 100;
            break;
        case WATER:
            result = 100;
            break;
        case ELECTRICITY:
            result = 90;
            break;
        case NORMAL:
            result = 120;
            break;
        default:
            assert(false && "Not a valid element and fail to get maxHP");
            break;
    }
    return result;
}

Slime::Slime(Element ele): elem(ele),
                            sleepRound(0), attackEffect(false),
                            tagged(false){
    switch (ele){
        case GRASS:
            name = "Green";
            HP = 110;
            ATK = 10;
            defence = 10;
            speed = 10;
            break;
        case FIRE:
            name = "Red";
            HP = 100;
            ATK = 12;
            defence = 10;
            speed = 11;
            break;
        case WATER:
            name = "Blue";
            HP = 100;
            ATK = 10;
            defence = 14;
            speed = 9;
            break;
        case ELECTRICITY:
            name = "Yellow";
            HP = 90;
            ATK = 10;
            defence = 10;
            speed = 12;
            break;
        case NORMAL:
            name = "Pink";
            HP = 120;
            ATK = 9;
            defence = 8;
            speed = 7;
            break;
        default:
            assert(false && "No element matched when initializing a slime.");
            break;
    }
}

std::string Slime::getName() const {
    return name;
}

Element Slime::getElem() const {
    return elem;
}

int Slime::getHP() const {
    return HP;
}

int Slime::getATK() const {
    return ATK;
}

int Slime::getDef() const {
    return defence;
}

int Slime::getSpeed() const{
    return speed;
}

bool Slime::isAlive() const {
    return HP > 0;
}

int Slime::getSleepRound() const {
    return sleepRound;
}

bool Slime::inEffect() const {
    return attackEffect;
}

bool Slime::hurt(int damage){
    bool killed = false;
    HP -= damage;
    // 睡眠回合数置零
    sleepRound = 0;
    if (HP <= 0){
        HP = 0;
        killed = true;
    }
    return killed;
}

void Slime::fallAsleep(int sleepTime){
    // 检验是否正在睡眠（若正在睡眠，不会改变睡眠回合数）
    if (sleepRound == 0){
        sleepRound = sleepTime;
    }
}

void Slime::getPainted(){
    if (!tagged){
        // 标记涂鸦状态
        tagged = true;
        // 涂鸦效果：防御力减半
        defence /= 2;
    }
}

bool Slime::isPainted() const {
    return tagged;
}


int Slime::heal(int value){
    int before = HP;
    HP += value;
    // 避免HP超过草元素的HP上限
    HP = (HP > maxHP(GRASS)) ? maxHP(GRASS) : HP;
    return HP - before;
}

void Slime::proceed(std::ostream& os, bool isPlayer){
    if (sleepRound == 1){
        os << (isPlayer ? "Your " : "Enemy's ");
        os << name << " wakes up" << std::endl;
    }
    if (sleepRound > 0){
        sleepRound--;
    }
}
