# xetulai
Đề tài NCKH của sinh viên
1. Thiết bị:
2. Cách hoạt động:
Hoạt động chung: Xe tự chạy theo hướng đích và tự tránh chướng ngại vật
  - Khởi tạo: (setup)
  - Hoạt động (loop) : có 2 trạng thái
    + Khởi động: xác định hướng đích, ban đầu khi GPS chưa hoạt động thì hướng đích chính là hướng đặt xe ban đầu, sau đó chuyển trạng thái       sang vận hành
    + Vận hành: gồm 3 bước lần lượt: thu nhận và tính toán số liệu từ các cảm biến, tính các giá trị điều khiển, vận hành xe theo các giá         trị điều khiển
