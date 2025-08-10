#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED

#include <vector>

#include "slime.h"
#include "skill.h"
#include "battleField.h"

// value��vec�г���ʱ����true����֮false�����ڸ���validInput
template <class T>
bool appear(T value, std::vector<T> vec){
    for (size_t i = 0; i < vec.size(); ++i){
        if (vec[i] == value){
            return true;
        }
    }
    return false;
}

// ���������л�ȡ�Ϸ�������valid�г��֣���ֵ
template <class T>
T validInput(std::istream& is, std::ostream& os, std::vector<T> inputSet, std::string info){
    // valid���������ж������Ƿ�Ϸ������Ϸ�ʱѭ��Ҫ���������
    // ��ʼʱδ���룬valid����Ϊfalse
    bool valid = false;
    T result;
    while (!valid){
        os << info;
        is >> result;
        // �����Ƿ�Ϸ�
        if (appear(result, inputSet)){
            valid = true;
        }
    }
    return result;
}

// ��vec��ɾȥvalue���õ�����
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

    // ���캯����ͨ������Ԫ�س�ʼ��
    Actor(Element e1, Element e2, Element e3);

    Actor();

    // ������������������ڴ�й©
    ~Actor();

    // ��������ʷ��ķ,index = 0,1,2
    void changeSlime(int index);

    // ��ȡ���ϵ�ʷ��ķ
    Slime& getSlime();

    // ��ȡ����Ϊindex��ʷ��ķ,index = 0,1,2
    Slime& getSlime(int index);

    // ��ȡ���ʷ��ķ����
    int numOfAlive() const;

    // ��ȡ����ʷ��ķ��team�е�����
    int inBattleIndex() const;
protected:
    // ʷ��ķ���飬��һ���洢������ʷ��ķָ�������
    std::vector<Slime*> team;

    // ����ʷ��ķ��Ӧ��team�е�����
    int inBattle;

};

class Player : public Actor{
public:
    // ���캯����ͨ�����������team
    Player(std::istream& is, std::ostream& os);


};

class Enemy : public Actor{
public:
    // ���캯������Actor���캯���Ļ����ϣ�Ҫ��ӡ���з�ѡ�����Ϣ
    Enemy(Element e1, Element e2, Element e3, std::ostream& os);

    // ʹ�ø���ҩ
    void takeRevival(std::ostream& os, int index);

    // �Ƿ���ʣ�ิ��ҩ
    bool haveRevival() const;
private:
    // �з�ʣ�ิ��ҩ������
    int restRevival;
};
#endif // ACTOR_H_INCLUDED
