#ifndef SKILL_H_INCLUDED
#define SKILL_H_INCLUDED

#include "slime.h"
#include "battleField.h"

// �������Ƶ�ö��
enum SkillName{Tackle, Claw, Quick, Slap, Sing, Paint,
               Leaf, Synthesis, Flame, Sunny, Stream, Rainy, Volt, Thunder};

// �˺����ʵ�ö��
enum Multiplier{HALF, ONE, TWO};

// ��ȡ���ܵ����ȼ�
int priority(SkillName skillName);

class Skill{
public:
    // �������ܣ�ͨ������private�еľ�̬����ʵ�֣�prior�������ڴ��ݸ�activatePaint�����ж��Ƿ���Ч
    static bool activate(Slime& user, Slime& target, SkillName skillName, BattleField& battleField, std::ostream& os, bool fromPlayer, bool prior);

private:
    // ����ȷ�����Ƶļ��ܣ������༼����Ҫ����boolֵ��ȷ�������Ƿ�����
    // �����༼��
    static void activateTackle(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    static void activateClaw(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    static void activateQuick(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    static void activateSlap(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    // Ч���༼�ܣ��ԶԷ�ʷ��ķ��
    static void activateSing(Slime& user, Slime& target, std::ostream& os, bool fromPlayer);
    static bool activatePaint(Slime& user, Slime& target, std::ostream& os, bool fromPlayer, bool prior);
    // �����༼��
    static void activateLeaf(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    // �ظ��༼��
    static void activateSynthesis(Slime& user, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    // �����༼��
    static void activateFlame(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    // Ч���༼�ܣ���������
    static void activateSunny(Slime& user, BattleField& battleField, std::ostream& os, bool fromPlayer);
    // �����༼��
    static void activateStream(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    // Ч���༼�ܣ���������
    static void activateRainy(Slime& user, BattleField& battleField, std::ostream& os, bool fromPlayer);
    // �����༼��
    static void activateVolt(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
    static bool activateThunder(Slime& user, Slime& target, const BattleField& battleField, std::ostream& os, bool fromPlayer);
};


#endif // SKILL_H_INCLUDED
