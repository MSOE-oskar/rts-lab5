package edu.msoe.swe4211.lightcontroller.pcgui;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.net.UnknownHostException;

import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JTextField;

import edu.msoe.swe4211.core.network.iNetworkController;

/**
 * This class provides the layout for the connection GUII panel.  It shows how one can makle a connection with the remove PI.
 */
public class ConnectionGUIPanel extends JPanel {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	/**
	 * This constructor will instantiate a new instance of the connection gui display.
	 * 
	 * @param nwc This is the network controller.
	 */
	public ConnectionGUIPanel(final iNetworkController nwc) {
		this.setLayout(new BorderLayout());

		final JTextField textField = new JTextField("192.168.0.1");

		final JButton connectDisconnect = new JButton("Connect");

		connectDisconnect.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				if (nwc.getConnectionStatus() == false) {
					nwc.setIPAddress(textField.getText());
					try {
						nwc.connect();
						connectDisconnect.setText("Disconnect");
					} catch (UnknownHostException e) {
					} catch (IOException e) {
					}
				}
				else
				{
						nwc.disconnect();
						connectDisconnect.setText("Connect");
				}
			}
		});

		this.add(textField, BorderLayout.CENTER);
		this.add(connectDisconnect, BorderLayout.EAST);
	}
}
