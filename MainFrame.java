import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.filechooser.FileNameExtensionFilter;

/* Trabalho de POO feito pelos alunos:
 * Hélio Márcio Cabral Santos - N° USP: 14577862
 * Gabriel Martins Monteiro - N° USP: 14572099
 * Eduardo de Luna Freire - N° USP: 14567304*/

public class MainFrame extends JFrame {
    private Font mainFont = new Font("Segoe print", Font.BOLD, 18);
    private JList<String> playersList;
    private DefaultListModel<String> playersListModel;
    private String playerId, playerAge, playerName, playerNationality, playerClub;
    private JTextField playerIdField, playerAgeField, playerNameField, playerNationalityField, playerClubField;
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

        JMenuItem mniCarregarCSV = new JMenuItem("Carregar Arquivo CSV");
        JMenuItem mniCarregar = new JMenuItem("Carregar Arquivo");
        JMenuItem mniListar = new JMenuItem("Listar todos os jogadores");
        JMenuItem mniConectar = new JMenuItem("Conectar");
        JMenuItem mniDesconectar = new JMenuItem("Desconectar");

        menuArquivo.add(mniCarregarCSV);
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

        mniCarregarCSV.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();
        
                fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
                fileChooser.setMultiSelectionEnabled(false);
                FileNameExtensionFilter filter = new FileNameExtensionFilter("Arquivos CSV", "csv");
                fileChooser.setFileFilter(filter);
                int result = fileChooser.showOpenDialog(MainFrame.this);
        
                if (result == JFileChooser.APPROVE_OPTION) {
                    selectedFileName = fileChooser.getSelectedFile().getName();
                    JOptionPane.showMessageDialog(MainFrame.this, "Arquivo selecionado: " + selectedFileName);
                }

                try {
					String new_file = selectedFileName.replaceAll(".csv", ".bin");
                    server.sendMessage("1 " + selectedFileName + " " + new_file);
					selectedFileName = new_file;
                } catch (IOException e1) {
                    e1.printStackTrace();
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
                    openScrollableWindow(response); // Processa a resposta do servidor
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

        JButton btnLimpar = new JButton("Limpar");
        btnLimpar.setFont(mainFont);

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
                    res += "nomeJogador \"" + NomeJogador.toUpperCase() + "\" ";
                    cont++;
                }
                if(!Nacionalidade.isEmpty()){
                    res += "nacionalidade \"" + Nacionalidade.toUpperCase() + "\" ";
                    cont++;
                }
                if(!NomeClube.isEmpty()){
                    res += "nomeClube \"" + NomeClube.toUpperCase() + "\" ";
                    cont++;
                }
                    
				if(cont != 0){
                    try {
                        res = server.sendMessage("3 " + selectedFileName + " 1\n" + cont + " " + res);
                        res = res.substring(8);
                        processPlayersList(res);
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
				}

            }
        });

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
        setLocationRelativeTo(null);
        setVisible(true);
    }

    private void openScrollableWindow(String response) {
		JFrame newFrame = new JFrame("Todos os registros");
        newFrame.setSize(600, 400);
		newFrame.setLocationRelativeTo(null);

        JTextArea textArea = new JTextArea(response);
        textArea.setLineWrap(true);
        textArea.setWrapStyleWord(true);
        textArea.setEditable(false);
		textArea.setFont(mainFont);

        JScrollPane scrollPane = new JScrollPane(textArea);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);

        newFrame.add(scrollPane);
        newFrame.setVisible(true);
	}
    
    private void processPlayersList(String response) {
        playersListModel.clear(); // Limpa a lista atual

        // Separa as linhas da resposta
        String[] lines = response.split("\n");
        StringBuilder sb = new StringBuilder();

        for (int i = 0; i < lines.length; i++) {
            sb.append(lines[i]).append("<br>");

            // Se chegou ao final de um conjunto de cinco linhas ou é a última linha
            if ((i + 1) % 6 == 0 || i == lines.length - 1) {
                // Adiciona o conjunto de cinco linhas como um único item formatado em HTML na lista
                playersListModel.addElement("<html>" + sb.toString().trim() + "</html>");
                sb.setLength(0); // Limpa o StringBuilder para o próximo conjunto de cinco linhas
            }

			//if(i > 100) break;
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
        JFrame editFrame = new JFrame("Editar dados");
        editFrame.setLayout(new BorderLayout());

        /***************** Painel Botoes ****************/
        JButton btnAplicar = new JButton("Aplicar");
        btnAplicar.setFont(mainFont);

        JButton btnRemover = new JButton("Remover");
        btnRemover.setFont(mainFont);

        btnAplicar.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    server.sendMessage("5 " + selectedFileName + " ind.bin 1\n1 id " + playerId);
                    server.sendMessage("6 " + selectedFileName + " ind.bin 1\n" + playerIdField.getText() + " " + playerAgeField.getText() + " \"" + playerNameField.getText().toUpperCase() + "\" \"" + playerNationalityField.getText().toUpperCase() + "\" \"" + playerClubField.getText().toUpperCase() + "\"");
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
                editFrame.dispose();
            }
        });

        btnRemover.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    server.sendMessage("5 " + selectedFileName + " ind.bin 1\n1 id " + playerId);
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
                editFrame.dispose();
            }
        });

        JPanel botaoPanelEdit = new JPanel();
        botaoPanelEdit.setLayout(new GridLayout(1, 2, 5, 5));
        botaoPanelEdit.setOpaque(false);
        botaoPanelEdit.add(btnAplicar);
        botaoPanelEdit.add(btnRemover);

        /***************** Painel Dados ****************/
        // Use regex to extract player details
        Pattern pattern = Pattern.compile(
            "ID : (.*?)<br>Idade: (.*?)<br>Nome do Jogador: (.*?)<br>Nacionalidade do Jogador: (.*?)<br>Clube do Jogador: (.*?)<br>", 
            Pattern.DOTALL
        );
        Matcher matcher = pattern.matcher(playerInfo);

        if (matcher.find()) {
            playerId = matcher.group(1).trim();
            playerAge = matcher.group(2).trim();
            playerName = matcher.group(3).trim();
            playerNationality = matcher.group(4).trim();
            playerClub = matcher.group(5).trim();
        }

        // Panel for editable fields
        JPanel editablePanel = new JPanel(new GridLayout(5, 2));
        
        editablePanel.add(new JLabel("ID:"));
        playerIdField = new JTextField(playerId);
        editablePanel.add(playerIdField);
        
        editablePanel.add(new JLabel("Idade:"));
        playerAgeField = new JTextField(playerAge);
        editablePanel.add(playerAgeField);
        
        editablePanel.add(new JLabel("Nome do Jogador:"));
        playerNameField = new JTextField(playerName);
        editablePanel.add(playerNameField);
        
        editablePanel.add(new JLabel("Nacionalidade do Jogador:"));
        playerNationalityField = new JTextField(playerNationality);
        editablePanel.add(playerNationalityField);
        
        editablePanel.add(new JLabel("Clube do Jogador:"));
        playerClubField = new JTextField(playerClub);
        editablePanel.add(playerClubField);

        editFrame.add(editablePanel, BorderLayout.NORTH);
        editFrame.add(botaoPanelEdit, BorderLayout.SOUTH);

        editFrame.setSize(500, 300);
        editFrame.setLocationRelativeTo(null);
        editFrame.setVisible(true);
    }
    

    public static void main(String[] args) {
        MainFrame myFrame = new MainFrame();
        myFrame.iniciar();
    }
}
