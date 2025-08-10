#ifndef SKILL_H_INCLUDED
#define SKILL_H_INCLUDED

#include "slime.h"
#include "battleField.h"

// 技能名称的枚举
enum SkillName{Tackle, Claw, Quick, Slap, Sing, Paint,
               Leaf, Synthesis, Flame, Sunny, Stream, Rainy, Volt, Thunder};

// 伤害倍率的枚举
enum Multiplier{HALF, ONE, TWO};

// 获取技能的优先级
int priority(SkillName skillName);

class Skill{
public:
    // 发动技能，通过调用private中的静态函数实现，prior变量用于传递给activatePaint函数判断是否生效
    static bool activate(Slime& user, Slime& target, SkillName skillName, BattleField& battleField, std::ostream& os, bool fromPlayer, bool prior);

private:
    // 发动确定名称的技能，攻击类技能需要返回bool值以确定攻击是否致命
    // 攻击类技能
    static void activateTackle(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    static void activateClaw(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    static void activateQuick(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    static void activateSlap(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    // 效果类技能（对对方史莱姆）
    static void activateSing(Slime& user, Slime& target, std::ostream& os, bool fromPlayer);
    static bool activatePaint(Slime& user, Slime& target, std::ostream& os, bool fromPlayer, bool prior);
    // 攻击类技能
    static void activateLeaf(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    // 回复类技能
    static void activateSynthesis(Slime& user, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    // 攻击类技能
    static void activateFlame(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    // 效果类技能（对天气）
    static void activateSunny(Slime& user, BattleField& battleField, std::ostream& os, bool fromPlayer);
    // 攻击类技能
    static void activateStream(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    // 效果类技能（对天气）
    static void activateRainy(Slime& user, BattleField& battleField, std::ostream& os, bool fromPlayer);
    // 攻击类技能
    static void activateVolt(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    static bool activateThunder(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
};


#endif // SKILL_H_INCLUDED
