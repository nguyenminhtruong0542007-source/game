/*
 * ================================================================
 *  kh.h  -  FILE KHAI BAO (HEADER)
 * ================================================================
 *  Chua khai bao cac struct, class va prototype ham dung chung
 *  cho ca chuong trinh Maze Game.
 *  Phan CAI DAT (dinh nghia chi tiet) nam trong caidat.cpp
 * ================================================================
 */

#ifndef KH_H
#define KH_H

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <string>

using namespace std;

// ---------------------------------------------------------------
// Huong di chuyen: 0 = Bac(N), 1 = Nam(S), 2 = Dong(E), 3 = Tay(W)
// ---------------------------------------------------------------
extern const int DR[4];
extern const int DC[4];

// ---------------------------------------------------------------
// Cau truc 1 o (cell) trong me cung
// wall = true nghia la CON tuong (khong di qua duoc)
// ---------------------------------------------------------------
struct Cell {
    bool visited = false;
    bool N = true, S = true, E = true, W = true;
};

// ---------------------------------------------------------------
// Toa do 1 o trong luoi me cung
// ---------------------------------------------------------------
struct Point {
    int r, c;
    bool operator==(const Point& o) const { return r == o.r && c == o.c; }
};

// =================================================================
// LOP Maze - CAU TRUC DU LIEU CHINH: 2D ARRAY (grid) LUU ME CUNG
// =================================================================
class Maze {
public:
    int rows, cols;
    vector<vector<Cell>> grid;   // 2D Array luu cau truc me cung
    mt19937 rng;

    Maze(int r, int c);

    bool inBounds(int r, int c) const;
    void removeWall(int r, int c, int dir);
    bool wallExists(int r, int c, int dir) const;
    bool canMove(int r, int c, int dir) const;

    // 1) Tao me cung bang DFS + Stack (Recursive Backtracker)
    void generate(bool verbose = false);

    // Pha them vai buc tuong ngau nhien de tao vong lap (loop)
    // giup phan so sanh BFS vs DFS co y nghia
    void braid(double extraPassageChance = 0.08);

    // 2) Hien thi me cung dang ASCII
    void display(const vector<Point>& path = {}, Point* playerPos = nullptr) const;

    // 4) Giai me cung bang BFS (dung Queue) - duong ngan nhat
    vector<Point> solveBFS(Point start, Point goal, int* visitedCount = nullptr) const;

    // 5) Giai me cung bang DFS (dung Stack) - co backtracking
    vector<Point> solveDFS(Point start, Point goal, bool verbose, int* backtrackCount = nullptr) const;
};

// ---------------------------------------------------------------
// Cac ham tien ich (utility) dung chung
// ---------------------------------------------------------------
char getKeyPress();                 // doc 1 phim bam ngay, khong can Enter
void clearScreen();                 // xoa man hinh terminal
void sleepMs(int ms);                // tam dung (dung khi minh hoa DFS)

// ---------------------------------------------------------------
// Cac ham thao tac o muc "man hinh / che do choi" (dung trong main)
// ---------------------------------------------------------------
pair<int, int> chooseDifficulty();      // 7) chon do kho / kich thuoc me cung
void playManual(Maze& maze);            // 3) nguoi choi tu giai bang W/A/S/D
void compareBFSvsDFS(Maze& maze);       // 6) so sanh BFS vs DFS

#endif // KH_H
