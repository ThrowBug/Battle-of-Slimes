#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "actor.h"
#include <easy2d/easy2d.h>

// 玩家可选的行动
enum class PlayerAction{ESCAPE, SKILL, CHANGE};

// 敌方可行的行动（任务3）
enum class EnemyAction{SKILL, CHANGE, POTION};

// 打印双方出战史莱姆的HP
void printHP(Player& player, Enemy& enemy, std::ostream& os);

// 玩家选择行动，获得玩家选项对应的枚举成员
PlayerAction selectAction(std::istream& is, std::ostream& os, const Player& player);

// 选择行动时的提示信息，被selectAction中的validInput调用
std::string actionInfo(std::vector<int> vec);

// 玩家可选行动集
std::vector<int> actionChoice(const Player& player);

// 选择技能时的提示信息，被selectSkill中的validInput调用
std::string skillInfo(Player& player);

// 根据玩家输入的整数和当前史莱姆属性确定相应的技能名
SkillName decideSkill(int n, Player& player);

// 玩家选择交换上场的史莱姆在team中的索引
int selectChange(std::istream& is, std::ostream& os, Player& player);


// 敌方决策（任务3）
// 决策行动，通过引用参数改变外部变量
void enemyDecide(Player& player, Enemy& enemy, const BattleField& battleField, EnemyAction& action, SkillName& skillName, int& ord);

// 被动交换
void passive(Actor& actor, bool isPlayer, std::istream& is, std::ostream& os);


// 可视化部分(任务3)
// 通过是否是玩家以及史莱姆（的元素属性）获得对应图片的名称
std::string picName(bool isPlayer, Slime& slime);
// 以下函数只进行Sprite的尺寸与位置设置
// 将天气图片正确设置
void setWeather(easy2d::Sprite *wea);
// 将图片设置到左侧大史莱姆处
void setLeftBig(easy2d::Sprite *slime);
// 将图片设置到左侧下方小史莱姆处
void setLeftSmall1(easy2d::Sprite *slime);
// 将图片设置到左侧上方小史莱姆处
void setLeftSmall2(easy2d::Sprite *slime);
// 将图片设置到右侧大史莱姆处
void setRightBig(easy2d::Sprite *slime);
// 将图片设置到右侧下方小史莱姆处
void setRightSmall1(easy2d::Sprite *slime);
// 将图片设置到右侧上方小史莱姆处
void setRightSmall2(easy2d::Sprite *slime);
// 通过是否是玩家以及史莱姆（的元素属性）获得对应图片的名称
std::string picName(bool isPlayer, Slime& slime);

// 返回一个向量，包括了堆上创建的六个史莱姆Sprite的指针
std::vector<easy2d::Sprite*> initSlimes(Player& player, Enemy& enemy, easy2d::Scene *scene);

// 更新史莱姆Sprite
void updateSlimes(Player& player, Enemy& enemy, easy2d::Scene *scene,
                  const std::vector<easy2d::Sprite*>& slimes, std::vector<bool>& record);

// 初始化天气Sprite
std::vector<easy2d::Sprite*> initWea();

// 更新天气Sprite
void updateWea(const BattleField& battleField, easy2d::Scene *scene,
               const std::vector<easy2d::Sprite*>& weathers, std::vector<bool>& weaRec);

// 初始化状态图标（睡眠与涂鸦）
std::vector<easy2d::Sprite*> initCond();

// 更新状态图标（睡眠与涂鸦）
void updateCond(const Slime& left, const Slime& right, easy2d::Scene *scene,
              const std::vector<easy2d::Sprite*>& conditions, std::vector<bool>& condRec);

// 添加血条的黑底
std::vector<easy2d::ShapeNode*> initBottom(easy2d::Scene *scene);

// 初始化血条
std::vector<easy2d::ShapeNode*> initHPcol(easy2d::Scene *scene);

// 更新血条
void updateHPcol(const Slime& left, const Slime& right, easy2d::Scene *scene,
                 std::vector<easy2d::ShapeNode*>& cols);

// 初始化血量数值
std::vector<easy2d::Text*> initHPval(const Slime& left, const Slime& right, easy2d::Scene *scene);

// 更新血量数值
void updateHPval(const Slime& left, const Slime& right, easy2d::Scene *scene,
                 std::vector<easy2d::Text*>& vals);


// 刷新界面
void update(Player& player, Enemy& enemy, const BattleField& battleField, easy2d::Scene *scene,
            const std::vector<easy2d::Sprite*>& slimes, std::vector<bool>& record,
            const std::vector<easy2d::Sprite*>& weathers, std::vector<bool>& weaRec,
            const std::vector<easy2d::Sprite*>& conditions, std::vector<bool>& condRec,
            std::vector<easy2d::ShapeNode*>& cols, std::vector<easy2d::Text*>& vals);


#endif // ENGINE_H_INCLUDED