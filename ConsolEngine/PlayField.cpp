// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include "PlayField.h"
#include <cstdlib> // std::rand, std::exit
#include <ctime>   // std::time


PlayField::PlayField()
    : BaseApp{kPlayFieldWidth + 35, kPlayFieldHeight} // [50, 20]
    , tetromino_old_{nullptr}
    , current_time_{0.0f}
    , score_{0}
    , step_{kStep} 
{
    srand(static_cast<size_t>(0));
    next_tetromino_ = rand() % 5; // max 4 rotation
    next_flip_ = static_cast<bool>(rand() % 2); // true/false
    createTetromino();
}

void PlayField::KeyPressed(int btnCode)
{
    switch (btnCode)
    {
    case KeyCode::SPACE:
        clearOldStuff();
        if (canRotate())
            tetromino_->rotate();
        tetromino_old_ = tetromino_->copy();
        break;

    case KeyCode::ARROW_LEFT:
        if (emptyLeft())
            tetromino_->coord.pos_X--;
        break;

    case KeyCode::ARROW_RIGHT:
        if (emptyRight())
            tetromino_->coord.pos_X++;
        break;

    case KeyCode::ARROW_DOWN:
        // soft drop
        if (emptyDown()) {
            tetromino_->coord.pos_Y++;
            current_time_ = 0;
        } else {
            current_time_ = step_;
        } 
        break;

    case KeyCode::ENTER:
        // gravity
        while (emptyDown()) {
            tetromino_->coord.pos_Y++;
        }
        current_time_ = step_;
        break;
    }

    if (tetromino_->coord.pos_X < 0) {
        tetromino_->coord.pos_X = 0;
    } else if (tetromino_->coord.pos_X + tetromino_->getSizeX() > kPlayFieldWidth - 1) {
        tetromino_->coord.pos_X = kPlayFieldWidth - tetromino_->getSizeX();
    }
}

void PlayField::UpdateF(float deltaTime)
{
    clearOldStuff();
    removeRows();
    current_time_ += deltaTime;
    if (current_time_ > step_) {
        current_time_ = 0;

        if (emptyDown()) {
            tetromino_->coord.pos_Y++;
        } else {
            fixTetroOnTheDeck();
            if (!createTetromino())
                gameOver();
        }
    }

    drawBorders();
    
    tetromino_old_ = tetromino_->copy();
    drawCurrentTetromino(tetromino_);
    drawNextTetromino();

    displayStatistics();
}

bool PlayField::createTetromino()
{
    int aTetromino = next_tetromino_;
    next_tetromino_ = rand() % 5;
    int side = rand() % 4;
    bool flip = next_flip_;
    next_flip_ = static_cast<bool>(rand() % 2);

    switch (aTetromino)
    {
    case 0:
        tetromino_ = new O(START_X, START_Y);
        break;
    case 1:
        tetromino_ = new I(START_X, START_Y, static_cast<bool>((side>1) ? side - 2 : side));
        break;
    case 2:
        tetromino_ = new T(START_X, START_Y, side);
        break;
    case 3:
        tetromino_ = new L(START_X, START_Y, side, flip);
        break;
    case 4:
        tetromino_ = new Z(START_X, START_Y, side, flip);
        break;
    default:
        break;
    }

    for (int x = 0; x < tetromino_->getSizeX(); x++) {
        for (int y = 0; y < tetromino_->getSizeY(); y++) {
            if (GetChar(tetromino_->coord.pos_X + x, tetromino_->coord.pos_Y + y) != symbol::kEmpty)
                return false;
        }
    }
    tetromino_old_ = tetromino_->copy();

    return true;
}

void PlayField::drawBorders()
{
    for (int x = 0; x <= kPlayFieldWidth; ++x) {
        for (int y = 0; y <= kPlayFieldHeight; ++y) {
            SetChar(0, y, symbol::kBorderVerticle);  // left
            SetChar(kPlayFieldWidth, y, symbol::kBorderVerticle); // right
        }
    }
    
    // 3 lines
    for (int x = kPlayFieldWidth + 1; x < X_SIZE; ++x) {
        SetChar(x, 5, symbol::kBorderHorisontal);
        SetChar(x, 9, symbol::kBorderHorisontal);
        SetChar(x, 13, symbol::kBorderHorisontal);
    }
}

void PlayField::drawCurrentTetromino(const Tetromino* aTetromino)
{
    for (int x = 0; x < aTetromino->getSizeX(); ++x) {
        for (int y = 0; y < aTetromino->getSizeY(); ++y) {
            if (aTetromino->deck[x][y] != symbol::kEmpty) {
                SetChar(x + aTetromino->coord.pos_X,
                        y + aTetromino->coord.pos_Y,
                        aTetromino->deck[x][y]);
            }
        }
    }
}

void PlayField::drawNextTetromino()
{
    for (int y = 1; y < 5; y++)
        for (int x = X_SIZE - 6; x < X_SIZE; x++)
            SetChar(x, y, symbol::kEmpty);

    Tetromino* next_tetro = nullptr;
    switch (next_tetromino_)
    {
    case 0:
        next_tetro = new O(X_SIZE - 4, 2);
        break;
    case 1:
        next_tetro = new I(X_SIZE - 5, 3, true);
        break;
    case 2:
        next_tetro = new T(X_SIZE - 5, 2, 2);
        break;
    case 3:
        next_tetro = (next_flip_) ? new L(X_SIZE - 5, 2, 1, next_flip_)
                                  : new L(X_SIZE - 5, 2, 3, next_flip_);
        break;
    case 4:
        next_tetro = new Z(X_SIZE - 5, 2, 0, next_flip_);
        break;
    default:
        return;
        break;
    }

    drawCurrentTetromino(next_tetro);
}

