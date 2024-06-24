import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.*;
import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.filechooser.FileNameExtensionFilter;

public class MainFrame extends JFrame {
    private Font mainFont = new Font("Segoe print", Font.BOLD, 18);
    private JList<String> playersList;
    private DefaultListModel<String> playersListModel;
    JTextField tfID, tfIdade, tfNomeJogador, tfNacionalidade, tfNomeClube;
    Client server;
    String selectedFileName;

    public void iniciar() {
        /***************** Barra de Menu ****************/
        JMenuBar menuBar = new JMenuBar();

        JMenu menuArquivo = new JMenu("Arquivo");
        menuBar.add(menuArquivo);

        JMenu menuServidor = new JMenu("Servidor");
        menuBar.add(menuServidor);

        JMenuItem mniCarregar = new JMenuItem("Carregar Arquivo");
        JMenuItem mniListar = new JMenuItem("Listar todos os jogadores");
        JMenuItem mniConectar = new JMenuItem("Conectar");
        JMenuItem mniDesconectar = new JMenuItem("Desconectar");

        menuArquivo.add(mniCarregar);
        menuArquivo.add(mniListar);
        menuServidor.add(mniConectar);
        menuServidor.add(mniDesconectar);

        mniConectar.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                ConnectDialog conectarServ = new ConnectDialog(MainFrame.this);
                conectarServ.setVisible(true); // Espera aqui até o popup ser fechado

                String host = conectarServ.getHost();
                int port = conectarServ.getPort();

				if(host.equals("localhost")){
					try {
						host = InetAddress.getLocalHost().getHostAddress();
					} catch (UnknownHostException e1) {
						e1.printStackTrace();
					}
				}

                server = new Client();
                server.setAddr(host, port);

                try {
                    server.connect();
					JOptionPane.showMessageDialog(MainFrame.this, "Conexão bem-sucedida com o servidor.");
                } catch (IOException f) {
                    f.printStackTrace();
					JOptionPane.showMessageDialog(MainFrame.this, "Erro ao conectar com o servidor.");
                }
            }
        });

        mniDesconectar.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    server.disconnect();
					JOptionPane.showMessageDialog(MainFrame.this, "Servidor desconectado.");
                } catch (IOException e1) {
                    e1.printStackTrace();
					JOptionPane.showMessageDialog(MainFrame.this, "Erro ao desconectar.");
                }
            }
        });

        mniCarregar.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();
        
                fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
                fileChooser.setMultiSelectionEnabled(false);
                FileNameExtensionFilter filter = new FileNameExtensionFilter("Arquivos BIN", "bin");
                fileChooser.setFileFilter(filter);
                int result = fileChooser.showOpenDialog(MainFrame.this);
        
                if (result == JFileChooser.APPROVE_OPTION) {
                    selectedFileName = fileChooser.getSelectedFile().getName();
                    JOptionPane.showMessageDialog(MainFrame.this, "Arquivo selecionado: " + selectedFileName);
                }
            }
        });

        mniListar.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    String response = server.sendMessage("2 " + selectedFileName);
                    processPlayersList(response); // Processa a resposta do servidor
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
            }
        });

        /***************** Painel Busca ****************/
        JLabel lbID = new JLabel("ID:");
        lbID.setFont(mainFont);

        tfID = new JTextField();
        tfID.setFont(mainFont);

        JLabel lbIdade = new JLabel("Idade:");
        lbIdade.setFont(mainFont);

        tfIdade = new JTextField();
        tfIdade.setFont(mainFont);

        JLabel lbNomeJogador = new JLabel("Nome do Jogador:");
        lbNomeJogador.setFont(mainFont);

        tfNomeJogador = new JTextField();
        tfNomeJogador.setFont(mainFont);

        JLabel lbNacionalidade = new JLabel("Nacionalidade:");
        lbNacionalidade.setFont(mainFont);

        tfNacionalidade = new JTextField();
        tfNacionalidade.setFont(mainFont);

        JLabel lbNomeClube = new JLabel("Nome do Clube:");
        lbNomeClube.setFont(mainFont);

        tfNomeClube = new JTextField();
        tfNomeClube.setFont(mainFont);

        JPanel buscaPanel = new JPanel();
        buscaPanel.setLayout(new GridLayout(5, 2, 0, 2));
        buscaPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        buscaPanel.setOpaque(false);
        buscaPanel.add(lbID);
        buscaPanel.add(tfID);
        buscaPanel.add(lbIdade);
        buscaPanel.add(tfIdade);
        buscaPanel.add(lbNomeJogador);
        buscaPanel.add(tfNomeJogador);
        buscaPanel.add(lbNacionalidade);
        buscaPanel.add(tfNacionalidade);
        buscaPanel.add(lbNomeClube);
        buscaPanel.add(tfNomeClube);

        /***************** Painel Botao ****************/
        JButton btnBuscar = new JButton("Buscar");
        btnBuscar.setFont(mainFont);
        btnBuscar.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                String ID = tfID.getText();
                String Idade = tfIdade.getText();
                String NomeJogador = tfNomeJogador.getText();
                String Nacionalidade = tfNacionalidade.getText();
                String NomeClube = tfNomeClube.getText();
                String res = "";
                int cont = 0;
                
                if(!ID.isEmpty()){
                    res += "id " + ID + " ";
                    cont++;
                }
                if(!Idade.isEmpty()){
                    res += "idade " + Idade + " ";
                    cont++;
                }
                if(!NomeJogador.isEmpty()){
                    res += "nomeJogador \"" + NomeJogador + "\" ";
                    cont++;
                }
                if(!Nacionalidade.isEmpty()){
                    res += "nacionalidade \"" + Nacionalidade + "\" ";
                    cont++;
                }
                if(!NomeClube.isEmpty()){
                    res += "nomeClube \"" + NomeClube + "\" ";
                    cont++;
                }
                    
                    try {
                        server.sendMessage("3 " + selectedFileName + " 1");
                        res = server.sendMessage(cont + " " + res);
                        processPlayersList(res);
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }

            }
        });

        JButton btnLimpar = new JButton("Limpar");
        btnLimpar.setFont(mainFont);
        btnLimpar.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                tfID.setText("");
                tfIdade.setText("");
                tfNomeJogador.setText("");
                tfNacionalidade.setText("");
                tfNomeClube.setText("");

                playersListModel.clear();
            }
        });

        JPanel botaoPanel = new JPanel();
        botaoPanel.setLayout(new GridLayout(1, 2, 5, 5));
        botaoPanel.setOpaque(false);
        botaoPanel.add(btnBuscar);
        botaoPanel.add(btnLimpar);

        /***************** Painel Principal ****************/
        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BorderLayout());
        mainPanel.setBackground(new Color(255, 255, 255));
        mainPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        mainPanel.add(buscaPanel, BorderLayout.NORTH);
        mainPanel.add(botaoPanel, BorderLayout.SOUTH);

        playersListModel = new DefaultListModel<>();
        playersList = new JList<>(playersListModel);
        playersList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        playersList.setCellRenderer(new MultiLineCellRenderer()); // Define um renderizador personalizado
        playersList.addListSelectionListener(new ListSelectionListener() {
            public void valueChanged(ListSelectionEvent e) {
                if (!e.getValueIsAdjusting()) {
                    int selectedIndex = playersList.getSelectedIndex();
                    if (selectedIndex != -1) {
                        String selectedPlayer = playersListModel.getElementAt(selectedIndex);
                        showPlayerDetails(selectedPlayer);
                    }
                }
            }
        });

        JScrollPane scrollPane = new JScrollPane(playersList);
        mainPanel.add(scrollPane, BorderLayout.CENTER);

        add(mainPanel);

        /***************** Geral ****************/
        setTitle("Busca de jogadores");
        setSize(550, 500);
        setMinimumSize(new Dimension(300, 400));
        setJMenuBar(menuBar);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setVisible(true);
    }
    
    private void processPlayersList(String response) {
        playersListModel.clear(); // Limpa a lista atual

        // Separa as linhas da resposta
        String[] lines = response.split("\n");
        StringBuilder sb = new StringBuilder();

        for (int i = 0; i < lines.length; i++) {
            sb.append(lines[i]).append("<br>");

            // Se chegou ao final de um conjunto de três linhas ou é a última linha
            if ((i + 1) % 4 == 0 || i == lines.length - 1) {
                // Adiciona o conjunto de três linhas como um único item formatado em HTML na lista
                playersListModel.addElement("<html>" + sb.toString().trim() + "</html>");
                sb.setLength(0); // Limpa o StringBuilder para o próximo conjunto de três linhas
            }
        }
    }

    private class MultiLineCellRenderer extends DefaultListCellRenderer {
        public Component getListCellRendererComponent(JList<?> list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
            JLabel label = (JLabel) super.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
            label.setText((String) value);
            return label;
        }
    }

    private void showPlayerDetails(String playerInfo) {
        JFrame detailsFrame = new JFrame("Detalhes do Jogador");
        JTextArea textArea = new JTextArea(playerInfo);
        textArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(textArea);
        detailsFrame.getContentPane().add(scrollPane, BorderLayout.CENTER);
        detailsFrame.setSize(300, 200);
        detailsFrame.setLocationRelativeTo(null);
        detailsFrame.setVisible(true);
    }

    public static void main(String[] args) {
        MainFrame myFrame = new MainFrame();
        myFrame.iniciar();
    }
}
