/*
 * ================================================================
 *  main.cpp  -  DIEM VAO CHUONG TRINH (MENU) (BẢN ĐÃ FIX C++11)
 * ================================================================
 */

#include "kh.h"

int main() {
    cout << "================================================\n";
    cout << "           MAZE GAME - C++ CLI                 \n";
    cout << "================================================\n";

    // FIX: Sử dụng std::pair thay cho C++17 Structured Bindings
    pair<int, int> startDiff = chooseDifficulty();
    Maze maze(startDiff.first, startDiff.second);
    maze.generate();
    maze.braid(0.06);

    while (true) {
        clearScreen();
        cout << "=== ME CUNG (" << maze.rows << " x " << maze.cols << ") ===\n";
        maze.display();
        cout << "\n===== MENU =====\n";
        cout << "1. Choi thu cong (W/A/S/D)\n";
        cout << "2. Giai tu dong bang BFS (duong ngan nhat)\n";
        cout << "3. Giai tu dong bang DFS (hien thi backtrack)\n";
        cout << "4. So sanh BFS vs DFS\n";
        cout << "5. Tao me cung moi / doi do kho\n";
        cout << "6. Thoat\n";
        cout << "Lua chon: ";

        int choice;
        if (!(cin >> choice)) return 0;

        Point start{ 0, 0 }, goal{ maze.rows - 1, maze.cols - 1 };

        if (choice == 1) {
            playManual(maze);
        }
        else if (choice == 2) {
            clearScreen();
            int visitedCount;
            auto path = maze.solveBFS(start, goal, &visitedCount);
            cout << "=== KET QUA BFS ===\n";
            maze.display(path);
            cout << "\nDo dai duong di: " << (path.size() - 1) << " buoc\n";
            cout << "So o da tham trong qua trinh tim kiem: " << visitedCount << "\n";
            cout << "\nNhan Enter de tiep tuc...";
            cin.ignore(); cin.get();
        }
        else if (choice == 3) {
            clearScreen();
            cout << "=== DANG GIAI BANG DFS (hien thi qua trinh) ===\n\n";
            int backtracks;
            auto path = maze.solveDFS(start, goal, true, &backtracks);
            cout << "\n=== KET QUA DFS ===\n";
            maze.display(path);
            cout << "\nDo dai duong di: " << (path.size() - 1) << " buoc\n";
            cout << "So lan backtrack: " << backtracks << "\n";
            cout << "\nNhan Enter de tiep tuc...";
            cin.ignore(); cin.get();
        }
        else if (choice == 4) {
            clearScreen();
            compareBFSvsDFS(maze);
        }
        else if (choice == 5) {
            // FIX: Sử dụng std::pair thay cho C++17 Structured Bindings
            pair<int, int> newDiff = chooseDifficulty();
            maze = Maze(newDiff.first, newDiff.second);
            maze.generate();
            maze.braid(0.06);
        }
        else if (choice == 6) {
            cout << "Tam biet!\n";
            break;
        }
    }
    return 0;
}