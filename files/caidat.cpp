/*
 * ================================================================
 *  caidat.cpp  -  FILE CAI DAT (IMPLEMENTATION)
 * ================================================================
 *  Dinh nghia chi tiet cho tat ca struct/class/ham da khai bao
 *  trong kh.h
 * ================================================================
 */

#include "kh.h"

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

// dx, dy tuong ung 4 huong: N, S, E, W
const int DR[4] = {-1, 1, 0, 0};
const int DC[4] = {0, 0, 1, -1};

// ---------------------------------------------------------------
// Doc 1 phim bam ngay lap tuc, khong can Enter (che do raw mode)
// ---------------------------------------------------------------
char getKeyPress() {
#ifdef _WIN32
    return static_cast<char>(_getch());
#else
    termios oldt{}, newt{};
    // Neu stdin khong phai la terminal thuc su (vd bi pipe/redirect)
    // thi fallback ve doc binh thuong bang cin de khong bi loi.
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
        char c;
        cin >> c;
        return c;
    }
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char ch = static_cast<char>(getchar());
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[H"; // ANSI escape: xoa man hinh + ve goc tren
#endif
}

void sleepMs(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

// =================================================================
//                      CAI DAT LOP Maze
// =================================================================

Maze::Maze(int r, int c) : rows(r), cols(c) {
    random_device rd;
    rng.seed(rd());
    grid.assign(rows, vector<Cell>(cols));
}

bool Maze::inBounds(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

// Go tuong giua cell (r,c) va cell lang gieng theo huong dir
void Maze::removeWall(int r, int c, int dir) {
    int nr = r + DR[dir], nc = c + DC[dir];
    if (dir == 0) { grid[r][c].N = false; grid[nr][nc].S = false; }
    if (dir == 1) { grid[r][c].S = false; grid[nr][nc].N = false; }
    if (dir == 2) { grid[r][c].E = false; grid[nr][nc].W = false; }
    if (dir == 3) { grid[r][c].W = false; grid[nr][nc].E = false; }
}

bool Maze::wallExists(int r, int c, int dir) const {
    if (dir == 0) return grid[r][c].N;
    if (dir == 1) return grid[r][c].S;
    if (dir == 2) return grid[r][c].E;
    return grid[r][c].W;
}

bool Maze::canMove(int r, int c, int dir) const {
    int nr = r + DR[dir], nc = c + DC[dir];
    if (!inBounds(nr, nc)) return false;
    return !wallExists(r, c, dir);
}

// =============================================================
// 1) TAO ME CUNG: RECURSIVE BACKTRACKER (DFS dung Stack)
//
// Y tuong thuat toan:
//  - Coi moi o (cell) la 1 dinh trong do thi luoi, ban dau tat
//    ca cac o deu duoc bao boc boi tuong (wall = true).
//  - Bat dau tu 1 o goc, danh dau da tham (visited) va day
//    (push) vao Stack.
//  - Lap: nhin o dang o dinh Stack (top). Tim cac o lang gieng
//    CHUA duoc tham.
//      + Neu co: chon ngau nhien 1 o lang gieng chua tham, PHA
//        TUONG giua o hien tai va o do (tao loi di), danh dau
//        o lang gieng la visited, day no vao Stack (di tiep).
//      + Neu KHONG con lang gieng nao chua tham (ngo cut): POP
//        o hien tai ra khoi Stack -> "quay lui" (backtrack) ve
//        o truoc do de tiep tuc tim nhanh khac.
//  - Thuat toan dung khi Stack rong -> moi o da duoc tham.
//
// Vi sao ket qua la 1 "me cung hoan hao" (perfect maze)?
//  - Moi lan pha tuong, ta chi noi 1 o MOI (chua tham) voi o
//    hien tai => khong bao gio tao vong lap (cycle).
//  - Tat ca N = rows*cols o deu duoc tham va noi voi dung N-1
//    canh (khong cycle, lien thong) => cau truc me cung chinh
//    la 1 CAY BAO TRUM (spanning tree).
//  - He qua: giua 2 o bat ky chi co DUY NHAT 1 duong di don.
//    Day la ly do neu khong "pha them tuong" (braiding), BFS
//    va DFS se cho ra CUNG MOT duong di (xem ham braid ben duoi).
// =============================================================
void Maze::generate(bool verbose) {
    grid.assign(rows, vector<Cell>(cols)); // reset toan bo luoi

    stack<Point> st;
    Point start{0, 0};
    grid[0][0].visited = true;
    st.push(start);

    while (!st.empty()) {
        Point cur = st.top();

        // Tim tat ca huong dan toi o lang gieng CHUA tham
        vector<int> dirs;
        for (int d = 0; d < 4; d++) {
            int nr = cur.r + DR[d], nc = cur.c + DC[d];
            if (inBounds(nr, nc) && !grid[nr][nc].visited) dirs.push_back(d);
        }

        if (!dirs.empty()) {
            shuffle(dirs.begin(), dirs.end(), rng); // chon ngau nhien
            int d = dirs[0];
            int nr = cur.r + DR[d], nc = cur.c + DC[d];

            removeWall(cur.r, cur.c, d);          // pha tuong -> tao duong
            grid[nr][nc].visited = true;
            st.push({nr, nc});                    // di toi (tien ve phia truoc)

            if (verbose) cout << "Di toi (" << nr << "," << nc << ")\n";
        } else {
            st.pop();                             // ngo cut -> quay lui
            if (verbose) cout << "Ngo cut, quay lui tu ("
                               << cur.r << "," << cur.c << ")\n";
        }
    }

    // reset lai visited de dung rieng cho buoc giai me cung sau nay
    for (auto& row : grid) for (auto& cell : row) cell.visited = false;
}

// Them mot vai duong "tat" (loai bo them vai buc tuong ngau nhien)
// de me cung khong con la cay bao trum tuyet doi nua, ma xuat
// hien vong lap (loops). Khi do co nhieu hon 1 duong di giua 2
// diem -> phan "So sanh BFS vs DFS" moi thuc su co y nghia (BFS
// luon tim duong NGAN NHAT, DFS co the tra ve duong DAI HON vi
// no chi di theo nhanh dau tien tim thay).
void Maze::braid(double extraPassageChance) {
    uniform_real_distribution<double> prob(0.0, 1.0);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            for (int d : {2, 1}) { // chi xet E, S de khong xu ly trung lap
                int nr = r + DR[d], nc = c + DC[d];
                if (!inBounds(nr, nc)) continue;
                if (wallExists(r, c, d) && prob(rng) < extraPassageChance) {
                    removeWall(r, c, d);
                }
            }
        }
    }
}

