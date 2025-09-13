#ifndef MODES_ASSETS_H
#define MODES_ASSETS_H

enum current_mode {
    MODE_MENU,
    MODE_CLOCK,
    MODE_CLOCKWEATHER,
    MODE_WEATHER,
};
typedef enum current_mode current_mode_t;

typedef struct {
    int selection;
} modestate_menu_t;

typedef struct {
    int year;
    int month;
    int month_day;
    int week_day;
    int hour;
    int minute;
    int second;
} modestate_clock_t;

typedef struct {
    int month;
    int month_day;
    int week_day;
    int hour;
    int minute;
    int temp_f;
    int weather_code;
    int temp_high_f;
    int temp_low_f;
} modestate_clockweather_t;

typedef struct {
    int temp_f;
    int weather_code;
    int temp_high_f;
    int temp_low_f;
    int sunset_hour;
    int sunset_minute;
} modestate_weather_t;

typedef struct {
    modestate_menu_t menu;
    modestate_clock_t clock;
    modestate_clockweather_t clockweather;
    modestate_weather_t weather;
} modestates_t;

#endif