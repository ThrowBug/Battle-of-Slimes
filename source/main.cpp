#include "slime.h"
#include "actor.h"
#include "skill.h"
#include "battleField.h"
#include "engine.h"
#include <easy2d/easy2d.h>

using namespace easy2d;
using namespace std;

istream& is = cin;
ostream& os = cout;

int main()
{
    if (Game::init()){
        // ��������
        Window::setTitle("Battle of Slimes 2");
        Window::setSize(1200, 300);

        // ��������
        auto scene = new Scene;

        // ���������뱳��
        auto background = gcnew Sprite("Background.png");
        background->setAnchor(0.5f, 0.5f);
        background->setPos(Window::getWidth() / 2, Window::getHeight() / 2);
        scene->addChild(background);

        // ��¼�Ƿ�ƽ�֣������ܡ�����ʤ��ʱ����ֵΪfalse
        bool draw = true;

        os << "Welcome to Battle of Slimes 2!" << endl;

        // ��ʼ��ս��
        BattleField battleField;

        // �з�ѡ��Blue, Green, Yellow������2��
        Enemy enemy(WATER, GRASS, ELECTRICITY, os);

        // ���ѡ��
        Player player(is, os);

        // ��ʼ�����ӻ�����
        // ��ʼ�����ӻ���ʷ��ķ
        vector<Sprite*> slimes = initSlimes(player, enemy, scene);

        // ��¼����ʷ��ķSprite�Ƿ񱻼��볡�����ӽڵ㣬true��ʾ�Ѿ�����
        vector<bool> record{1, 1, 1, 1, 1, 1};

        // ��ʼ�����ӻ�������
        vector<Sprite*> weas = initWea();

        // ��¼����Sprite�Ƿ񱻼��볡�����ӽڵ㣬true��ʾ�Ѿ�����
        vector<bool> weaRec{0, 0};

        // ��ʼ��״̬ͼ��
        vector<Sprite*> conds = initCond();

        // ��¼״̬Sprite�Ƿ񱻼��볡�����ӽڵ㣬true��ʾ�Ѿ�����
        vector<bool> condRec{0, 0, 0, 0};

        // ��ʼ��Ѫ���ĺڵ�
        vector<ShapeNode*> bottoms = initBottom(scene);

        // ��ʼ��Ѫ����Ѫ����Ҫ�ֶ�ɾ�����ڴ棩
        vector<ShapeNode*> cols = initHPcol(scene);

        // ��ʼ��Ѫ����ֵ����Ҫ�ֶ�ɾ�����ڴ棩
        vector<Text*> vals = initHPval(player.getSlime(), enemy.getSlime(), scene);

        // ֻ��enableΪtrueʱ������ִ���˳�������ʵ�ּ�����ִֻ��һ�ε�Ҫ��
        bool enable = true;
        // ���ü�����
        auto callback = [&enable](Event *evt){
            if (enable && evt->type == Event::KeyDown){
                enable = false;
                Game::quit();
            }
        };
        auto lis = gcnew Listener(callback);
        scene->addListener(lis);

        // ��ӡ��ʼHP����Ϸ��ʼ����Ϣ
        printHP(player, enemy, os);
        os << "Battle starts!" << endl;

        // ���볡������Ϸ��ʼ
        SceneManager::enter(scene);
        Game::start();


        // ��¼�غ���
        int round = 1;

        os << "--------------------------------------------------" << endl;

        // ��Ϸ������100���غ�
        while (round <= 100){

            os << "Round " << round << endl;

            // �׶�һ��ѡ���ж�

            // ��Ҿ��߻���
            // ���ѡ����ж�
            PlayerAction pAction;

            // ��ҵĵڶ��κϷ�����
            int secondInput;
            // ���ѡ��ļ������������߷�������ʱ�����壩
            SkillName pSkill;
            // ���ѡ����ϳ�ʷ��ķ��������ѡ�񽻻�ʱ�����壩
            int pIndex;
            // ���0.back�ķ��ش�
        action_menu:

            pAction = selectAction(is, os, player);
            // �����ѡ������
            if (pAction == PlayerAction::ESCAPE){
                os << "You escape" << endl;
                draw = false;
                break;
            } else if (pAction == PlayerAction::SKILL){
                vector<int> temp{0, 1, 2, 3};
                secondInput = validInput(is, os, temp, skillInfo(player));
                if (secondInput == 0){
                    // �ڶ�������Ϊ0��ѡ�񷵻أ��ص���Ǵ�
                    goto action_menu;
                } else{
                    // ���벻Ϊ0������ȷ�������ѡ�ļ�������
                    pSkill = decideSkill(secondInput, player);
                }
            } else if (pAction == PlayerAction::CHANGE){
                pIndex = selectChange(is, os, player);
                if (pIndex == 0){
                    // �ڶ�������Ϊ0��ѡ�񷵻أ��ص���Ǵ�
                    goto action_menu;
                }
            } else{
                assert(false && "No matched action for player.");
            }

            // �з����߻��ڣ�����2��
            // �з����ж�
            EnemyAction eAction;
            // �з�ѡ��ļ��ܣ����������ܣ�
            SkillName eSkill;
            // �з�ѡ��ĸ���ҩ���������򽻻��ϳ�ʷ��ķ������
            int ord;

            enemyDecide(player, enemy, battleField, eAction, eSkill, ord);

            // �׶ζ���ʵʩ�ж�
            if (pAction == PlayerAction::ESCAPE){
                // ���ѡ�����ܣ�����ƽ��
                draw = false;
                break;
            } else if (pAction == PlayerAction::SKILL){
                // ���ѡ�񷢶����ܣ����ݵз�ѡ����ж���֧
                if (eAction == EnemyAction::SKILL){
                    // �����������ܵ��Ⱥ�˳��
                    bool playerPrior;
                    if (priority(pSkill) > priority(eSkill)){
                        playerPrior = true;
                    } else if (priority(eSkill) > priority(pSkill)){
                        playerPrior = false;
                    } else{
                        if (player.getSlime().getSpeed() > enemy.getSlime().getSpeed()){
                            playerPrior = true;
                        } else{
                            playerPrior = false;
                        }
                    }
                    if (playerPrior){
                        // ��¼���ַ����ļ����Ƿ�ɹ�
                        bool effect = Skill::activate(player.getSlime(), enemy.getSlime(), pSkill, battleField, os, true, true);
                        if (effect){
                            // �ȷ����ļ��ܳɹ�
                            if (!enemy.getSlime().isAlive()){
                                if (enemy.numOfAlive() > 0){
                                    passive(enemy, false, is, os);
                                } else{
                                    os << "You Win" << endl;
                                    draw = false;
                                    break;
                                }
                            } else{
                                Skill::activate(enemy.getSlime(), player.getSlime(), eSkill, battleField, os, false, false);
                                if (!player.getSlime().isAlive()){
                                    if (player.numOfAlive() > 0){
                                        update(player, enemy, battleField, scene, slimes, record, weas, weaRec, conds, condRec, cols, vals);
                                        passive(player, true, is, os);
                                    } else{
                                        os << "You Lose" << endl;
                                        draw = false;
                                        break;
                                    }
                                }
                            }
                        } else{
                            // �ȷ����ļ���ʧ��
                            Skill::activate(enemy.getSlime(), player.getSlime(), eSkill, battleField, os, false, true);
                            if (!player.getSlime().isAlive()){
                                if (player.numOfAlive() > 0){
                                    update(player, enemy, battleField, scene, slimes, record, weas, weaRec, conds, condRec, cols, vals);
                                    passive(player, true, is, os);
                                } else{
                                    os << "You Lose" << endl;
                                    draw = false;
                                    break;
                                }
                            }
                        }
                    } else{
                        bool effect = Skill::activate(enemy.getSlime(), player.getSlime(), eSkill, battleField, os, false, true);
                        if (effect){
                            if (!player.getSlime().isAlive()){
                                if (player.numOfAlive() > 0){
                                    update(player, enemy, battleField, scene, slimes, record, weas, weaRec, conds, condRec, cols, vals);
                                    passive(player, true, is, os);
                                } else{
                                    os << "You Lose" << endl;
                                    draw = false;
                                    break;
                                }
                            } else{
                                Skill::activate(player.getSlime(), enemy.getSlime(), pSkill, battleField, os, true, false);
                                if (!enemy.getSlime().isAlive()){
                                    if (enemy.numOfAlive() > 0){
                                        passive(enemy, false, is, os);
                                    } else{
                                        os << "You Win" << endl;
                                        draw = false;
                                        break;
                                    }
                                }
                            }
                        } else{
                            Skill::activate(player.getSlime(), enemy.getSlime(), pSkill, battleField, os, true, true);
                            if (!enemy.getSlime().isAlive()){
                                if (enemy.numOfAlive() > 0){
                                    passive(enemy, false, is, os);
                                } else{
                                    os << "You Win" << endl;
                                    draw = false;
                                    break;
                                }
                            }
                        }
                    }
                } else if (eAction == EnemyAction::CHANGE){
                    // �з���������
                    enemy.changeSlime(ord);
                    os << "Enemy sends " << enemy.getSlime().getName() << endl;
                    // ��ҷ�������
                    Skill::activate(player.getSlime(), enemy.getSlime(), pSkill, battleField, os, true, true);
                    // ����Ƿ�з���Ҫ��������
                    if (!enemy.getSlime().isAlive()){
                        if (enemy.numOfAlive() > 0){
                            passive(enemy, false, is, os);
                        } else{
                            os << "You Win" << endl;
                            draw = false;
                            break;
                        }
                    }
                } else if (eAction == EnemyAction::POTION){
                    // �з�ʹ�ø���ҩ
                    enemy.takeRevival(os, ord);
                    // ��ҷ�������
                    Skill::activate(player.getSlime(), enemy.getSlime(), pSkill, battleField, os, true, true);
                    // ����Ƿ�з���Ҫ��������
                    if (!enemy.getSlime().isAlive()) {
                        if (enemy.numOfAlive() > 0) {
                            passive(enemy, false, is, os);
                        } else {
                            os << "You Win" << endl;
                            draw = false;
                            break;
                        }
                    }
                } else{
                    assert(false && "Enemy has no other action in Task2.");
                }
            } else if (pAction == PlayerAction::CHANGE){
                if (eAction == EnemyAction::SKILL){
                    // �����������
                    player.changeSlime(pIndex - 1);
                    os << "You send " << player.getSlime().getName() << endl;
                    // �з���������
                    Skill::activate(enemy.getSlime(), player.getSlime(), eSkill, battleField, os, false, true);
                    // ����Ƿ������Ҫ��������
                    if (!player.getSlime().isAlive()){
                        if (player.numOfAlive() > 0){
                            update(player, enemy, battleField, scene, slimes, record, weas, weaRec, conds, condRec, cols, vals);
                            passive(player, true, is, os);
                        } else{
                            os << "You Lose" << endl;
                            draw = false;
                            break;
                        }
                    }
                } else if (eAction == EnemyAction::CHANGE){
                    // ��ѡ����������ʱ������Ȼ�
                    player.changeSlime(pIndex - 1);
                    os << "You send " << player.getSlime().getName() << endl;
                    // �з�����������2��
                    enemy.changeSlime(ord);
                    os << "Enemy sends Red" << endl;
                } else if (eAction == EnemyAction::POTION) {
                    player.changeSlime(pIndex - 1);
                    os << "You send " << player.getSlime().getName() << endl;
                    enemy.takeRevival(os, ord);
                } else{
                    assert(false && "Enemy has no other action in Task3.");
                }
            } else{
                assert(false && "Player has no other action in Task3.");
            }

            // ��ӡHP
            printHP(player, enemy, os);

            if (round != 100){
                os << "--------------------------------------------------" << endl;

                // ��������
                battleField.proceed(os);

                // ˯�߻غ�������
                player.getSlime().proceed(os, true);
                enemy.getSlime().proceed(os, false);
            }

            // ˢ�½���
            update(player, enemy, battleField, scene, slimes, record, weas, weaRec, conds, condRec, cols, vals);

            // �غ�����1
            round++;
        }
        // �ж�ƽ��
        if (draw){
            os << "Draw" << endl;
        }

        // ������ڴ�
        delete scene;
        for (size_t i = 0; i < slimes.size(); ++i){
            delete slimes[i];
        }
        for (size_t i = 0; i < weas.size(); ++i){
            delete weas[i];
        }
        for (size_t i = 0; i < conds.size(); ++i){
            delete conds[i];
        }
        for (size_t i = 0; i < bottoms.size(); ++i){
            delete bottoms[i];
        }
        for (size_t i = 0; i < cols.size(); ++i){
            delete cols[i];
        }
        for (size_t i = 0; i < vals.size(); ++i){
            delete vals[i];
        }
    }
    Game::destroy();

    return 0;
}
