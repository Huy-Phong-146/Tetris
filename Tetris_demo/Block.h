#pragma once

#include "Utilities.h"

//=============================
// BaseBlock Class and Concrete Classes
//=============================
class BaseBlock {
public:
    int x;
    int y;
    vector<vector<char>> shape;
    Color blockColor;

    BaseBlock(Color blockColor);
    virtual ~BaseBlock() {}
    void rotate(const vector<vector<char>>& grid);
};

class BlockI : public BaseBlock {
public:
    BlockI();
};

class BlockO : public BaseBlock {
public:
    BlockO();
};

class BlockT : public BaseBlock {
public:
    BlockT();
};

class BlockL : public BaseBlock {
public:
    BlockL();
};

class BlockJ : public BaseBlock {
public:
    BlockJ();
};

class BlockS : public BaseBlock {
public:
    BlockS();
};

class BlockZ : public BaseBlock {
public:
    BlockZ();
};
