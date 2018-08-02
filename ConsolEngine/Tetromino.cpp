#include "Tetromino.h"

Tetromino::Tetromino(const Coordinates& aCoord, int deck_X, int deck_Y)
    : size_X_{deck_X}
    , size_Y_{deck_Y}
    , coord{aCoord}
    , deck{nullptr}
{
    initDeck();
}

Tetromino::Tetromino(const Coordinates& aCoord)    
    : size_X_{0}
    , size_Y_{0}
    , coord{aCoord}
    , deck{nullptr}
{
}

Tetromino::~Tetromino()
{
    clearDeck();
}

void Tetromino::initDeck()
{
    deck = new wchar_t*[size_X_];
    for (int i = 0; i < size_X_; i++) {
        deck[i] = new wchar_t[size_Y_];
    }

    for (int x = 0; x < size_X_; x++) {
        for (int y = 0; y < size_Y_; y++) {
            deck[x][y] = symbol::kEmpty;
        }
    }
}

void Tetromino::clearDeck()
{
    for (int i = 0; i < size_X_; i++)
        delete[] deck[i];
    delete[] deck;
}

void Tetromino::rotate()
{
    if (deck == nullptr)
        return;

    wchar_t** tempM = new wchar_t*[size_X_];
    for (int x = 0; x < size_X_; ++x) {
        tempM[x] = new wchar_t[size_Y_];
    }

    for (int x = 0; x < size_X_; ++x) {
        for (int y = 0; y < size_Y_; ++y) {
            tempM[x][y] = deck[x][y];
        }
    }

    clearDeck();
    int temp = size_X_;
    size_X_ = size_Y_;
    size_Y_ = temp;
    initDeck();

    for (int x = 0; x < size_X_; x++) {
        for (int y = 0; y < size_Y_; y++) {
            deck[x][y] = tempM[y][size_X_ - x - 1];
        }
    }

    for (int x = 0; x < size_Y_; x++)
        delete[] tempM[x];
    delete[] tempM;
}

void Tetromino::flip()
{
    int temp;
    for (int y = 0; y < size_Y_; y++) {
        for (int x = 0; x < size_X_ / 2; x++) {
            temp = deck[x][y];
            deck[x][y] = deck[size_X_ - x - 1][y];
            deck[size_X_ - x - 1][y] = temp;
        }
    }
}

I::I(int x, int y, bool aSide)
    : Tetromino{ Coordinates{ x, y }, 1, 4 }
    , side{false}
{
    deck[0][0] = symbol::kPiece;
    deck[0][1] = symbol::kPiece;
    deck[0][2] = symbol::kPiece;
    deck[0][3] = symbol::kPiece;

    if (aSide != side)
        rotate();
}

Tetromino* I::copy()
{
    return new I(coord.pos_X, coord.pos_Y, side);
}

void I::rotate()
{
    Tetromino::rotate();
    side = !side;
}

////////////////////////////////////////////////////////////

O::O(int x, int y)
    : Tetromino{ Coordinates{x, y }, 2, 2}
{
    deck[0][0] = symbol::kPiece;
    deck[0][1] = symbol::kPiece;
    deck[1][0] = symbol::kPiece;
    deck[1][1] = symbol::kPiece;
}

Tetromino* O::copy()
{
    return new O(coord.pos_X, coord.pos_Y);
}

void O::rotate() {}

////////////////////////////////////////////////////////////

T::T(int x, int y, int aSide)
    : Tetromino{ Coordinates{ x, y }, 3, 2 }
    , side{0}
{
    deck[1][1] = symbol::kPiece;
    deck[0][0] = symbol::kPiece;
    deck[1][0] = symbol::kPiece;
    deck[2][0] = symbol::kPiece;

    while (side < aSide) {
        rotate();
    }
}

Tetromino* T::copy()
{
    return new T(coord.pos_X, coord.pos_Y, side);
}

void T::rotate()
{
    Tetromino::rotate();
    if (++side > 3)
        side = 0;
}

////////////////////////////////////////////////////////////

Z::Z(int x, int y, int side, bool flip)
    : Tetromino{ Coordinates{ x, y }, 3, 2 }
    , mySide{ 0 }
    , myFlip{ flip }
{
    deck[0][0] = symbol::kPiece;
    deck[1][0] = symbol::kPiece;
    deck[1][1] = symbol::kPiece;
    deck[2][1] = symbol::kPiece;

    if (myFlip)
        Tetromino::flip();

    while (mySide < side) {
        rotate();
    }
}

Tetromino* Z::copy()
{
    return new Z(coord.pos_X, coord.pos_Y, mySide, myFlip);
}

void Z::rotate()
{
    Tetromino::rotate();
    if (++mySide > 3)
        mySide = 0;
}

////////////////////////////////////////////////////////////

L::L(int x, int y, int side, bool flip)
    : Tetromino{ Coordinates{x, y}, 2, 3 }
    , mySide{0}
    , myFlip{flip}
{
    deck[0][0] = symbol::kPiece;
    deck[0][1] = symbol::kPiece;
    deck[0][2] = symbol::kPiece;
    deck[1][2] = symbol::kPiece;

    if (myFlip)
        Tetromino::flip();

    while (mySide < side) {
        rotate();
    }
}

Tetromino* L::copy()
{
    return new L(coord.pos_X, coord.pos_Y, mySide, myFlip);
}

void L::rotate()
{
    Tetromino::rotate();
    if (++mySide > 3)
        mySide = 0;
}