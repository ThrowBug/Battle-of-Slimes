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
        // 调整窗口
        Window::setTitle("Battle of Slimes 2");
        Window::setSize(1200, 300);

        // 创建场景
        auto scene = new Scene;

        // 创建并加入背景
        auto background = gcnew Sprite("Background.png");
        background->setAnchor(0.5f, 0.5f);
        background->setPos(Window::getWidth() / 2, Window::getHeight() / 2);
        scene->addChild(background);

        // 记录是否平局，当逃跑、决出胜负时，赋值为false
        bool draw = true;

        os << "Welcome to Battle of Slimes 2!" << endl;

        // 初始化战场
        BattleField battleField;

        // 敌方选择Blue, Green, Yellow（任务2）
        Enemy enemy(WATER, GRASS, ELECTRICITY, os);

        // 玩家选择
        Player player(is, os);

        // 初始化可视化界面
        // 初始化可视化的史莱姆
        vector<Sprite*> slimes = initSlimes(player, enemy, scene);

        // 记录六个史莱姆Sprite是否被加入场景的子节点，true表示已经加入
        vector<bool> record{1, 1, 1, 1, 1, 1};

        // 初始化可视化的天气
        vector<Sprite*> weas = initWea();

        // 记录天气Sprite是否被加入场景的子节点，true表示已经加入
        vector<bool> weaRec{0, 0};

        // 初始化状态图标
        vector<Sprite*> conds = initCond();

        // 记录状态Sprite是否被加入场景的子节点，true表示已经加入
        vector<bool> condRec{0, 0, 0, 0};

        // 初始化血条的黑底
        vector<ShapeNode*> bottoms = initBottom(scene);

        // 初始化血条的血（需要手动删除堆内存）
        vector<ShapeNode*> cols = initHPcol(scene);

        // 初始化血量数值（需要手动删除堆内存）
        vector<Text*> vals = initHPval(player.getSlime(), enemy.getSlime(), scene);

        // 只有enable为true时监听器执行退出操作，实现监听器只执行一次的要求
        bool enable = true;
        // 设置监听器
        auto callback = [&enable](Event *evt){
            if (enable && evt->type == Event::KeyDown){
                enable = false;
                Game::quit();
            }
        };
        auto lis = gcnew Listener(callback);
        scene->addListener(lis);

        // 打印初始HP和游戏开始的信息
        printHP(player, enemy, os);
        os << "Battle starts!" << endl;

        // 进入场景，游戏开始
        SceneManager::enter(scene);
        Game::start();


        // 记录回合数
        int round = 1;

        os << "--------------------------------------------------" << endl;

        // 游戏最多进行100个回合
        while (round <= 100){

            os << "Round " << round << endl;

            // 阶段一：选择行动

            // 玩家决策环节
            // 玩家选择的行动
            PlayerAction pAction;

            // 玩家的第二次合法输入
            int secondInput;
            // 玩家选择的技能名（仅决策发动技能时有意义）
            SkillName pSkill;
            // 玩家选择的上场史莱姆索引（仅选择交换时有意义）
            int pIndex;
            // 标记0.back的返回处
        action_menu:

            pAction = selectAction(is, os, player);
            // 若玩家选择逃跑
            if (pAction == PlayerAction::ESCAPE){
                os << "You escape" << endl;
                draw = false;
                break;
            } else if (pAction == PlayerAction::SKILL){
                vector<int> temp{0, 1, 2, 3};
                secondInput = validInput(is, os, temp, skillInfo(player));
                if (secondInput == 0){
                    // 第二次输入为0，选择返回，回到标记处
                    goto action_menu;
                } else{
                    // 输入不为0，可以确定玩家所选的技能名称
                    pSkill = decideSkill(secondInput, player);
                }
            } else if (pAction == PlayerAction::CHANGE){
                pIndex = selectChange(is, os, player);
                if (pIndex == 0){
                    // 第二次输入为0，选择返回，回到标记处
                    goto action_menu;
                }
            } else{
                assert(false && "No matched action for player.");
            }

            // 敌方决策环节（任务2）
            // 敌方的行动
            EnemyAction eAction;
            // 敌方选择的技能（若发动技能）
            SkillName eSkill;
            // 敌方选择的复苏药对象索引或交换上场史莱姆的索引
            int ord;

            enemyDecide(player, enemy, battleField, eAction, eSkill, ord);

            // 阶段二：实施行动
            if (pAction == PlayerAction::ESCAPE){
                // 玩家选择逃跑，不是平局
                draw = false;
                break;
            } else if (pAction == PlayerAction::SKILL){
                // 玩家选择发动技能，根据敌方选择的行动分支
                if (eAction == EnemyAction::SKILL){
                    // 决定发动技能的先后顺序
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
                        // 记录先手发动的技能是否成功
                        bool effect = Skill::activate(player.getSlime(), enemy.getSlime(), pSkill, battleField, os, true, true);
                        if (effect){
                            // 先发动的技能成功
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
                            // 先发动的技能失败
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
                    // 敌方主动交换
                    enemy.changeSlime(ord);
                    os << "Enemy sends " << enemy.getSlime().getName() << endl;
                    // 玩家发动技能
                    Skill::activate(player.getSlime(), enemy.getSlime(), pSkill, battleField, os, true, true);
                    // 检查是否敌方需要被动交换
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
                    // 敌方使用复苏药
                    enemy.takeRevival(os, ord);
                    // 玩家发动技能
                    Skill::activate(player.getSlime(), enemy.getSlime(), pSkill, battleField, os, true, true);
                    // 检查是否敌方需要被动交换
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
                    // 玩家主动交换
                    player.changeSlime(pIndex - 1);
                    os << "You send " << player.getSlime().getName() << endl;
                    // 敌方发动技能
                    Skill::activate(enemy.getSlime(), player.getSlime(), eSkill, battleField, os, false, true);
                    // 检查是否玩家需要被动交换
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
                    // 都选择主动交换时，玩家先换
                    player.changeSlime(pIndex - 1);
                    os << "You send " << player.getSlime().getName() << endl;
                    // 敌方交换（任务2）
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

            // 打印HP
            printHP(player, enemy, os);

            if (round != 100){
                os << "--------------------------------------------------" << endl;

                // 天气更新
                battleField.proceed(os);

                // 睡眠回合数更新
                player.getSlime().proceed(os, true);
                enemy.getSlime().proceed(os, false);
            }

            // 刷新界面
            update(player, enemy, battleField, scene, slimes, record, weas, weaRec, conds, condRec, cols, vals);

            // 回合数加1
            round++;
        }
        // 判定平局
        if (draw){
            os << "Draw" << endl;
        }

        // 清理堆内存
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
