#pragma once

namespace symbol {

const wchar_t kBorderVerticle = L'|';
const wchar_t kBorderHorisontal = L'_';
const wchar_t kEmpty = L' ';
const wchar_t kPiece = L'■';

} // symbol

struct Coordinates {
    int pos_X;
    int pos_Y;
    Coordinates()
        : pos_X{0}, pos_Y{0}
    {}

    Coordinates(int x, int y)
        : pos_X{x}, pos_Y{y}
    {}
};

class Tetromino
{
public:
    Tetromino(const Coordinates& aCoord, int x, int y);
    virtual ~Tetromino();

    virtual Tetromino* copy() = 0;
    virtual void rotate() = 0;

    int getSizeX() const { return size_X_; }
    int getSizeY() const { return size_Y_; }

    Coordinates coord;
    wchar_t** deck;

protected:
    Tetromino(const Coordinates& aCoord);
    void clearDeck();
    void initDeck();
    void flip();

    int size_X_;
    int size_Y_;
};


class I : public Tetromino
{
public:
    I(int x, int y, bool aSide = false);
    virtual Tetromino* copy() override;
    virtual void rotate() override;
    bool side;
};

class O : public Tetromino
{
public:
    O(int x, int y);
    virtual Tetromino* copy() override;
    virtual void rotate() override;
};


class T : public Tetromino
{
public:
    T(int x, int y, int aSide = 0);
    virtual Tetromino* copy() override;
    virtual void rotate() override;
    int side;
};

class Z : public Tetromino
{
public:
    Z(int, int, int = 0, bool = 0);
    virtual Tetromino* copy() override;
    virtual void rotate() override;
    int mySide;
    const bool myFlip;
};

class L : public Tetromino
{
public:
    L(int, int, int = 0, bool = 0);
    virtual Tetromino* copy() override;
    virtual void rotate() override;
    int mySide;
    const bool myFlip;
};


