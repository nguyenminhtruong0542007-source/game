GIỚI THIỆU TỔNG QUAN DỰ ÁN GAME MÊ CUNG
1. Giới thiệu đề tài

Dự án Maze Game (Game giải mê cung) là một chương trình được xây dựng bằng ngôn ngữ lập trình C++ với mục đích mô phỏng một trò chơi tìm đường trong mê cung.

Trong trò chơi, hệ thống sẽ tự động tạo ra một mê cung ngẫu nhiên gồm nhiều ô vuông. Người chơi bắt đầu tại vị trí S (Start) và nhiệm vụ là tìm đường đến vị trí E (Exit) để thoát khỏi mê cung.

Bên cạnh chế độ chơi thủ công, chương trình còn tích hợp các thuật toán tìm kiếm trên đồ thị như BFS (Breadth First Search) và DFS (Depth First Search) để máy tính có thể tự giải mê cung, đồng thời so sánh sự khác nhau giữa hai thuật toán.

2. Mục tiêu của dự án

Dự án được xây dựng nhằm các mục tiêu:

Áp dụng kiến thức về cấu trúc dữ liệu và giải thuật vào một chương trình thực tế.
Xây dựng hệ thống tạo mê cung tự động bằng thuật toán.
Mô phỏng quá trình di chuyển và tìm đường trong game.
Hiểu cách hoạt động của các thuật toán duyệt đồ thị.
So sánh ưu điểm và nhược điểm của BFS và DFS.
3. Ý tưởng hoạt động của game

Mỗi mê cung được xem như một đồ thị (Graph):

Mỗi ô trong mê cung là một đỉnh (Vertex).
Các lối đi giữa các ô là cạnh (Edge).

Chương trình gồm các bước chính:

Người chơi chọn độ khó.
Hệ thống tạo một mê cung ngẫu nhiên.
Hiển thị mê cung trên màn hình dạng ký tự ASCII.
Người chơi có thể:
Tự điều khiển nhân vật.
Hoặc yêu cầu máy tìm đường.
Chương trình hiển thị kết quả và so sánh thuật toán.
