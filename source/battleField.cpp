#include "battleField.h"

// 初始天气为一般
BattleField::BattleField(): weather(TYPICAL), duration(0){}

Weather BattleField::getWeather() const{
    return weather;
}

void BattleField::setWeather(Weather wea, int time){
    assert(time >= 0 && "Duration is expected to be not less than 0.");
    weather = wea;
    duration = time;
}

void BattleField::proceed(std::ostream& os){
    if (weather != TYPICAL){
        if (duration == 1){
            os << "Weather becomes normal" << std::endl;
        }
        duration--;
    }
    if (duration == 0){
        weather = TYPICAL;
    }
}
