#include "engine.h"
void printHP(Player& player, Enemy& enemy, std::ostream& os){
    os << "Your " << player.getSlime().getName() << ": HP " << player.getSlime().getHP()
       << " || "
       << "Enemy's " << enemy.getSlime().getName() << ": HP " << enemy.getSlime().getHP()
       << std::endl;
}

PlayerAction selectAction(std::istream& is, std::ostream& os, const Player& player){
    std::vector<int> actionVec = actionChoice(player);
    int input = validInput(is, os, actionVec, actionInfo(actionVec));
    switch (input){
        case 0:
            return PlayerAction::ESCAPE;
        case 1:
            return PlayerAction::SKILL;
        case 2:
            return PlayerAction::CHANGE;
        default:
            assert(false && "Not a valid input to choose action.");
    }
    return PlayerAction::ESCAPE;
}

std::string actionInfo(std::vector<int> vec){
    // 考虑到选项之间有逗号和空格，用first变量记录是否为第一个选项
    bool first = true;
    std::string info = "Select your action (";
    if (appear(0, vec)){
        info += (first ? "" : ", ");
        info += "0 for escape";
        first = false;
    }
    if (appear(1,vec)){
        info += (first ? "" : ", ");
        info += "1 for skill";
        first = false;
    }
    if (appear(2,vec)){
        info += (first ? "" : ", ");
        info += "2 for change";
        first = false;
    }
    info += "): ";
    return info;
}


std::vector<int> actionChoice(const Player& player){
    // 0代表escape，1代表skill，任何时候均可以进行
    std::vector<int> result{0, 1};
    // 2代表交换，仅在有其他存活的史莱姆时可以进行
    if (player.numOfAlive() > 1){
        result.push_back(2);
    }
    return result;
}


std::string skillInfo(Player& player){
    std::string info;
    switch (player.getSlime().getElem()){
        case GRASS:
            info = "Select the skill (0 for back, 1 for Tackle, 2 for Leaf, 3 for Synthesis): ";
            break;
        case FIRE:
            info = "Select the skill (0 for back, 1 for Claw, 2 for Flame, 3 for Sunny): ";
            break;
        case WATER:
            info = "Select the skill (0 for back, 1 for Tackle, 2 for Stream, 3 for Rainy): ";
            break;
        case ELECTRICITY:
            info = "Select the skill (0 for back, 1 for Quick, 2 for Volt, 3 for Thunder): ";
            break;
        case NORMAL:
            info = "Select the skill (0 for back, 1 for Slap, 2 for Sing, 3 for Paint): ";
            break;
        default:
            assert(false && "Not a valid Element.");
            break;
    }
    return info;
}


SkillName decideSkill(int n, Player& player){
    SkillName result;
    switch (player.getSlime().getElem()){
        case GRASS:
            if (n == 1){
                result = Tackle;
            }else if (n == 2){
                result = Leaf;
            }else if (n == 3){
                result = Synthesis;
            }else{
                assert(false && "No matched n in case GRASS.");
            }
            break;
        case FIRE:
            if (n == 1){
                result = Claw;
            }else if (n == 2){
                result = Flame;
            }else if (n == 3){
                result = Sunny;
            }else{
                assert(false && "No matched n in case FIRE.");
            }
            break;
        case WATER:
            if (n == 1){
                result = Tackle;
            }else if (n == 2){
                result = Stream;
            }else if (n == 3){
                result = Rainy;
            }else{
                assert(false && "No matched n in case WATER.");
            }
            break;
        case ELECTRICITY:
            if (n == 1){
                result = Quick;
            }else if (n == 2){
                result = Volt;
            }else if (n == 3){
                result = Thunder;
            }else{
                assert(false && "No matched n in case ELECTRICITY.");
            }
            break;
        case NORMAL:
            if (n == 1){
                result = Slap;
            }else if (n == 2){
                result = Sing;
            }else if (n == 3){
                result = Paint;
            }else{
                assert(false && "No matched n in case NORMAL.");
            }
            break;
        default:
            assert(false && "No matched element to decide the name of skill.");
    }
    return result;
}


