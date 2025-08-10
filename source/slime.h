#ifndef SLIME_H_INCLUDED
#define SLIME_H_INCLUDED

#include <string>
#include <iostream>
#include <cassert>

// 元素属性
enum Element{NORMAL, GRASS, FIRE, WATER, ELECTRICITY};

// 获取属性对应最大HP
int maxHP(Element ele);

// 获取克制ele的元素类型 (待实现）
Element counterElem(Element ele);

class Slime{
public:
    // 构造函数
    Slime(Element ele);

    // 获取名字
    std::string getName() const;

    // 获取元素属性
    Element getElem() const;

    // 获取生命值
    int getHP() const;

    // 获取攻击力
    int getATK() const;

    // 获取防御力
    int getDef() const;

    // 获取速度
    int getSpeed() const;

    // 获取存活状态
    bool isAlive() const;

    // 获取睡眠回合数
    int getSleepRound() const;

    // 获取强攻药生效状态
    bool inEffect() const;

    // 受伤扣除生命值，睡眠回合数置零，返回true表示HP已降低为0
    bool hurt(int damage);

    // 入睡，只有未处于睡眠状态的史莱姆调用有效
    void fallAsleep(int sleepTime);

    // 被涂鸦
    void getPainted();

    // 获得被涂鸦状态
    bool isPainted() const;

    // 回复HP，只可以被光合作用技能调用，返回实际的回血量（由于有HP上限）
    int heal(int value);

    // 睡眠状态更新
    void proceed(std::ostream& os, bool isPlayer);
private:
    // 名字
    std::string name;
    // 属性
    Element elem;
    // 生命值、攻击力、防御力、速度
    int HP, ATK, defence, speed;
    // 睡眠回合数
    int sleepRound;
    // 强攻药生效状态
    bool attackEffect;
    // 被涂鸦
    bool tagged;

};


#endif // SLIME_H_INCLUDED
