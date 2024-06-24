import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

public class ConnectDialog extends JDialog {
    private Font mainFont = new Font("Segoe print", Font.BOLD, 18);
    JTextField tfEndereco, tfPorta;
    String host;
    int port;

    public ConnectDialog(JFrame parent) {
        super(parent, "Conectar ao Servidor", true); // Define o JDialog como modal
        setLayout(new BorderLayout());
        setSize(400, 250);
        setMinimumSize(new Dimension(200, 100));
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

        /***************** Painel Escrita ****************/
        JLabel lbEndereco = new JLabel("Endereço:");
        lbEndereco.setFont(mainFont);

        tfEndereco = new JTextField();
        tfEndereco.setFont(mainFont);

        JLabel lbPorta = new JLabel("Porta:");
        lbPorta.setFont(mainFont);

        tfPorta = new JTextField();
        tfPorta.setFont(mainFont);

        JPanel escritaPanel = new JPanel();
        escritaPanel.setLayout(new GridLayout(2, 2, 0, 2));
        escritaPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        escritaPanel.setOpaque(false);
        escritaPanel.add(lbEndereco);
        escritaPanel.add(tfEndereco);
        escritaPanel.add(lbPorta);
        escritaPanel.add(tfPorta);

        /***************** Painel Botoes ****************/
        JButton btnConectar = new JButton("Conectar");
        btnConectar.setFont(mainFont);
        btnConectar.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                host = tfEndereco.getText();
                port = Integer.parseInt(tfPorta.getText());
                dispose();
            }
        });

        JPanel connectPanel = new JPanel();
        connectPanel.setLayout(new BorderLayout());
        connectPanel.setBackground(new Color(255, 255, 255));
        connectPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        connectPanel.add(escritaPanel, BorderLayout.NORTH);
        connectPanel.add(btnConectar, BorderLayout.SOUTH);

        add(connectPanel);
    }

    public String getHost() {
        return host;
    }

    public int getPort() {
        return port;
    }
}