int selectChange(std::istream& is, std::ostream& os, Player& player){
    // 用temp记录可以交换上场的史莱姆的索引加上1的结果
    std::vector<int> temp;
    for (int i = 0; i < 3; ++i){
        // 不在场上，且存活，可上场
        if (i != player.inBattleIndex() && player.getSlime(i).isAlive()){
            temp.push_back(i + 1);
        }
    }
    std::string info = "Select your next slime (0 for back";
    for (int index : temp){
        info += ", ";
        info += std::to_string(index);
        info += " for ";
        info += player.getSlime(index - 1).getName();
        info += "-HP";
        info += std::to_string(player.getSlime(index - 1).getHP());
    }
    info += "): ";
    temp.push_back(0);
    return validInput(is, os, temp, info);
}

void enemyDecide(Player& player, Enemy& enemy, const BattleField& battleField, EnemyAction& action, SkillName& skillName, int& ord) {
    if (enemy.haveRevival() && (!enemy.getSlime(0).isAlive() || (!enemy.getSlime(1).isAlive() && !enemy.getSlime(2).isAlive()))) {
        action = EnemyAction::POTION;
        if (!enemy.getSlime(0).isAlive()) {
            ord = 0;
        }
        else if (!enemy.getSlime(1).isAlive()) {
            ord = 1;
        }
        else {
            ord = 2;
        }
    }
    else if (enemy.getSlime().getElem() == WATER) {
        if (battleField.getWeather() == Sunny) {
            action = EnemyAction::SKILL;
            skillName = Rainy;
        }
        else if (player.getSlime().getElem() == ELECTRICITY ||
            (battleField.getWeather() == RAINY && player.getSlime().getElem() == GRASS)) {
            if (enemy.getSlime(1).isAlive()) {
                action = EnemyAction::CHANGE;
                ord = 1;
            }
            else if (battleField.getWeather() == RAINY && enemy.getSlime(2).isAlive()) {
                action = EnemyAction::CHANGE;
                ord = 2;
            }
            else {
                action = EnemyAction::SKILL;
                skillName = Stream;
            }
        }
        else if (player.getSlime().getElem() == WATER) {
            action = EnemyAction::SKILL;
            skillName = Tackle;
        }
        else if (battleField.getWeather() == RAINY) {
            action = EnemyAction::SKILL;
            skillName = Stream;
        }
        else {
            action = EnemyAction::SKILL;
            skillName = Rainy;
        }
    }
    else if (enemy.getSlime().getElem() == GRASS) {
        if (player.getSlime().getElem() == FIRE) {
            if (enemy.getSlime(0).isAlive()) {
                action = EnemyAction::CHANGE;
                ord = 0;
            }
            else if (enemy.getSlime().getHP() >= maxHP(GRASS) * 2.0 / 3) {
                action = EnemyAction::SKILL;
                skillName = Tackle;
            }
            else {
                action = EnemyAction::SKILL;
                skillName = Synthesis;
            }
        }
        else if (player.getSlime().getElem() == GRASS) {
            if (enemy.getSlime().getHP() >= maxHP(GRASS) / 2.0) {
                action = EnemyAction::SKILL;
                skillName = Tackle;
            }
            else {
                action = EnemyAction::SKILL;
                skillName = Synthesis;
            }
        }
        else if (enemy.getSlime().getHP() >= maxHP(GRASS) / 2.0 || player.getSlime().getHP() < maxHP(player.getSlime().getElem()) / 5.0) {
            action = EnemyAction::SKILL;
            skillName = Leaf;
        }
        else {
            action = EnemyAction::SKILL;
            skillName = Synthesis;
        }
    }
    else if (enemy.getSlime().getElem() == ELECTRICITY) {
        if (player.getSlime().getHP() < maxHP(player.getSlime().getElem()) / 5.0) {
            action = EnemyAction::SKILL;
            skillName = Quick;
        }
        else if (player.getSlime().getElem() == FIRE) {
            if (enemy.getSlime(1).isAlive()) {
                action = EnemyAction::CHANGE;
                ord = 0;
            }
            else {
                action = EnemyAction::SKILL;
                skillName = Volt;
            }
        }
        else if (battleField.getWeather() == RAINY) {
            action = EnemyAction::SKILL;
            skillName = Thunder;
        }
        else if (enemy.getSlime(1).isAlive()) {
            action = EnemyAction::CHANGE;
            ord = 1;
        }
        else if (player.getSlime().getElem() == ELECTRICITY || player.getSlime().getElem() == GRASS) {
            action = EnemyAction::SKILL;
            skillName = Quick;
        }
        else {
            action = EnemyAction::SKILL;
            skillName = Volt;
        }
    }
}


