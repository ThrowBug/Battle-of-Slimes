#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED

#include <vector>

#include "slime.h"
#include "skill.h"
#include "battleField.h"

// value在vec中出现时返回true，反之false。用于辅助validInput
template <class T>
bool appear(T value, std::vector<T> vec){
    for (size_t i = 0; i < vec.size(); ++i){
        if (vec[i] == value){
            return true;
        }
    }
    return false;
}

// 从输入流中获取合法（即在valid中出现）的值
template <class T>
T validInput(std::istream& is, std::ostream& os, std::vector<T> inputSet, std::string info){
    // valid变量用于判断输入是否合法，不合法时循环要求玩家输入
    // 初始时未输入，valid变量为false
    bool valid = false;
    T result;
    while (!valid){
        os << info;
        is >> result;
        // 检验是否合法
        if (appear(result, inputSet)){
            valid = true;
        }
    }
    return result;
}

// 从vec中删去value所得的向量
template <class T>
std::vector<T> removeValue(std::vector<T> vec, T value){
    for (int i = vec.size() - 1; i >= 0; i--){
        if (vec[i] == value){
            vec.erase(vec.begin() + i);
        }
    }
    return vec;
}

class Actor{
public:

    // 构造函数：通过三个元素初始化
    Actor(Element e1, Element e2, Element e3);

    Actor();

    // 析构函数，避免堆上内存泄漏
    ~Actor();

    // 更换场上史莱姆,index = 0,1,2
    void changeSlime(int index);

    // 获取场上的史莱姆
    Slime& getSlime();

    // 获取索引为index的史莱姆,index = 0,1,2
    Slime& getSlime(int index);

    // 获取存活史莱姆数量
    int numOfAlive() const;

    // 获取场上史莱姆在team中的索引
    int inBattleIndex() const;
protected:
    // 史莱姆队伍，是一个存储了三个史莱姆指针的向量
    std::vector<Slime*> team;

    // 场上史莱姆对应于team中的索引
    int inBattle;

};

class Player : public Actor{
public:
    // 构造函数，通过玩家输入获得team
    Player(std::istream& is, std::ostream& os);


};

class Enemy : public Actor{
public:
    // 构造函数，在Actor构造函数的基础上，要打印出敌方选择的信息
    Enemy(Element e1, Element e2, Element e3, std::ostream& os);

    // 使用复苏药
    void takeRevival(std::ostream& os, int index);

    // 是否有剩余复苏药
    bool haveRevival() const;
private:
    // 敌方剩余复苏药的数量
    int restRevival;
};
#endif // ACTOR_H_INCLUDED
