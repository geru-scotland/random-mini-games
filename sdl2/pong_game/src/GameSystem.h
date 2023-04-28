/*
 * This file is part of the Random-mini-games project.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * @authors - Geru-Scotland (https://github.com/geru-scotland)
 */

#ifndef PONG_GAME_GAMESYSTEM_H
#define PONG_GAME_GAMESYSTEM_H

#include "SDL2/SDL.h"

#pragma once

const char* const gameWindow = "Pong v1";
const int windowWidth = 1024;
const int windowHeight = 768;
const int windowTopLeft = 100;

/* PADDLE */
const int paddlePosX = 10;
const float paddleHeight = 80.0f;
const float paddleVelocity = 300.0f;

/* BALL */
const int ballVelocityX = 200;
const int ballVelocityY = -235;
const int ballSize = 16;

/* GENERIC OBJECT */
const int objectThickness = 16;

struct Vector{
    float x;
    float y;
};

struct BallData{
    Vector position;
    Vector velocity;
    int size;
};

struct PaddleData{
    Vector position;
    uint8_t direction;
    float velocity;
    float height;
};

template<typename T>
struct RectObject : SDL_Rect {
    RectObject(T _x, T _y, int width, int height) : SDL_Rect(){
        x = static_cast<int>(_x);
        y = static_cast<int>(_y);
        w = width;
        h = height;
    };
};

enum Directions {
    DIRECTION_UP = 1,
    DIRECTION_DOWN = 2
};

class GameSystem {

public:
    GameSystem();
    bool Initialize();
    void Shutdown();

private:
    void SetObjectsData();
    void RunMainLoop();
    void ProcessInput();
    void UpdateGame();
    void RenderOutput();

    SDL_Window* _window;
    SDL_Renderer* _renderer;
    bool _isRunning;
    Uint64 _totalTime;
    float _diff;

    PaddleData _paddle;
    BallData _ball;
};

#endif //PONG_GAME_GAMESYSTEM_H