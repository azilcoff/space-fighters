#pragma once

template <typename Type>
constexpr Type WINDOW_WIDTH = Type(1200);
template <typename Type>
constexpr Type WINDOW_HEIGHT = Type(800);
constexpr float PLAYER_SHIP_WIDTH = 80.0f;
constexpr float PLAYER_SHIP_HEIGHT = 150.0f;
constexpr float PLAYER_START_Y = WINDOW_HEIGHT<float> / 2.0f - PLAYER_SHIP_HEIGHT / 2.0f;
constexpr float PLAYER_SPEED = 10.0f;
constexpr float LAZER_WIDTH = 120.0f;
constexpr float LAZER_HEIGHT = 25.0f;
constexpr float LAZER_SPEED = 12.0f;