# Arena RPG - Đồ án OOP (PHY10609 / PET10001)

Game nhập vai theo lượt (toàn bộ chữ trong game bằng tiếng Anh).

## Luật chơi
- Chọn **1 trong 3 hero** (Warrior / Mage / Archer) và chơi liên tục qua các màn **cho đến khi hero đó gục**.
  Khi gục, chọn 1 trong các hero còn lại để vào trận tiếp (quái giữ nguyên máu). **Cả 3 gục = thua.**
- 5 màn: màn n có n quái; màn 5 có 4 quái nhỏ + **AMOEBA BOSS** (chu kỳ 3 lượt: Toxic Storm → Pseudopod Slam → xoay đập).
- Quái đa dạng: Germ Warrior, Germ Mage, Germ Archer, Acid Spitter (gây độc), Shell Brute (trâu, có thể làm choáng).
- Mỗi lượt: **Attack / Skills / Items / Defend** (Defend: nhận một nửa sát thương, +6 MP).
- **Critical** (sát thương x2) và **Miss** (đánh trượt): ti lệ theo từng nhân vật (Archer crit 25%, Brute miss 25%...).
- Kỹ năng: 1 chiêu chung (Power Strike) + 1 chiêu riêng mỗi nhân vật + học thêm sau mỗi màn
  (Fire Burst, Venom Dart, Thunder Stun, Second Wind, Life Drain, Mighty Blow).
- Vật phẩm: Health/Mana/Great Potion, Elixir, Sharp Blade (vũ khí), Iron Shield (giáp).
- **Sau mỗi màn chọn thưởng ITEM hoặc SKILL**, rồi chọn 1 trong 3 phần thưởng.
- Điểm cuối tính bằng `CurvedActivity` (redefining `setScore`, +10%).

## Cấu trúc (3 tầng tách riêng để báo cáo)

| Thư mục | Ngôn ngữ | Vai trò |
|---|---|---|
| `core/` | **C++ OOP** (chỉ dùng kỹ thuật đã học) | TOÀN BỘ luật chơi: lớp `Game` (state machine) + Character, Warrior, Mage, Archer, Spitter, Brute, AmoebaBoss, Item, Weapon, Armor, Potion, StatusEffect, Skill, DungeonArena, Dice, CurvedActivity, GameException |
| `console/` | C++ | Bản chơi trên dòng lệnh (chỉ gọi lớp `Game`) |
| `web/engine/` | C++ | `GameEngine`: bọc lớp `Game`, nhận lệnh dạng chữ, trả trạng thái JSON |
| `web/server.py` | Python (thư viện chuẩn) | Chạy engine C++, chuyển lệnh qua HTTP |
| `web/static/` | HTML + CSS + JavaScript | Đồ họa **pixel-art** (sprite pixel hóa từ SVG), thanh HP/MP, số DMG/CRIT/MISS bay lên |
| `UML/` | Graphviz | Sơ đồ lớp UML và sơ đồ kiến trúc |

Luật chơi **không** nằm trong web: HTML/JS chỉ vẽ và gửi lệnh (`HERO 1`, `ATTACK 2`, `SKILL 1 0`, `ITEM 0`, `DEFEND`, `REWARD 2`, `PICK 0`...).
Xem sơ đồ: `UML/architecture.png`.

## Chạy bản đồ họa (web)
Cần: g++ và Python 3.
- Windows: bấm đôi `web/run_web.bat` — Linux/macOS: `web/run_web.sh`
- Trình duyệt tự mở http://localhost:8000. Lần đầu server tự biên dịch engine C++.

## Chạy bản console
- Linux/macOS: `./build_console.sh` — Windows: `build_console.bat`

## UML
`UML/arena_uml.png` (.svg, .pdf) vẽ theo code thật, `UML/architecture.png` là kiến trúc 3 tầng.
