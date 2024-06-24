import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;

public class EditFrame extends JFrame{
	private Font mainFont = new Font("Segoe print", Font.BOLD, 18);

	public void iniciar(){
		/***************** Painel Botoes ****************/
		JButton btnAplicar = new JButton("Aplicar");
		btnAplicar.setFont(mainFont);
		btnAplicar.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				//aplicar alteracoes nos dados do jogador
			}
		});

		/***************** Painel Editar ****************/
		JPanel editPanel = new JPanel();

		editPanel.setBackground(new Color(255, 255, 255));
		editPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

		add(editPanel);

		/***************** Geral ****************/
		setTitle("Editar dados");
		setSize(600, 250);
		setMinimumSize(new Dimension(200, 100));
		setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
		setVisible(true);
	}
}
