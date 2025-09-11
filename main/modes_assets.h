#ifndef MODES_ASSETS_H
#define MODES_ASSETS_H

#include "display_assets.h"

typedef struct {
    int year;
    int month;
    int month_day;
    int week_day;
    int hour;
    int minute;
    int second;
} ModeState_Clock;

typedef struct {
    int month;
    int month_day;
    int week_day;
    int hour;
    int minute;
    
} ModeState_ClockWeather;

typedef struct {
    void;
} ModeState_Weather;

#endif