// =============================================================
// 2) HIEN THI ME CUNG DANG ASCII
//
// Kich thuoc luoi hien thi: (2*rows+1) x (2*cols+1)
// Moi cell (r,c) chiem 1 diem "tam" tai vi tri (2r+1, 2c+1).
// Giua 2 cell la vi tri "tuong" - in '#' neu con tuong, ' ' neu
// da bi pha (co the di qua).
//
// path      : danh dau duong di (vd ket qua BFS/DFS) bang '.'
// playerPos : vi tri nguoi choi hien tai, in bang '@'
// =============================================================
void Maze::display(const vector<Point>& path, Point* playerPos) const {
    int H = 2 * rows + 1, W = 2 * cols + 1;
    vector<string> canvas(H, string(W, '#'));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int gr = 2 * r + 1, gc = 2 * c + 1;
            canvas[gr][gc] = ' ';
            if (!grid[r][c].N) canvas[gr - 1][gc] = ' ';
            if (!grid[r][c].S) canvas[gr + 1][gc] = ' ';
            if (!grid[r][c].E) canvas[gr][gc + 1] = ' ';
            if (!grid[r][c].W) canvas[gr][gc - 1] = ' ';
        }
    }

    for (auto& p : path) {
        int gr = 2 * p.r + 1, gc = 2 * p.c + 1;
        canvas[gr][gc] = '.';
    }

    // diem bat dau / dich
    canvas[1][1] = 'S';
    canvas[2 * rows - 1][2 * cols - 1] = 'E';

    if (playerPos) {
        int gr = 2 * playerPos->r + 1, gc = 2 * playerPos->c + 1;
        canvas[gr][gc] = '@';
    }

    for (auto& row : canvas) cout << row << "\n";
}

