# -*- coding: utf-8 -*-
"""
server.py - CAU NOI giua trinh duyet va loi C++.

  Trinh duyet (HTML/JS)  --HTTP-->  server.py  --stdin/stdout-->  arena_engine (C++)

Server KHONG chua luat choi. No chi:
  1. bien dich engine C++ (neu chua co) bang g++,
  2. chay engine thanh tien trinh con,
  3. chuyen lenh tu web vao engine va tra JSON ve web,
  4. phuc vu file giao dien trong thu muc static/.

Chay:  python server.py      roi mo http://localhost:8000
"""
import glob
import http.server
import json
import os
import re
import subprocess
import sys
import threading
import webbrowser

PORT = 8000
HERE = os.path.dirname(os.path.abspath(__file__))
CORE = os.path.join(HERE, "..", "core")
ENGINE_SRC = os.path.join(HERE, "engine")
STATIC = os.path.join(HERE, "static")
EXE = os.path.join(ENGINE_SRC, "arena_engine.exe" if os.name == "nt" else "arena_engine")

# Chi cho phep lenh dang  CHU_HOA [so] [so]
CMD_PATTERN = re.compile(r"^[A-Z]{3,6}( -?\d{1,2}){0,2}$")


def build_engine():
    # Dung duong dan TUONG DOI + chay trong thu muc web/ de lenh g++ khong chua
    # ky tu co dau (g++/ld tren Windows loi neu duong dan thu muc co tieng Viet).
    def rel(path):
        return os.path.relpath(path, HERE).replace("\\", "/")

    sources = [rel(os.path.join(ENGINE_SRC, "engine_main.cpp")),
               rel(os.path.join(ENGINE_SRC, "GameEngine.cpp"))]
    sources += sorted(rel(p) for p in glob.glob(os.path.join(CORE, "*.cpp")))
    cmd = ["g++", "-O2", "-I", rel(CORE), "-I", rel(ENGINE_SRC), "-o", rel(EXE)] + sources
    print("Bien dich engine C++ ...")
    result = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
    if result.returncode != 0:
        print(result.stderr)
        sys.exit("Khong bien dich duoc engine. Hay cai g++ (MinGW/MSYS2) roi thu lai.")
    print("OK ->", EXE)


class Engine:
    """Boc tien trinh C++: moi lenh gui di -> doc dung 1 dong JSON tra ve."""

    def __init__(self):
        self.lock = threading.Lock()
        self.proc = None
        self._start()

    def _start(self):
        self.proc = subprocess.Popen([EXE], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                     text=True, bufsize=1, encoding="utf-8")

    def send(self, command):
        with self.lock:
            if self.proc.poll() is not None:
                self._start()
            self.proc.stdin.write(command + "\n")
            self.proc.stdin.flush()
            return self.proc.stdout.readline().strip()

    def close(self):
        if self.proc and self.proc.poll() is None:
            self.proc.stdin.write("QUIT\n")
            self.proc.stdin.flush()


engine = None


class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=STATIC, **kwargs)

    def do_POST(self):
        if self.path != "/api/cmd":
            self.send_error(404)
            return
        length = int(self.headers.get("Content-Length", 0))
        try:
            command = json.loads(self.rfile.read(length))["cmd"]
        except Exception:
            self.send_error(400, "JSON khong hop le")
            return
        if not CMD_PATTERN.match(command):
            self.send_error(400, "Lenh khong hop le")
            return
        body = engine.send(command).encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, fmt, *args):   # bot log cho gon
        pass


def main():
    global engine
    if not os.path.exists(EXE):
        build_engine()
    engine = Engine()
    server = http.server.ThreadingHTTPServer(("127.0.0.1", PORT), Handler)
    url = "http://localhost:%d" % PORT
    print("Arena RPG dang chay tai", url, "(Ctrl+C de dung)")
    threading.Timer(0.8, lambda: webbrowser.open(url)).start()
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        engine.close()


if __name__ == "__main__":
    main()
