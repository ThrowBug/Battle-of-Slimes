#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "actor.h"
#include <easy2d/easy2d.h>

// ��ҿ�ѡ���ж�
enum class PlayerAction{ESCAPE, SKILL, CHANGE};

// �з����е��ж�������3��
enum class EnemyAction{SKILL, CHANGE, POTION};

// ��ӡ˫����սʷ��ķ��HP
void printHP(Player& player, Enemy& enemy, std::ostream& os);

// ���ѡ���ж���������ѡ���Ӧ��ö�ٳ�Ա
PlayerAction selectAction(std::istream& is, std::ostream& os, const Player& player);

// ѡ���ж�ʱ����ʾ��Ϣ����selectAction�е�validInput����
std::string actionInfo(std::vector<int> vec);

// ��ҿ�ѡ�ж���
std::vector<int> actionChoice(const Player& player);

// ѡ����ʱ����ʾ��Ϣ����selectSkill�е�validInput����
std::string skillInfo(Player& player);

// �����������������͵�ǰʷ��ķ����ȷ����Ӧ�ļ�����
SkillName decideSkill(int n, Player& player);

// ���ѡ�񽻻��ϳ���ʷ��ķ��team�е�����
int selectChange(std::istream& is, std::ostream& os, Player& player);


// �з����ߣ�����3��
// �����ж���ͨ�����ò����ı��ⲿ����
void enemyDecide(Player& player, Enemy& enemy, const BattleField& battleField, EnemyAction& action, SkillName& skillName, int& ord);

// ��������
void passive(Actor& actor, bool isPlayer, std::istream& is, std::ostream& os);


// ���ӻ�����(����3)
// ͨ���Ƿ�������Լ�ʷ��ķ����Ԫ�����ԣ���ö�ӦͼƬ������
std::string picName(bool isPlayer, Slime& slime);
// ���º���ֻ����Sprite�ĳߴ���λ������
// ������ͼƬ��ȷ����
void setWeather(easy2d::Sprite *wea);
// ��ͼƬ���õ�����ʷ��ķ��
void setLeftBig(easy2d::Sprite *slime);
// ��ͼƬ���õ�����·�Сʷ��ķ��
void setLeftSmall1(easy2d::Sprite *slime);
// ��ͼƬ���õ�����Ϸ�Сʷ��ķ��
void setLeftSmall2(easy2d::Sprite *slime);
// ��ͼƬ���õ��Ҳ��ʷ��ķ��
void setRightBig(easy2d::Sprite *slime);
// ��ͼƬ���õ��Ҳ��·�Сʷ��ķ��
void setRightSmall1(easy2d::Sprite *slime);
// ��ͼƬ���õ��Ҳ��Ϸ�Сʷ��ķ��
void setRightSmall2(easy2d::Sprite *slime);
// ͨ���Ƿ�������Լ�ʷ��ķ����Ԫ�����ԣ���ö�ӦͼƬ������
std::string picName(bool isPlayer, Slime& slime);

// ����һ�������������˶��ϴ���������ʷ��ķSprite��ָ��
std::vector<easy2d::Sprite*> initSlimes(Player& player, Enemy& enemy, easy2d::Scene *scene);

// ����ʷ��ķSprite
void updateSlimes(Player& player, Enemy& enemy, easy2d::Scene *scene,
                  const std::vector<easy2d::Sprite*>& slimes, std::vector<bool>& record);

// ��ʼ������Sprite
std::vector<easy2d::Sprite*> initWea();

// ��������Sprite
void updateWea(const BattleField& battleField, easy2d::Scene *scene,
               const std::vector<easy2d::Sprite*>& weathers, std::vector<bool>& weaRec);

// ��ʼ��״̬ͼ�꣨˯����Ϳѻ��
std::vector<easy2d::Sprite*> initCond();

// ����״̬ͼ�꣨˯����Ϳѻ��
void updateCond(const Slime& left, const Slime& right, easy2d::Scene *scene,
              const std::vector<easy2d::Sprite*>& conditions, std::vector<bool>& condRec);

// ���Ѫ���ĺڵ�
std::vector<easy2d::ShapeNode*> initBottom(easy2d::Scene *scene);

// ��ʼ��Ѫ��
std::vector<easy2d::ShapeNode*> initHPcol(easy2d::Scene *scene);

// ����Ѫ��
void updateHPcol(const Slime& left, const Slime& right, easy2d::Scene *scene,
                 std::vector<easy2d::ShapeNode*>& cols);

// ��ʼ��Ѫ����ֵ
std::vector<easy2d::Text*> initHPval(const Slime& left, const Slime& right, easy2d::Scene *scene);

// ����Ѫ����ֵ
void updateHPval(const Slime& left, const Slime& right, easy2d::Scene *scene,
                 std::vector<easy2d::Text*>& vals);


// ˢ�½���
void update(Player& player, Enemy& enemy, const BattleField& battleField, easy2d::Scene *scene,
            const std::vector<easy2d::Sprite*>& slimes, std::vector<bool>& record,
            const std::vector<easy2d::Sprite*>& weathers, std::vector<bool>& weaRec,
            const std::vector<easy2d::Sprite*>& conditions, std::vector<bool>& condRec,
            std::vector<easy2d::ShapeNode*>& cols, std::vector<easy2d::Text*>& vals);


#endif // ENGINE_H_INCLUDED