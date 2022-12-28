#ifndef POS_H
#define POS_H

#include <algorithm>

struct Pos{
    Pos() : row(-1), col(-1) {}
    Pos(int r, int c) : row(r), col(c) {}
    int row;
    int col;

    bool operator==(const Pos& other) const{
        return row == other.row && col == other.col;
    }

    std::string toString(){
        return std::to_string(row * 8 + col);
    }
};

struct PosHash{
    std::size_t operator()(const Pos& pos) const
    {
      using std::hash;
      return ((hash<int>()(pos.row)
               ^ (hash<int>()(pos.col) << 1)) >> 1);
    }
};


#endif // POS_H
