import socket
import threading
import subprocess

HEADER = 64
HOST = socket.gethostbyname(socket.gethostname())  # Endereço IP do servidor (localhost)
PORT = 5050  # Porta que o servidor irá escutar
ADDR = (HOST, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!SAI"

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDR)

def handle_client(conn, addr):
    print(f"[NOVA CONEXÃO] {addr} conectado.")
    
    connected = True
    while connected:
        msg_length = conn.recv(HEADER).decode(FORMAT)
        if msg_length:
            msg_length = int(msg_length)
            msg = conn.recv(msg_length).decode(FORMAT)
            if msg == DISCONNECT_MESSAGE:
                connected = False
                break

            # Enviar o comando para o programa em C
            try:
                processo = subprocess.Popen(["./programaTrab"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
                stdout, stderr = processo.communicate(input=msg + "\n")

                # Verifica se houve erro
                if stderr:
                    conn.send(f"Erro ao executar o comando: {stderr}".encode(FORMAT))
                else:
                    # Enviar a saída de volta para o cliente
                    conn.send(stdout.encode(FORMAT))
            except Exception as e:
                conn.send(f"Erro ao executar o comando: {str(e)}".encode(FORMAT))

    conn.close()
    print(f"[CONEXÃO FECHADA] {addr} desconectado.")

def start():
    server.listen()
    print(f"[ESCUTANDO] Servidor escutando em {HOST}:{PORT}")

    while True:
        conn, addr = server.accept()
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()
        print(f"[CONECTADOS ATIVOS] {threading.active_count() - 1}")

print("[INICIANDO] Iniciando servidor")
start()
