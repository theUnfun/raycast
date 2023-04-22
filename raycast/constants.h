#pragma once

constexpr uint32_t WINDOW_WIDTH = 800;
constexpr uint32_t WINDOW_HEIGHT = 800;

constexpr float pi = 3.1415926535f;

constexpr float MOVEMENT_SPEED = 200.;
constexpr float ROTATION_SPEED = 100.;
constexpr float VISION_RANGE = 500.;

constexpr float FOV_HORIZONTAL = 80.0f;
constexpr float FOV_VERTICAL = 60.0f;

constexpr float WALL_HEIGHT = 100.0f;
constexpr uint32_t NUMBER_OF_RAYS = WINDOW_WIDTH / 8;

constexpr float EPSILON = 1e-4f;
// constexpr float NUMBER_OF_RAYS = 8;