// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#pragma once

#include "BaseApp.h"
#include "Tetromino.h"

const int kPlayFieldWidth = 15;
const int kPlayFieldHeight = 20;
const int START_X = 7;  
const int START_Y = 1;
const float kStep = 1.0f;
const float kQuickStep = 0.8f;

enum KeyCode {
    SPACE       = 32,
    ENTER       = 13,
    ARROW_LEFT  = 75,
    ARROW_RIGHT = 77,
    ARROW_DOWN  = 80
};

class PlayField : public BaseApp
{
public:
    PlayField();
    virtual void KeyPressed(int btnCode) override;
    virtual void UpdateF(float deltaTime) override;

    void drawBorders();
    
    bool createTetromino();
    void drawCurrentTetromino(const Tetromino* tetromino);
    void drawNextTetromino();
    
    void clearOldStuff();
    void fixTetroOnTheDeck();

    bool emptyLeft();
    bool emptyRight();
    bool emptyDown();

    bool canRotate();
    bool removeRows();

    void displayStatistics();
    void gameOver();

private:
    Tetromino* tetromino_;
    Tetromino* tetromino_old_;

    float current_time_;
    int next_tetromino_;
    bool next_flip_;
    int score_;
    float step_;
};