void passive(Actor& actor, bool isPlayer, std::istream& is, std::ostream& os){
    if (isPlayer){
        // 用temp记录可以交换上场的史莱姆的索引加上1的结果
        std::vector<int> temp;
        for (int i = 0; i < 3; ++i){
            if (actor.getSlime(i).isAlive()){
                temp.push_back(i + 1);
            }
        }
        std::string info = "Select your next slime (";
        // 打印的第一项，前面无逗号
        bool firstItem = true;
        for (int index : temp){
            if (firstItem){
                firstItem = false;
            }else{
                info += ", ";
            }
            info += std::to_string(index);
            info += " for ";
            info += actor.getSlime(index - 1).getName();
            info += "-HP";
            info += std::to_string(actor.getSlime(index - 1).getHP());
        }
        info += "): ";
        int choice = validInput(is, os, temp, info);
        actor.changeSlime(choice - 1);
        os << "You send " << actor.getSlime().getName() << std::endl;
    }else{
        // 任务2中的敌方被动交换逻辑
        int curIndex = actor.inBattleIndex();
        if (curIndex == 0){
            actor.changeSlime(actor.getSlime(1).isAlive() ? 1 : 2);
        }else if (curIndex == 1){
            actor.changeSlime(actor.getSlime(0).isAlive() ? 0 : 2);
        }else if (curIndex == 2){
            actor.changeSlime(actor.getSlime(0).isAlive() ? 0 : 1);
        }
        os << "Enemy sends " << actor.getSlime().getName() << std::endl;
    }
}

std::string picName(bool isPlayer, Slime& slime){
    std::string result;
    switch (slime.getElem()){
        case GRASS:
            result = "SlimeGrass";
            break;
        case WATER:
            result = "SlimeWater";
            break;
        case FIRE:
            result = "SlimeFire";
            break;
        case ELECTRICITY:
            result = "SlimeThunder";
            break;
        case NORMAL:
            result = "SlimeNormal";
            break;
        default:
            assert(false && "No matched picture");
            break;
    }
    result += isPlayer ? "" : "_r";
    result += ".png";
    return result;
}

void setWeather(easy2d::Sprite *wea){
    wea->setScale(0.8f);
    wea->setPos(600, 40);
}

void setLeftBig(easy2d::Sprite *slime){
    slime->setScale(0.35f);
    slime->setPos(400, 160);
}

void setLeftSmall1(easy2d::Sprite *slime){
    slime->setScale(0.15f);
    slime->setPos(140, 250);
}

void setLeftSmall2(easy2d::Sprite *slime){
    slime->setScale(0.15f);
    slime->setPos(200, 160);
}

void setRightBig(easy2d::Sprite *slime){
    slime->setScale(0.35f);
    slime->setPos(800, 160);
}

void setRightSmall1(easy2d::Sprite *slime){
    slime->setScale(0.15f);
    slime->setPos(1060, 250);
}

void setRightSmall2(easy2d::Sprite *slime){
    slime->setScale(0.15f);
    slime->setPos(1000, 160);
}

std::vector<easy2d::Sprite*> initSlimes(Player& player, Enemy& enemy, easy2d::Scene *scene){
    std::vector<easy2d::Sprite*> result;
    for (int i = 0; i < 3; ++i){
        result.push_back(new easy2d::Sprite(picName(true, player.getSlime(i))));
        result[i]->setAnchor(0.5f, 0.5f);
    }
    for (int i = 0; i < 3; ++i){
        result.push_back(new easy2d::Sprite(picName(false, enemy.getSlime(i))));
        result[i + 3]->setAnchor(0.5f, 0.5f);
    }
    setLeftBig(result[0]);
    setLeftSmall1(result[1]);
    setLeftSmall2(result[2]);
    setRightBig(result[3]);
    setRightSmall1(result[4]);
    setRightSmall2(result[5]);
    for (int i = 0; i < 6; ++i){
        scene->addChild(result[i]);
    }
    return result;
}

