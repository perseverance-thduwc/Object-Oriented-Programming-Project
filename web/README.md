# Phần đồ họa web (tách riêng khỏi lõi C++)

    Trình duyệt (static/)  --HTTP POST /api/cmd-->  server.py  --stdin/stdout-->  arena_engine (C++)

- static/index.html, style.css : bố cục và giao diện
- static/art.js  : hình vẽ nhân vật / quái / Trùm / icon bằng SVG (không dùng ảnh ngoài)
- static/pixel.js: biến SVG thành pixel-art (canvas nhỏ + giảm màu + viền đen) và vẽ nền pixel từng màn
- static/game.js : gửi lệnh, nhận JSON, vẽ lại màn hình, số sát thương bay
- server.py      : biên dịch + chạy engine, chuyển lệnh (chỉ nhận lệnh dạng CHỮ_HOA [số] [số])
- engine/GameEngine.* : lớp C++ nhận lệnh, gọi các lớp trong ../core, trả JSON

Lệnh: NEW, STATE, HERO i, ATTACK e, SKILL s e, ITEM k, DEFEND, REWARD 1|2, BACK, PICK i.