// =============================================================
// 4) GIAI ME CUNG BANG BFS (dung Queue) - LUON tra ve DUONG
//    NGAN NHAT
//
// Vi sao BFS dam bao duong ngan nhat?
//  - Me cung la 1 do thi khong trong so (moi canh = 1 buoc di,
//    chi phi bang nhau).
//  - BFS duyet do thi theo TUNG LOP KHOANG CACH: dau tien no
//    tham het cac dinh cach diem xuat phat DUNG 1 buoc, roi moi
//    den cac dinh cach 2 buoc, roi 3 buoc,... (nho dung Queue -
//    FIFO: vao truoc ra truoc).
//  - Vi vay, DINH DAU TIEN ma BFS cham toi dich (goal) chinh la
//    o LOP KHOANG CACH NHO NHAT co the co -> duong di theo do
//    (truy vet qua mang "parent") chac chan la NGAN NHAT.
//  - BFS khong bao gio "di lech" sang nhanh xa hon truoc khi thu
//    het cac kha nang gan hon, nen no khong the bo lo 1 duong
//    ngan hon ma no chua kip xet.
//  - Do phuc tap: O(V + E) voi V = so o, E = so canh (loi di).
// =============================================================
vector<Point> Maze::solveBFS(Point start, Point goal, int* visitedCount) const {
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<Point>> parent(rows, vector<Point>(cols, {-1, -1}));

    queue<Point> q;
    q.push(start);
    visited[start.r][start.c] = true;
    int steps = 0;

    while (!q.empty()) {
        Point cur = q.front(); q.pop();
        steps++;
        if (cur == goal) break;

        for (int d = 0; d < 4; d++) {
            if (!canMove(cur.r, cur.c, d)) continue;
            int nr = cur.r + DR[d], nc = cur.c + DC[d];
            if (!visited[nr][nc]) {
                visited[nr][nc] = true;
                parent[nr][nc] = cur;
                q.push({nr, nc});
            }
        }
    }

    if (visitedCount) *visitedCount = steps;

    vector<Point> path;
    if (!visited[goal.r][goal.c]) return path; // khong tim thay

    Point cur = goal;
    while (!(cur == start)) {
        path.push_back(cur);
        cur = parent[cur.r][cur.c];
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

// =============================================================
// 5) GIAI ME CUNG BANG DFS (dung Stack) - CO HIEN THI QUA TRINH
//    BACKTRACK
//
// Khac voi BFS, DFS di THEO 1 NHANH DUY NHAT cho den khi:
//   - gap dich (thanh cong), hoac
//   - gap ngo cut (khong con lang gieng chua tham) -> POP khoi
//     stack de "quay lui" (backtrack) ve o truoc, thu nhanh khac.
// Vi DFS chi quan tam "co duong di hay khong" (khong quan tam
// buoc gan/xa), duong DFS tim duoc CO THE dai hon duong BFS neu
// me cung co nhieu hon 1 duong di (co vong lap / loop). Neu me
// cung la "perfect maze" (khong braid) thi vi chi co DUY NHAT 1
// duong di giua 2 diem, DFS va BFS se cho CUNG 1 ket qua.
// =============================================================
vector<Point> Maze::solveDFS(Point start, Point goal, bool verbose, int* backtrackCount) const {
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<Point> path; // dong thoi dong vai tro "stack" duong di hien tai
    path.push_back(start);
    visited[start.r][start.c] = true;
    int backtracks = 0;

    while (!path.empty()) {
        Point cur = path.back();
        if (cur == goal) break;

        bool moved = false;
        for (int d = 0; d < 4; d++) {
            if (!canMove(cur.r, cur.c, d)) continue;
            int nr = cur.r + DR[d], nc = cur.c + DC[d];
            if (!visited[nr][nc]) {
                visited[nr][nc] = true;
                path.push_back({nr, nc});
                moved = true;
                if (verbose) {
                    cout << "  -> Di toi (" << nr << "," << nc << ")\n";
                    sleepMs(15);
                }
                break;
            }
        }

        if (!moved) {
            backtracks++;
            if (verbose) {
                cout << "  <- BACKTRACK tu (" << cur.r << "," << cur.c << ")\n";
                sleepMs(15);
            }
            path.pop_back();
        }
    }

    if (backtrackCount) *backtrackCount = backtracks;
    return path; // neu rong -> khong tim thay (khong xay ra vi maze lien thong)
}

// =================================================================
//              CAC HAM MUC "MAN HINH / CHE DO CHOI"
// =================================================================

// 7) In danh sach lua chon do kho, tra ve (rows, cols)
pair<int, int> chooseDifficulty() {
    cout << "\n=== CHON DO KHO ===\n";
    cout << "1. De     (10 x 10)\n";
    cout << "2. Vua    (15 x 15)\n";
    cout << "3. Kho    (25 x 25)\n";
    cout << "4. Cuc kho(40 x 40)\n";
    cout << "5. Tuy chinh...\n";
    cout << "Lua chon: ";
    int choice; cin >> choice;
    switch (choice) {
        case 1: return {10, 10};
        case 2: return {15, 15};
        case 3: return {25, 25};
        case 4: return {40, 40};
        case 5: {
            int r, c;
            cout << "Nhap so hang (rows): "; cin >> r;
            cout << "Nhap so cot (cols): "; cin >> c;
            r = max(3, min(80, r));
            c = max(3, min(80, c));
            return {r, c};
        }
        default: return {15, 15};
    }
}

// 3) Nguoi choi tu giai bang W/A/S/D
void playManual(Maze& maze) {
    Point player{0, 0};
    Point goal{maze.rows - 1, maze.cols - 1};
    int moves = 0;

    while (true) {
        clearScreen();
        cout << "=== CHOI THU CONG (W=len A=trai S=xuong D=phai, Q=thoat) ===\n";
        cout << "So buoc da di: " << moves << "\n\n";
        maze.display({}, &player);

        if (player == goal) {
            cout << "\n*** CHUC MUNG! Ban da thoat me cung sau " << moves << " buoc! ***\n";
            int bfsSteps;
            auto bfsPath = maze.solveBFS({0, 0}, goal, &bfsSteps);
            cout << "(Duong ngan nhat theo BFS chi can " << (bfsPath.size() - 1) << " buoc)\n";
            cout << "Nhan phim bat ky de quay lai menu...";
            getKeyPress();
            return;
        }

        char key = getKeyPress();
        int dir = -1;
        if (key == 'w' || key == 'W') dir = 0;
        else if (key == 's' || key == 'S') dir = 1;
        else if (key == 'd' || key == 'D') dir = 2;
        else if (key == 'a' || key == 'A') dir = 3;
        else if (key == 'q' || key == 'Q') return;

        if (dir != -1 && maze.canMove(player.r, player.c, dir)) {
            player.r += DR[dir];
            player.c += DC[dir];
            moves++;
        }
    }
}

// 6) So sanh BFS vs DFS
void compareBFSvsDFS(Maze& maze) {
    Point start{0, 0}, goal{maze.rows - 1, maze.cols - 1};

    int bfsVisited;
    auto bfsPath = maze.solveBFS(start, goal, &bfsVisited);

    int dfsBacktracks;
    auto dfsPath = maze.solveDFS(start, goal, false, &dfsBacktracks);

    cout << "\n=== SO SANH BFS vs DFS ===\n";
    cout << "BFS: do dai duong di = " << (bfsPath.size() - 1)
         << " buoc | so o da tham = " << bfsVisited << "\n";
    cout << "DFS: do dai duong di = " << (dfsPath.size() - 1)
         << " buoc | so lan backtrack = " << dfsBacktracks << "\n\n";

    if (bfsPath.size() == dfsPath.size()) {
        cout << "-> Hai duong co CUNG DO DAI.\n";
        cout << "   (Neu me cung chua duoc 'braid' them duong tat, day la dieu\n"
             << "    chac chan xay ra, vi giua 2 o chi co DUY NHAT 1 duong di -\n"
             << "    me cung la mot cay bao trum / spanning tree.)\n";
    } else {
        cout << "-> BFS tim duong NGAN HON DFS " << (dfsPath.size() - bfsPath.size())
             << " buoc.\n"
             << "   DFS di theo nhanh dau tien no gap (khong quan tam gan/xa)\n"
             << "   nen co the di vong truoc khi den dich, con BFS luon dam bao\n"
             << "   tim duong ngan nhat vi no duyet tuan tu theo tung 'lop'\n"
             << "   khoang cach tang dan.\n";
    }

    cout << "\nDuong BFS: \n";
    maze.display(bfsPath);
    cout << "\nDuong DFS: \n";
    maze.display(dfsPath);

    cout << "\nNhan Enter de tiep tuc...";
    cin.ignore(); cin.get();
}
