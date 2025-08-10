#ifndef SLIME_H_INCLUDED
#define SLIME_H_INCLUDED

#include <string>
#include <iostream>
#include <cassert>

// Ԫ������
enum Element{NORMAL, GRASS, FIRE, WATER, ELECTRICITY};

// ��ȡ���Զ�Ӧ���HP
int maxHP(Element ele);

// ��ȡ����ele��Ԫ������ (��ʵ�֣�
Element counterElem(Element ele);

class Slime{
public:
    // ���캯��
    Slime(Element ele);

    // ��ȡ����
    std::string getName() const;

    // ��ȡԪ������
    Element getElem() const;

    // ��ȡ����ֵ
    int getHP() const;

    // ��ȡ������
    int getATK() const;

    // ��ȡ������
    int getDef() const;

    // ��ȡ�ٶ�
    int getSpeed() const;

    // ��ȡ���״̬
    bool isAlive() const;

    // ��ȡ˯�߻غ���
    int getSleepRound() const;

    // ��ȡǿ��ҩ��Ч״̬
    bool inEffect() const;

    // ���˿۳�����ֵ��˯�߻غ������㣬����true��ʾHP�ѽ���Ϊ0
    bool hurt(int damage);

    // ��˯��ֻ��δ����˯��״̬��ʷ��ķ������Ч
    void fallAsleep(int sleepTime);

    // ��Ϳѻ
    void getPainted();

    // ��ñ�Ϳѻ״̬
    bool isPainted() const;

    // �ظ�HP��ֻ���Ա�������ü��ܵ��ã�����ʵ�ʵĻ�Ѫ����������HP���ޣ�
    int heal(int value);

    // ˯��״̬����
    void proceed(std::ostream& os, bool isPlayer);
private:
    // ����
    std::string name;
    // ����
    Element elem;
    // ����ֵ�������������������ٶ�
    int HP, ATK, defence, speed;
    // ˯�߻غ���
    int sleepRound;
    // ǿ��ҩ��Ч״̬
    bool attackEffect;
    // ��Ϳѻ
    bool tagged;

};


#endif // SLIME_H_INCLUDED