void PlayField::displayStatistics()
{
    string info = to_string(step_);
    for (int i = 0; i < 6; ++i) {
        SetChar(X_SIZE - 6 + i, 7, *(L"SPEED:" + i));
    }

    for (int i = 0; i < 5; i++) {
        SetChar(X_SIZE - 6 + i, 8, info[i]);
    }
    SetChar(X_SIZE - 1, 8, 's');


    info = to_string(score_);
    for (int i = 0; i < 6; ++i) {
        SetChar(X_SIZE - 6 + i, 11, *(L"SCORE:" + i));
    }

    for (size_t i = 0; i < info.length(); ++i) {
        SetChar(X_SIZE - 4 + i, 12, info[i]);
    }
}

void PlayField::clearOldStuff()
{
    for (int x = 0; x < tetromino_old_->getSizeX(); ++x) {
        for (int y = 0; y < tetromino_old_->getSizeY(); ++y) {
            if (tetromino_old_->deck[x][y] != symbol::kEmpty) {
                SetChar(x + tetromino_old_->coord.pos_X,
                        y + tetromino_old_->coord.pos_Y,
                        symbol::kEmpty);
            }
        }
    }
}

void PlayField::fixTetroOnTheDeck()
{
    clearOldStuff();
    for (int x = 0; x < tetromino_->getSizeX(); ++x) {
        for (int y = 0; y < tetromino_->getSizeY(); ++y) {
            if (tetromino_->deck[x][y] != symbol::kEmpty) {
                SetChar(x + tetromino_->coord.pos_X,
                        y + tetromino_->coord.pos_Y,
                        symbol::kPiece);
            }
        }
    }

    delete tetromino_;
    tetromino_ = nullptr;
    delete tetromino_old_;
    tetromino_old_ = nullptr;
}
bool PlayField::emptyDown()
{
    if (tetromino_->coord.pos_Y + tetromino_->getSizeY() > Y_SIZE)
        return false;

    for (int x = 0; x < tetromino_->getSizeX(); ++x) {
        for (int y = tetromino_->getSizeY() - 1; y >= 0; --y) {
            if (tetromino_->deck[x][y] != symbol::kEmpty) {
                if (GetChar(tetromino_->coord.pos_X + x, tetromino_->coord.pos_Y + y + 1) != symbol::kEmpty)
                    return false;
                else
                    break;
            }
        }
    }
    return true;
}

bool PlayField::emptyRight()
{
    for (int y = 0; y < tetromino_->getSizeY(); ++y) {
        for (int x = tetromino_->getSizeX() - 1; x >= 0; --x) {
            if (tetromino_->deck[x][y] != symbol::kEmpty) {
                if (GetChar(tetromino_->coord.pos_X + x + 1, tetromino_->coord.pos_Y + y) == symbol::kPiece)
                    return false;
                else
                    break;
            }
        }
    }
    return true;
}

bool PlayField::emptyLeft()
{
    for (int y = 0; y < tetromino_->getSizeY(); ++y) {
        for (int x = 0; x < tetromino_->getSizeX(); ++x) {
            if (tetromino_->deck[x][y] != symbol::kEmpty) {
                if (GetChar(tetromino_->coord.pos_X + x - 1, tetromino_->coord.pos_Y + y) != symbol::kEmpty)
                    return false;
                else
                    break;
            }
        }
    }
    return true;
}

bool PlayField::canRotate()
{
    Tetromino* tmp = tetromino_->copy();
    tmp->rotate();
    for (int y = 0; y < tmp->getSizeY(); ++y) {
        for (int x = 0; x < tmp->getSizeX(); ++x) {
            if (tmp->deck[x][y] != symbol::kEmpty &&
                GetChar(tmp->coord.pos_X + x, tmp->coord.pos_Y + y) == symbol::kPiece) {
                return false;
            }
        }
    }
    return true;
}

bool PlayField::removeRows()
{
    bool filledString;
    for (int y = Y_SIZE; y > 0; --y) {
        filledString = true;
        for (int x = 0; x < kPlayFieldWidth; ++x) {
            if (GetChar(x, y) == symbol::kEmpty) {
                filledString = false;
                break;
            }
        }

        if (!filledString)
            continue;

        for (int x = 1; x < kPlayFieldWidth; ++x) {
            SetChar(x, y, symbol::kEmpty);
        }

        for (int i = y; i > 1; i--) {
            for (int x = 0; x < kPlayFieldWidth; ++x) {
                SetChar(x, i, GetChar(x, i - 1));
            }
        }

        ++score_;
        step_ *= kQuickStep;
        return true;
    }
    return false;
}

void PlayField::gameOver()
{
    system("cls");
    std::cout << "\t\tG A M E  O V E R\n\n\t\t    Score: " << score_ << std::endl;
    cin.ignore();
    exit(0);
}