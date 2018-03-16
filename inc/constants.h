#ifndef CONSTANTS
#define CONSTANTS

#include <QColor>

const int RESOLUTION_X = 800;
const int RESOLUTION_Y = 600;
const int DIALOG_X = 800;
const int DIALOG_Y = 600;
const double RESOLUTION_RATIO = static_cast<double>(RESOLUTION_X)/static_cast<double>(RESOLUTION_Y);
const int MARGIN_SIZE = 24;
const int MAX_MAP_X = 40;
const int MAX_MAP_Y = 30;
const int TARGET_OPACITY = 80;
const int HOVER_OPACITY = 40;
const int ITERATIONS = 32;

const double MIN_REWARD = -1000.0;
const double MAX_REWARD = 1000.0;
const int REWARD_PRECISION = 3;
const double REWARD_STEP = 0.01;

const double MIN_PROBABILITY = 0.0;
const double MAX_PROBABILITY = 1.0;
const int PROBABILITY_PRECISION = 2;
const double PROBABILITY_STEP = 0.01;

const double MIN_DISCOUNTING = 0.01;
const double MAX_DISCOUNTING = 1.0;
const int DISCOUNTING_PRECISION = 2;
const double DISCOUNTING_STEP = 0.01;

const int NUMBER_OF_COLORS = 16;
const QColor colors[NUMBER_OF_COLORS] = {
    QColor(114,147,203), QColor(225,151,76), QColor(132,186,91), QColor(211,94,96),
    QColor(128,133,133), QColor(144,103,167), QColor(171,104,87), QColor(204,194,16),
    QColor(57,106,177), QColor(218,124,48), QColor(62,150,81), QColor(204,37,41),
    QColor(83,81,84), QColor(107,76,154), QColor(146,36,40), QColor(148,139,61)
};

#endif // CONSTANTS

