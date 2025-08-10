#ifndef BATTLEFIELD_H_INCLUDED
#define BATTLEFIELD_H_INCLUDED

#include "slime.h"
enum Weather{TYPICAL, SUNNY, RAINY};
class BattleField{
public:
    BattleField();

    // ��ȡ����
    Weather getWeather() const;

    // ���������Լ�����ʱ��
    void setWeather(Weather wea, int time);

    // ����ʱ���1��һ���������⣩����������ʱ�ص�һ��������ÿ�غϽ���ʱ����
    void proceed(std::ostream& os);
private:
    // ����
    Weather weather;
    // �����ĳ���ʱ��
    int duration;

};

#endif // BATTLEFIELD_H_INCLUDED
