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

#include "GameSystem.h"

GameSystem::GameSystem() :
        _window(nullptr),
        _renderer(nullptr),
        _isRunning(false),
        _totalTime(0),
        _diff(0.0f),
        _paddle(),
        _ball() {
    this->Initialize();
    this->RunMainLoop();
}

bool GameSystem::Initialize(){

    int sdlInit = SDL_InitSubSystem(SDL_INIT_VIDEO);
    if(sdlInit != 0){
        SDL_Log("Unable to initialize SDL_INIT_VIDEO: %s ", SDL_GetError());
    }

    _window = SDL_CreateWindow(
        gameWindow,
        windowTopLeft,
        windowTopLeft,
        windowWidth,
        windowHeight,
        0
    );

    if(!_window){
        SDL_Log("Error while creating window: %s ", SDL_GetError());
        return false;
    }

    _renderer = SDL_CreateRenderer(
            _window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            );

    if(!_renderer){
        SDL_Log("Unable to create Renderer %s", SDL_GetError());
        return false;
    }

    this->SetObjectsData();

    SDL_Log("SDL Library, Video subsystem and Window successfully initialized.");
    return true;
}

void GameSystem::SetObjectsData() {

    _paddle.position.x = paddlePosX;
    _paddle.position.y = windowHeight/2.0;
    _paddle.direction = 0;
    _paddle.height = paddleHeight;
    _paddle.velocity = paddleVelocity;


    _ball.position.x = windowWidth/2.0;
    _ball.position.y = windowHeight/2.0;
    _ball.velocity.x = ballVelocityX;
    _ball.velocity.y = ballVelocityY;
    _ball.size = ballSize;
}
void GameSystem::ProcessInput() {

    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                _isRunning = false;
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);

    if(state[SDL_SCANCODE_ESCAPE])
        _isRunning = false;

    _paddle.direction = 0;

    if(state[SDL_SCANCODE_W])
        _paddle.direction |= DIRECTION_UP;

    if(state[SDL_SCANCODE_S])
        _paddle.direction |= DIRECTION_DOWN;

    int xCoef = _ball.velocity.x < 0 ? -1 : 1;
    int yCoef = _ball.velocity.y < 0 ? -1 : 1;

    int mPressed = state[SDL_SCANCODE_M] ? 1 : 0;
    int lPressed = state[SDL_SCANCODE_L] ? 1 : 0;

    if(mPressed || lPressed){
        _ball.velocity.x += static_cast<float>((mPressed - lPressed) * xCoef * 20);
        _ball.velocity.y += static_cast<float>((mPressed - lPressed) * yCoef * 20);
    }
}

void GameSystem::UpdateGame() {

    while(!SDL_TICKS_PASSED(SDL_GetTicks64(), _totalTime + 16))
        return;

    _diff = static_cast<float>(SDL_GetTicks64() - _totalTime) / 1000.0f;

    if(_diff > 0.5){
        _diff = 0.5f;
    }

    /*******************
     *    MOVEMENTS    *
     *******************/

    if(_paddle.direction & DIRECTION_UP){
        if(_paddle.position.y <= _paddle.height/2 + objectThickness)
            _paddle.position.y = _paddle.height/2 + objectThickness;
        else
            _paddle.position.y -= _paddle.velocity * _diff;
    }

    if(_paddle.direction & DIRECTION_DOWN){
        if(_paddle.position.y >= windowHeight - objectThickness - _paddle.height/2)
            _paddle.position.y = windowHeight - objectThickness - _paddle.height/2;
        else
            _paddle.position.y += _paddle.velocity * _diff;
    }

    _ball.position.x += _ball.velocity.x * _diff;
    _ball.position.y += _ball.velocity.y * _diff;

    /*******************
     * COLLISION CHECK *
     *******************/

    // Bottom wall
    if(_ball.velocity.y > 0 && _ball.position.y >= windowHeight - objectThickness
    - static_cast<float>(_ball.size)/2)
        _ball.velocity.y *= -1;

    // Top wall
    if(_ball.velocity.y < 0 && _ball.position.y <= objectThickness
     + static_cast<float>(_ball.size)/2)
        _ball.velocity.y *= -1;

    // Right wall
    if(_ball.velocity.x > 0 && _ball.position.x >= windowWidth -
                               objectThickness -
                               static_cast<float>(_ball.size)/2)
        _ball.velocity.x *= -1;

    if(_ball.velocity.x < 0 &&
    _ball.position.x <= paddlePosX + objectThickness + static_cast<float>(_ball.size)/2 &&
    abs(_paddle.position.y - _ball.position.y) <= paddleHeight/2 + static_cast<float>(_ball.size)/2)
        _ball.velocity.x *= -1;

    // Off-screen
    if(_ball.velocity.x < 0 &&
       _ball.position.x <= 0){
        _isRunning = false;
    }

    _totalTime = SDL_GetTicks64();

#ifdef DEBUG_MODE
    SDL_Log("Diff: %f | Total time elapsed since SDL initialization: %d", _diff, _totalTime);
#endif
}