void updateSlimes(Player& player, Enemy& enemy, easy2d::Scene *scene,
                  const std::vector<easy2d::Sprite*>& slimes, std::vector<bool>& record){
    int inBattleLeft = player.inBattleIndex(),
        inBattleRight = enemy.inBattleIndex();
    // 断言场上的大史莱姆目前是scene的子节点
    assert(record[inBattleLeft] && record[inBattleRight + 3]);

    setLeftBig(slimes[inBattleLeft]);
    setRightBig(slimes[inBattleRight + 3]);
    // 更新场景的子节点集合，同时修改record
    for (int i = 0; i < 3; ++i){
        if (i != inBattleLeft){
            if (!player.getSlime(i).isAlive() && record[i]){
                scene->removeChild(slimes[i]);
                record[i] = false;
            } else if (player.getSlime(i).isAlive() && !record[i]){
                scene->addChild(slimes[i]);
                record[i] = true;
            }
        }
    }
    for (int i = 0; i < 3; ++i){
        if (i != inBattleRight){
            if (!enemy.getSlime(i).isAlive() && record[i + 3]){
                scene->removeChild(slimes[i + 3]);
                record[i + 3] = false;
            } else if (enemy.getSlime(i).isAlive() && !record[i + 3]){
                scene->addChild(slimes[i + 3]);
                record[i + 3] = true;
            }
        }
    }
    // 是否为team中顺序的第一只需要显示的小史莱姆
    bool firstSmall = true;
    // 将小史莱姆位置更新
    for (int i = 0; i < 3; ++i){
        if (i != inBattleLeft && record[i]){
            if (firstSmall){
                firstSmall = false;
                setLeftSmall1(slimes[i]);
            } else{
                setLeftSmall2(slimes[i]);
            }
        }
    }
    firstSmall = true;
    for (int i = 0; i < 3; ++i){
        if (i != inBattleRight && record[i + 3]){
            if (firstSmall){
                firstSmall = false;
                setRightSmall1(slimes[i + 3]);
            } else{
                setRightSmall2(slimes[i + 3]);
            }
        }
    }
}

std::vector<easy2d::Sprite*> initWea(){
    std::vector<easy2d::Sprite*> result;
    result.push_back(new easy2d::Sprite("Sunny.png"));
    result.push_back(new easy2d::Sprite("Rainy.png"));
    for (size_t i = 0; i < 2; ++i){
        result[i]->setAnchor(0.5f, 0.5f);
        result[i]->setScale(0.8f);
        result[i]->setPos(600, 40);
    }
    return result;
}

void updateWea(const BattleField& battleField, easy2d::Scene *scene,
               const std::vector<easy2d::Sprite*>& weathers, std::vector<bool>& weaRec){
    switch (battleField.getWeather()){
        case SUNNY:
            if (weaRec[1]){
                scene->removeChild(weathers[1]);
                weaRec[1] = false;
            }
            if (!weaRec[0]){
                scene->addChild(weathers[0]);
                weaRec[0] = true;
            }
            break;
        case RAINY:
            if (weaRec[0]){
                scene->removeChild(weathers[0]);
                weaRec[0] = false;
            }
            if (!weaRec[1]){
                scene->addChild(weathers[1]);
                weaRec[1] = true;
            }
            break;
        case TYPICAL:
            if (weaRec[0]){
                scene->removeChild(weathers[0]);
                weaRec[0] = false;
            }
            if (weaRec[1]){
                scene->removeChild(weathers[1]);
                weaRec[1] = false;
            }
            break;
        default:
            assert(false && "Not a valid weather and fail to update weather.");
            break;
    }
}

std::vector<easy2d::Sprite*> initCond(){
    std::vector<easy2d::Sprite*> result;
    for (int i = 0; i < 2; ++i){
        result.push_back(new easy2d::Sprite("IconSleep.png"));
        result.push_back(new easy2d::Sprite("IconPaint.png"));
    }
    for (size_t i = 0; i < 4; ++i){
        result[i]->setAnchor(0.5f, 0.5f);
        result[i]->setScale(0.8f);
    }
    result[0]->setPos(280, 70);
    result[1]->setPos(280, 100);
    result[2]->setPos(920, 70);
    result[3]->setPos(920, 100);
    return result;
}

