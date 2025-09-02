# 没用啊，能把数据发到 vofa，但是 vofa 的数据发不到单片机

import socket
import threading
import subprocess
import sys

PORT = 2568

def tcp_to_pyocd(proc_stdin, conn):
    try:
        while True:
            data = conn.recv(1024)
            if not data:
                break
            proc_stdin.write(data)
            proc_stdin.flush()
    except Exception:
        pass

def pyocd_to_tcp(proc_stdout, conn):
    try:
        while True:
            data = proc_stdout.read(1024)
            if not data:
                break
            conn.sendall(data)
    except Exception:
        pass

def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', PORT))
    server.listen(1)
    print(f"Listening on TCP port {PORT}...")

    conn, addr = server.accept()
    print(f"Connection from {addr}")

    # 启动 pyocd rtt 子进程
    proc = subprocess.Popen(
        ['pyocd', 'rtt', '-t', 'PY32F002Bx5'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        bufsize=0
    )

    t1 = threading.Thread(target=tcp_to_pyocd, args=(proc.stdin, conn))
    t2 = threading.Thread(target=pyocd_to_tcp, args=(proc.stdout, conn))
    t1.start()
    t2.start()
    t1.join()
    t2.join()
    conn.close()
    server.close()
    proc.terminate()

if __name__ == '__main__':
    main()