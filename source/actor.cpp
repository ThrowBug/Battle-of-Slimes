#include "actor.h"

// ����Actor
Actor::Actor(Element e1, Element e2, Element e3): team(3), inBattle(0){
    assert(e1 != e2 && e1 != e3 && e2 != e3 &&
           "A team can't have two slimes with the same element.");
    team[0] = new Slime(e1);
    team[1] = new Slime(e2);
    team[2] = new Slime(e3);
}

Actor::~Actor(){
    for (size_t i = 0; i < team.size(); ++i){
        delete team[i];
    }
}

Actor::Actor(){}

void Actor::changeSlime(int index){
    inBattle = index;
}

Slime& Actor::getSlime(){
    return *(team[inBattle]);
}

Slime& Actor::getSlime(int index){
    return *(team[index]);
}

int Actor::numOfAlive() const{
    int result = 0;
    for (size_t i = 0; i < 3; ++i){
        if (team[i]->isAlive()){
            result++;
        }
    }
    return result;
}

int Actor::inBattleIndex() const{
    return inBattle;
}
// ������Player
Player::Player(std::istream& is, std::ostream& os): Actor(){
    os << "You can select three from five "
            "(1 for Green, 2 for Red, 3 for Blue, 4 for Yellow, 5 for Pink)." << std::endl;
    // choiceΪ��ҵ�ѡ��һ��������Ԫ�أ�all�ǿɹ�ѡ�����ļ���
    std::vector<int> choice(3,0), all{1,2,3,4,5};
    choice[0] = validInput(is, os, all, "Select your 1st slime: ");
    // ɾ����ѡ�������������ظ�ѡͬһ������
    all = removeValue(all, choice[0]);
    choice[1] = validInput(is, os, all, "Select your 2nd slime: ");
    all = removeValue(all, choice[1]);
    choice[2] = validInput(is, os, all, "Select your 3rd slime: ");
    // �����������ʼ���г�ʼ��
    // ������ӳ��Ϊ��Ӧ��Ԫ��
    std::vector<Element> elemChoice(3,NORMAL);
    for (int i = 0; i < 3; ++i){
        switch (choice[i]){
            case 1:
                elemChoice[i] = GRASS;
                break;
            case 2:
                elemChoice[i] = FIRE;
                break;
            case 3:
                elemChoice[i] = WATER;
                break;
            case 4:
                elemChoice[i] = ELECTRICITY;
                break;
            case 5:
                elemChoice[i] = NORMAL;
                break;
            default:
                assert(false && "The number is invalid and fail to initialize player's team.");
                break;
        }
    }

    // ����ʷ��ķ����ָ�����team����
    for (int i = 0; i < 3; ++i){
        Slime* p = new Slime(elemChoice[i]);
        // ȷ�����ǿ�ָ��
        if (p == nullptr){
            assert(false && "nullptr can't be in team.");
        }
        team.push_back(p);
    }
    // ���ȳ�ս���ǵ�һֻ
    inBattle = 0;
    // ��ӡѡ���Լ���ս��Ϣ
    os << "You have " << team[0]->getName() << ", " << team[1]->getName() << " and "
        << team[2]->getName() << ", starting with " << team[0]->getName() << std::endl;
}

// ������Enemy
Enemy::Enemy(Element e1, Element e2, Element e3, std::ostream& os): Actor(e1, e2, e3), restRevival(1){
    os << "Enemy has " << team[0]->getName() << ", " << team[1]->getName() << " and "
        << team[2]->getName() << ", starting with " << team[0]->getName() << "." << std::endl;
}

void Enemy::takeRevival(std::ostream& os, int index) {
    // ȷ��ʹ�ø���ҩ�����Ѿ�����
    assert(!getSlime(index).isAlive());
    getSlime(index).heal(maxHP(getSlime(index).getElem()) / 2);
    // ����ҩ������1
    restRevival--;
    os << "Enemy uses Revival Potion on " << getSlime(index).getName() << std::endl;
}

bool Enemy::haveRevival() const {
    return (restRevival > 0);
}