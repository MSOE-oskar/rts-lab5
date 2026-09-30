package edu.msoe.swe4211.lightcontroller.pcgui;

/**
 * This file is not copyrighted.  Others are free to copy, modify, or redistribute this file as is seen fit.
 *
 **/
import java.awt.Color;
import java.awt.Container;
import javax.swing.JFrame;

import edu.msoe.swe4211.core.network.iNetworkController;


public class GUI {
	private Container contentPane;
	private JFrame jfMainWindow; // main window
	private LightControlPanel lightControlUI;
	private ConnectionGUIPanel connectionPanel;
	
	/**
	 * constructor for the GUI class creates the JFrame window and subcomponents
	 */
	public GUI(iNetworkController nwc) {
		// create a JFrame window and set its properties
		jfMainWindow = new JFrame("MSOE Disco Light Controller");
		jfMainWindow.setSize(325, 425); // window size
		jfMainWindow.setLocation(10, 10); // window location
		jfMainWindow.setResizable(false); // not resizeable

		/* !!!! */jfMainWindow.setVisible(true); // make visible!!!

		// EXIT_ON_CLOSE destroys all windows
		jfMainWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		// get the container that is part of the JFrame
		contentPane = jfMainWindow.getContentPane();

		// we're using absolute positioning
		contentPane.setLayout(null);

		// override default background color
		contentPane.setBackground(Color.GRAY);

		lightControlUI = new LightControlPanel(nwc);
		lightControlUI.setBounds(5,10,300,300);

		
		connectionPanel = new ConnectionGUIPanel(nwc);
		connectionPanel.setBounds(5, 325, 300, 50);

        
		contentPane.add(lightControlUI);
		contentPane.add(connectionPanel);
		
		// always call validate() after adding components
		// if the window is already visible
		contentPane.validate();
		// ...and this too; forces a refresh
		contentPane.repaint();
		contentPane.setFocusable(true);
		this.setDashColor(Color.CYAN);
	}

	/**
	 * This method will set the background color of all components on the display as is appropriate.
	 * @param c This is the color to be set.
	 */
	private void setDashColor(Color c) {
		this.contentPane.setBackground(c);
		this.jfMainWindow.setBackground(c);
	}

}
