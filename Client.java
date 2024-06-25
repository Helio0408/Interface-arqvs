import java.io.*;
import java.net.*;

public class Client {
    private static final int HEADER = 64;
    private static final String FORMAT = "UTF-8";
    private static final String DISCONNECT_MESSAGE = "!SAI";
    private String host;
    private int port;
    private Socket client;

    // Construtor padrão
    public Client() {
        try {
            this.host = InetAddress.getLocalHost().getHostAddress(); // Endereço IP do servidor (localhost)
            this.port = 5050; // Porta padrão
        } catch (UnknownHostException e) {
            throw new RuntimeException(e);
        }
    }

    // Construtor com parâmetros
    public Client(String host, int port) {
        this.host = host;
        this.port = port;
    }

    // Método para setar o endereço e a porta
    public void setAddr(String host, int port) {
        this.host = host;
        this.port = port;
    }

    // Método para conectar ao servidor
    public void connect() throws IOException {
        client = new Socket(host, port);
    }

    // Método para enviar uma mensagem ao servidor e obter a resposta
    public String sendMessage(String message) throws IOException {
        if (client == null) {
            throw new IllegalStateException("Client is not connected. Call connect() first.");
        }
        String response = send(message);
        if (message.equals(DISCONNECT_MESSAGE)) {
            disconnect();
        }
        return response;
    }

    // Método para desconectar do servidor
    public void disconnect() throws IOException {
        send(DISCONNECT_MESSAGE);
        client.close();
    }

    // Método privado para enviar mensagens ao servidor e obter a resposta
    private String send(String msg) throws IOException {
        OutputStream output = client.getOutputStream();
        PrintWriter writer = new PrintWriter(new OutputStreamWriter(output, FORMAT), true);
        InputStream input = client.getInputStream();
        BufferedReader reader = new BufferedReader(new InputStreamReader(input, FORMAT));

        // Preparar a mensagem para envio com o cabeçalho
        byte[] message = msg.getBytes(FORMAT);
        int msgLength = message.length;
        String sendLength = String.valueOf(msgLength);
        while (sendLength.length() < HEADER) {
            sendLength += " ";
        }
        writer.print(sendLength);
        writer.flush();
        output.write(message);
        output.flush();

        // Ler a resposta do servidor
        char[] buffer = new char[204800];
        int charsRead = reader.read(buffer, 0, 204800);
        if (charsRead != -1) {
            return new String(buffer, 0, charsRead);
        }
        return null;
    }

}