void updateCond(const Slime& left, const Slime& right, easy2d::Scene *scene,
               const std::vector<easy2d::Sprite*>& conditions, std::vector<bool>& condRec){
    if (left.getSleepRound() > 0 && !condRec[0]){
        scene->addChild(conditions[0]);
        condRec[0] = true;
    } else if (left.getSleepRound() == 0 && condRec[0]){
        scene->removeChild(conditions[0]);
        condRec[0] = false;
    }
    if (left.isPainted() && !condRec[1]){
        scene->addChild(conditions[1]);
        condRec[1] = true;
    } else if (!left.isPainted() && condRec[1]){
        scene->removeChild(conditions[1]);
        condRec[1] = false;
    }
    if (right.getSleepRound() > 0 && !condRec[2]){
        scene->addChild(conditions[2]);
        condRec[2] = true;
    } else if (right.getSleepRound() == 0 && condRec[2]){
        scene->removeChild(conditions[2]);
        condRec[2] = false;
    }
    if (right.isPainted() && !condRec[3]){
        scene->addChild(conditions[3]);
        condRec[3] = true;
    } else if (!right.isPainted() && condRec[3]){
        scene->removeChild(conditions[3]);
        condRec[3] = false;
    }
}


std::vector<easy2d::ShapeNode*> initBottom(easy2d::Scene *scene){
    std::vector<easy2d::ShapeNode*> result;
    // 构造矩形
    auto rect = easy2d::gcnew easy2d::Shape(easy2d::Shape::Rect,
                                            easy2d::Rect(easy2d::Point(), easy2d::Size(200, 10)));
    // 堆上创建矩形节点并加入result向量
    for (size_t i = 0; i < 2; ++i){
        result.push_back(new easy2d::ShapeNode(rect));
        result[i]->setAnchor(0, 0.5f);
    }
    result[0]->setPos(300, 240);
    result[1]->setPos(700, 240);
    // 设置绘图风格，并加入场景子节点
    easy2d::DrawingStyle style;
    style.mode = easy2d::DrawingStyle::Mode::Fill;
    style.fillColor = easy2d::Color::Black;
    style.strokeColor = easy2d::Color::Black;
    style.strokeWidth = 5;
    for (size_t i = 0; i < 2; ++i){
        result[i]->setDrawingStyle(style);
        scene->addChild(result[i]);
    }
    return result;
}

std::vector<easy2d::ShapeNode*> initHPcol(easy2d::Scene *scene){
    std::vector<easy2d::ShapeNode*> result;
    // 构造矩形
    auto rect = easy2d::gcnew easy2d::Shape(easy2d::Shape::Rect,
                                            easy2d::Rect(easy2d::Point(), easy2d::Size(200, 10)));
    // 堆上创建矩形节点并加入result向量
    for (size_t i = 0; i < 2; ++i){
        result.push_back(new easy2d::ShapeNode(rect));
        result[i]->setAnchor(0, 0.5f);
    }
    result[0]->setPos(300, 240);
    result[1]->setPos(700, 240);
    // 设置绘图风格，并加入场景子节点
    easy2d::DrawingStyle style;
    style.mode = easy2d::DrawingStyle::Mode::Solid;
    style.fillColor = easy2d::Color::LightBlue;
    for (size_t i = 0; i < 2; ++i){
        result[i]->setDrawingStyle(style);
        scene->addChild(result[i]);
    }
    return result;
}

