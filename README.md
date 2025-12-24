# Đồ án cuối kỳ môn học SS004.Q12 - Kỹ năng nghề nghiệp: Thiết kế game Tetris

> **Đồ án môn học:** Kỹ năng nghề nghiệp - Thiết kế game  
> **Chủ đề:** Triển khai trò chơi xếp gạch cổ điển (Tetris Game Clone)

![C++](https://img.shields.io/badge/Language-C%2B%2B17-blue?style=for-the-badge&logo=c%2B%2B)
![Platform](https://img.shields.io/badge/Platform-Windows_Console-lightgrey?style=for-the-badge&logo=windows)
![IDE](https://img.shields.io/badge/IDE-VS_Code%20%2F%20Visual_Studio-purple?style=for-the-badge&logo=visual-studio-code)
![Status](https://img.shields.io/badge/Status-Completed-success?style=for-the-badge)

---

## 📖 Giới thiệu

**Tetris** là một trò chơi giải đố sắp xếp gạch (Tile-matching video game) kinh điển. Mục tiêu của trò chơi là di chuyển và xoay các khối gạch (Tetrominoes) đang rơi xuống để tạo thành các hàng ngang lấp đầy và xóa chúng.

**🎯 Mục tiêu của đồ án:**
* Áp dụng kiến thức về Mảng 2 chiều (2D Array) để quản lý lưới trò chơi (Grid).
* Cài đặt thuật toán biến đổi hình học (Geometric Transformation) để xử lý xoay khối.
* Quản lý bộ nhớ và tối ưu hóa vòng lặp game (Game Loop).

---

## 📐 Cơ sở Toán học & Giải thuật Cốt lõi

Trò chơi được xây dựng trên không gian lưới (Grid) 2 chiều rời rạc, sử dụng đại số tuyến tính cơ bản để xử lý biến đổi hình học và logic tập hợp để kiểm tra va chạm.

### 1. Hệ tọa độ và Phép xoay Ma trận (Discrete Rotation)

Màn hình Console sử dụng hệ tọa độ với gốc $O(0,0)$ tại góc trên bên trái, trục $X$ hướng sang phải và trục $Y$ hướng xuống dưới.

Các khối Tetromino được biểu diễn dưới dạng ma trận vuông cấp $N$ (thường là $4 \times 4$). Để xoay khối $90^\circ$ theo chiều kim đồng hồ, ta không dùng công thức lượng giác (sin/cos) để tránh sai số số thực, mà dùng phép hoán vị chỉ số ma trận:

Với mỗi điểm $P(i, j)$ trong ma trận cục bộ của khối, vị trí mới $P'(i', j')$ được xác định bởi:

$$
\begin{cases}
    i' = j \\
    j' = N - 1 - i
\end{cases}
$$

Trong đó:
* $i, j$: chỉ số dòng và cột ban đầu.
* $N$: kích thước cạnh của ma trận khối.

> **Cơ chế Wall Kick:** Sau khi áp dụng phép xoay, nếu vị trí mới của khối chồng lấn lên tường hoặc khối khác, thuật toán sẽ thử dịch chuyển khối ($x \pm 1, y \pm 1$) để tìm vị trí hợp lệ. Nếu không tìm được, phép xoay bị hủy bỏ.

### 2. Thuật toán Phát hiện Va chạm (Collision Detection)

Trạng thái game được quản lý bởi ma trận bàn cờ $B$ kích thước $Height \times Width$. Một khối gạch $T$ tại vị trí toàn cục $(x_0, y_0)$ được coi là **Hợp lệ** nếu và chỉ nếu mọi phần tử gạch $c(dx, dy) \in T$ thỏa mãn hệ điều kiện:

**Ràng buộc biên (Boundary Constraints):**
* $0 \le (x_0 + dx) < Width$
* $0 \le (y_0 + dy) < Height$

**Ràng buộc hình học (Geometric Constraint):**
Ô tương ứng trên bàn cờ phải là ô trống:
* $B[y_0 + dy][x_0 + dx] = \text{'SPACE\_CHAR'}$

Nếu bất kỳ điều kiện nào sai, hành động di chuyển/xoay sẽ bị ngăn chặn. Nếu đang rơi tự do, khối sẽ bị khóa (Lock) vào bàn cờ.

### 3. Công thức Tính điểm & Combo (Scoring System)
Hệ thống tính điểm khuyến khích người chơi duy trì chuỗi Combo và xóa nhiều hàng cùng lúc. Công thức tổng quát được cài đặt như sau:

$$S_{total} = S_{current} + (S_{base} \times L) + S_{combo}$$

Trong đó:
* $L$: Cấp độ hiện tại (Level).
* $S_{base}$: Điểm cơ bản dựa trên số hàng xóa được ($n$):
    * $n=1 \rightarrow 100$
    * $n=2 \rightarrow 300$
    * $n=3 \rightarrow 500$
    * $n=4 \rightarrow 800$ (Tetris)
* $S_{combo}$: Điểm thưởng khi xóa hàng liên tiếp:
  $$S_{combo} = 50 \times (C - 1) \times n$$
  (Với $C$ là bộ đếm chuỗi combo hiện tại).

---

## 🎮 Cơ chế Điều khiển & Xử lý Tín hiệu

Hệ thống điều khiển được thiết kế dựa trên cơ chế **thăm dò tín hiệu (Polling)** không đồng bộ, đảm bảo độ trễ đầu vào (Input Latency) ở mức thấp nhất để duy trì tính thời gian thực (Real-time) của trò chơi.

### 1. Kiến trúc Xử lý Sự kiện

Thay vì sử dụng ngắt (Interrupt) phần cứng, Game Loop sử dụng hàm `_kbhit()` (từ thư viện `conio.h` hoặc `Windows API`) để kiểm tra bộ đệm bàn phím trong mỗi khung hình.

* **Cơ chế Input Draining:** Để khắc phục hiện tượng "bóng ma" (Ghost inputs) do tốc độ gõ phím nhanh hơn tốc độ xử lý khung hình, vòng lặp xử lý tín hiệu được thiết kế để đọc sạch bộ đệm:
  $$Input_{queue} \rightarrow \text{Process} \rightarrow \text{Empty Queue}$$

* **Chuẩn hóa tín hiệu:** Mọi ký tự đầu vào đều được đưa về dạng chữ thường (lowercase) thông qua hàm `tolower()` để đảm bảo tính nhất quán.

### 2. Bảng ánh xạ phím (Key Mapping)

| Hành động (Action) | Player 1 (Local) | Player 2 (PVP/Remote) | Mô tả Kỹ thuật |
| :--- | :---: | :---: | :--- |
| **Di chuyển Trái/Phải** | `A` / `D` | `Left` / `Right` | Tịnh tiến tọa độ $x \leftarrow x \pm 1$. Kiểm tra va chạm biên $x \in [0, Width]$. |
| **Soft Drop** | `S` | `Down` | Tăng tốc độ rơi tạm thời ($v_{drop} \uparrow$). Cộng điểm nhỏ. |
| **Hard Drop** | `SPACE` | `ENTER` | Tính toán khoảng cách $d$ tới đáy. Dịch chuyển tức thời $y \leftarrow y + d$. Khóa khối ngay lập tức. |
| **Xoay khối** | `W` | `Up` | Thực hiện phép biến đổi ma trận $M \rightarrow M^T \rightarrow M_{row\_swap}$. |
| **Chức năng hệ thống** | `P` (Pause), `Q` (Quit) | - | Ngắt vòng lặp Game Loop hoặc kích hoạt Menu con. |

---

## 👥 Thành viên thực hiện 

<div align="center">

| STT |   MSSV   |       Họ và tên        | 
|:---:|:--------:| ---------------------  |
|  1  | 23521128 | Nguyễn Thị Quỳnh Như   |
|  2  | 24521314 | Nguyễn Văn Phát        |
|  3  | 24521381 | Lê Văn Hữu Phúc        |
|  4  | 24521344 | Thái Hoàng Huy Phong   |
|  5  | 24521175 | Dương Phạm Trần Nguyễn |

</div>

---

## 📚 Tài liệu tham khảo

1. Wikipedia, "Tetris - Game Logic and Rules", [Online]. Available: https://en.wikipedia.org/wiki/Tetris
2. Game Programming Patterns, "The Game Loop".
3. Super Rotation System (SRS) Documentation for standard Tetris rotation rules.

<br>

<p align="center">
  Copyright © 2025 - Đại học Công nghệ Thông tin, ĐHQG-HCM
</p>