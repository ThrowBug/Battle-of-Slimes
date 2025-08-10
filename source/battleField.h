#ifndef BATTLEFIELD_H_INCLUDED
#define BATTLEFIELD_H_INCLUDED

#include "slime.h"
enum Weather{TYPICAL, SUNNY, RAINY};
class BattleField{
public:
    BattleField();

    // 获取天气
    Weather getWeather() const;

    // 设置天气以及持续时间
    void setWeather(Weather wea, int time);

    // 持续时间减1（一般天气除外），并在置零时回到一般天气，每回合结束时调用
    void proceed(std::ostream& os);
private:
    // 天气
    Weather weather;
    // 天气的持续时间
    int duration;

};

#endif // BATTLEFIELD_H_INCLUDED