void updateHPcol(const Slime& left, const Slime& right, easy2d::Scene *scene,
                 std::vector<easy2d::ShapeNode*>& cols){
    // 先将原来血条删除
    for (size_t i = 0; i < 2; ++i){
        scene->removeChild(cols[i]);
        delete cols[i];
        cols[i] = nullptr;
    }
    // 添加新血条
    // 根据实际血量占最大血量的占比计算血条长度
    float leftLen = 200.0 * left.getHP() / maxHP(left.getElem()),
        rightLen = 200.0 * right.getHP() / maxHP(right.getElem());
    // 构造矩形
    auto leftRect = easy2d::gcnew easy2d::Shape(easy2d::Shape::Rect,
                                                easy2d::Rect(easy2d::Point(), easy2d::Size(leftLen, 10))),
        rightRect = easy2d::gcnew easy2d::Shape(easy2d::Shape::Rect,
                                                easy2d::Rect(easy2d::Point(), easy2d::Size(rightLen, 10)));
    cols[0] = new easy2d::ShapeNode(leftRect);
    cols[1] = new easy2d::ShapeNode(rightRect);
    cols[0]->setAnchor(0, 0.5f);
    cols[1]->setAnchor(0, 0.5f);
    cols[0]->setPos(300, 240);
    cols[1]->setPos(700, 240);
    // 设置绘图风格，并加入场景子节点
    easy2d::DrawingStyle style;
    style.mode = easy2d::DrawingStyle::Mode::Solid;
    style.fillColor = easy2d::Color::LightBlue;
    for (size_t i = 0; i < 2; ++i){
        cols[i]->setDrawingStyle(style);
        scene->addChild(cols[i]);
    }
}

std::vector<easy2d::Text*> initHPval(const Slime& left, const Slime& right, easy2d::Scene *scene){
    std::vector<easy2d::Text*> result;
    // 获取血量文本内容
    std::string leftCont = std::to_string(left.getHP()) + "/" + std::to_string(maxHP(left.getElem())),
        rightCont = std::to_string(right.getHP()) + "/" + std::to_string(maxHP(right.getElem()));
    result.push_back(new easy2d::Text(leftCont));
    result.push_back(new easy2d::Text(rightCont));
    result[0]->setAnchor(0.5f, 0.5f);
    result[1]->setAnchor(0.5f, 0.5f);
    result[0]->setPos(400, 265);
    result[1]->setPos(800, 265);
    // 设置文字颜色为黑色
    easy2d::DrawingStyle style;
    style.mode = easy2d::DrawingStyle::Mode::Solid;
    style.fillColor = easy2d::Color::Black;
    for (size_t i = 0; i < 2; ++i){
        result[i]->setDrawingStyle(style);
        scene->addChild(result[i]);
    }
    return result;
}

void updateHPval(const Slime& left, const Slime& right, easy2d::Scene *scene,
                 std::vector<easy2d::Text*>& vals){
    // 先将原来血量文本删除
    for (size_t i = 0; i < 2; ++i){
        scene->removeChild(vals[i]);
        delete vals[i];
        vals[i] = nullptr;
    }
    // 获取血量文本内容
    std::string leftCont = std::to_string(left.getHP()) + "/" + std::to_string(maxHP(left.getElem())),
        rightCont = std::to_string(right.getHP()) + "/" + std::to_string(maxHP(right.getElem()));
    vals[0] = new easy2d::Text(leftCont);
    vals[1] = new easy2d::Text(rightCont);
    vals[0]->setAnchor(0.5f, 0.5f);
    vals[1]->setAnchor(0.5f, 0.5f);
    vals[0]->setPos(400, 265);
    vals[1]->setPos(800, 265);
    // 设置文字颜色为黑色
    easy2d::DrawingStyle style;
    style.mode = easy2d::DrawingStyle::Mode::Solid;
    style.fillColor = easy2d::Color::Black;
    for (size_t i = 0; i < 2; ++i){
        vals[i]->setDrawingStyle(style);
        scene->addChild(vals[i]);
    }
}

void update(Player& player, Enemy& enemy, const BattleField& battleField, easy2d::Scene *scene,
            const std::vector<easy2d::Sprite*>& slimes, std::vector<bool>& record,
            const std::vector<easy2d::Sprite*>& weathers, std::vector<bool>& weaRec,
            const std::vector<easy2d::Sprite*>& conditions, std::vector<bool>& condRec,
            std::vector<easy2d::ShapeNode*>& cols, std::vector<easy2d::Text*>& vals){
    updateSlimes(player, enemy, scene, slimes, record);
    updateWea(battleField, scene, weathers, weaRec);
    updateCond(player.getSlime(), enemy.getSlime(), scene, conditions, condRec);
    updateHPcol(player.getSlime(), enemy.getSlime(), scene, cols);
    updateHPval(player.getSlime(), enemy.getSlime(), scene, vals);
    // 添加0秒定时器
    easy2d::Timer::add(easy2d::Game::quit, 0.0);
    easy2d::SceneManager::enter(scene);
    easy2d::Game::start();
}