void GameSystem::RenderOutput() {

    // Set draw color to blue
    SDL_SetRenderDrawColor(_renderer,0,0,0,0);

    // Clear back buffer
    SDL_RenderClear(_renderer);

#ifdef DEBUG_MODE
    RectObject<float> centerXLine(0, static_cast<float>(windowHeight)/2, windowWidth, 1);
    RectObject<float> centerYLine(static_cast<float>(windowWidth)/2, 0, 1, windowHeight);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(_renderer, &centerXLine);
    SDL_RenderFillRect(_renderer, &centerYLine);
#endif

    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

    RectObject<int> upperWall(0, 0, windowWidth, objectThickness);
    RectObject<int> bottomWall(0, windowHeight - objectThickness,
                               windowWidth, objectThickness);
    RectObject<int> rightWall(windowWidth - objectThickness,
                              0, objectThickness, windowHeight);

    RectObject<float> ball(_ball.position.x - static_cast<float>(_ball.size)/2,
                           _ball.position.y - static_cast<float>(_ball.size)/2,
                           _ball.size, _ball.size);

    RectObject<float> paddle(_paddle.position.x,
                             _paddle.position.y - _paddle.height/2,
                             objectThickness, static_cast<int>(_paddle.height));

#ifdef DEBUG_MODE
    SDL_Log("Actual Paddle data: (%f,%f). Width: %i | Height: %f", _paddle.position.x, _paddle.position.y, objectThickness, _paddle.height);
    SDL_Log("Drawing Paddle at: (%f,%f). Width: %i | Height: %f", _paddle.position.x, _paddle.position.y, objectThickness, _paddle.height);
#endif

    SDL_RenderFillRect(_renderer, &upperWall);
    SDL_RenderFillRect(_renderer, &bottomWall);
    SDL_RenderFillRect(_renderer, &rightWall);
    SDL_RenderFillRect(_renderer, &ball);
    SDL_RenderFillRect(_renderer, &paddle);

#ifdef DEBUG_MODE
    RectObject<float> paddlePosPoint(_paddle.position.x - 2, _paddle.position.y - 2, 4, 4); // -2, thickness half (4)
    RectObject<float> ballPosPoint(_ball.position.x - 2, _ball.position.y - 2, 4, 4); // -2, thickness half (4)
    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);
    SDL_RenderFillRect(_renderer, &paddlePosPoint);
    SDL_RenderFillRect(_renderer, &ballPosPoint);
#endif

    // Swap front buffer and back buffer
    SDL_RenderPresent(_renderer);
}

void GameSystem::RunMainLoop() {
    _isRunning = true;

    while(_isRunning){
        this->ProcessInput();
        this->UpdateGame();
        this->RenderOutput();
    }
}

void GameSystem::Shutdown() {
    SDL_Quit();
}