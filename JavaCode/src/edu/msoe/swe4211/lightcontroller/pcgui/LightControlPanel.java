package edu.msoe.swe4211.lightcontroller.pcgui;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.Random;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import edu.msoe.swe4211.core.network.iNetworkController;

/**
 * This class defines the light control gui.
 *
 * @author schilling
 */
public class LightControlPanel extends JPanel {
    /**
     * These are the messages which will be sent to turn a light on or off.
     */
    static final int LIGHTOFFCMD = 0x00010000;
    static final int LIGHTONCMD = 0x00100000;
    static final int LIGHTDCUPDATECMD = 0x01000000;

    /**
     *
     */
    private static final long serialVersionUID = 1L;

    private JCheckBox ivChecksum = new JCheckBox("Send Invalid Checksum");
    private JCheckBox ivDestination = new JCheckBox(("Send Invalid Destinations"));
    private final iNetworkController NetworkController;
    private boolean monteCarloTestRunning = false;
    private JButton startMCTest = new JButton("Start MC Test");
    private JButton stopMCTest = new JButton("Stop MC Test");
    private Thread mcThread = null;


    private void sendMessage(int destination, int commandType, int message) throws IOException {
        int destinationOffset = 0;
        sendMessage(destination, commandType, message, 0);
    }


    private void sendMessage(int destination, int commandType, int message, int parameter) throws IOException {
        int destinationOffset = 0;
        if (ivDestination.isSelected()) {
            destinationOffset = ((int) (Math.random() * 100000 - 50000));
        }
        if (ivChecksum.isSelected()) {
            NetworkController.sendMessage(destination + destinationOffset, commandType, message, parameter, 0, 0xFEEDFEED);
        } else {
            NetworkController.sendMessage(destination + destinationOffset, commandType, message, parameter, 0);
        }
    }


    public LightControlPanel(final iNetworkController NetworkController) {
        this.NetworkController = NetworkController;

        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new GridLayout(0, 2, 10, 10));

        BorderLayout myLayout = new BorderLayout();
        this.setLayout(myLayout);

        JLabel lightOneDCSliderLabel = new JLabel("PWM 1 Duty Cycle", JLabel.CENTER);
        lightOneDCSliderLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JSlider lightOneDCSlider = new JSlider(JSlider.HORIZONTAL, 0, 100, 75);
        lightOneDCSlider.setMajorTickSpacing(20);
        lightOneDCSlider.setMinorTickSpacing(10);
        lightOneDCSlider.setPaintTicks(true);
        lightOneDCSlider.setPaintLabels(true);
        lightOneDCSlider.setName("PWM Duty Cycle");


        lightOneDCSlider.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                try {
                    sendMessage(1, iNetworkController.COMMAND_MSG_TYPE, LIGHTDCUPDATECMD, 10*lightOneDCSlider.getValue());
                } catch (IOException ioException) {
                    ioException.printStackTrace();
                }
            }
        });

        JLabel lightTwoDCSliderLabel = new JLabel("PWM 2 Duty Cycle", JLabel.CENTER);
        lightTwoDCSliderLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JSlider lightTwoDCSlider = new JSlider(JSlider.HORIZONTAL, 0, 100, 75);
        lightTwoDCSlider.setMajorTickSpacing(20);
        lightTwoDCSlider.setMinorTickSpacing(10);
        lightTwoDCSlider.setPaintTicks(true);
        lightTwoDCSlider.setPaintLabels(true);
        lightTwoDCSlider.setName("PWM Duty Cycle");

        lightTwoDCSlider.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                try {
                    sendMessage(2, iNetworkController.COMMAND_MSG_TYPE, LIGHTDCUPDATECMD, 10*lightTwoDCSlider.getValue());
                } catch (IOException ioException) {
                    ioException.printStackTrace();
                }
            }
        });

        JButton lightOneOn = new JButton("L1 ON");
        JButton lightOneOff = new JButton("L1 OFF");
        JButton lightTwoOn = new JButton("L2 ON");
        JButton lightTwoOff = new JButton("L2 OFF");

        mainPanel.add(lightOneDCSlider);
        mainPanel.add(lightTwoDCSlider);
        mainPanel.add(lightOneDCSliderLabel);
        mainPanel.add(lightTwoDCSliderLabel);

        mainPanel.add(lightOneOn);
        mainPanel.add(lightTwoOn);
        mainPanel.add(lightOneOff);
        mainPanel.add(lightTwoOff);

        mainPanel.add(ivChecksum);
        mainPanel.add(ivDestination);

        mainPanel.add(startMCTest);
        mainPanel.add(stopMCTest);
        stopMCTest.addActionListener(new ActionListener () {
            /**
             * Invoked when an action occurs.
             *
             * @param e the event to be processed
             */
            @Override
            public void actionPerformed(ActionEvent e) {
                monteCarloTestRunning = false;
                startMCTest.setEnabled(true);
                stopMCTest.setEnabled(false);
                mcThread=null;
            }
        });
        startMCTest.addActionListener(new ActionListener (){
            /**
             * Invoked when an action occurs.
             *
             * @param e the event to be processed
             */
            @Override
            public void actionPerformed(ActionEvent e) {
                Thread t = new Thread(new Runnable(){
                    /**
                     * When an object implementing interface {@code Runnable} is used
                     * to create a thread, starting the thread causes the object's
                     * {@code run} method to be called in that separately executing
                     * thread.
                     * <p>
                     * The general contract of the method {@code run} is that it may
                     * take any action whatsoever.
                     *
                     * @see Thread#run()
                     */
                    @Override
                    public void run() {
                        Random rnd = new Random();
                        while (monteCarloTestRunning)
                        {
                            try {
                                Thread.sleep(50);
                                sendMessage(rnd.nextInt(), rnd.nextInt(), rnd.nextInt());
                            } catch (InterruptedException | IOException ex ) {
                                ex.printStackTrace();
                            }
                        }
                    }
                });
                monteCarloTestRunning=true;
                t.start();
                mcThread = t;

                startMCTest.setEnabled(false);
                stopMCTest.setEnabled(true);

            }
        });

        if (!monteCarloTestRunning)
        {
            startMCTest.setEnabled(true);
            stopMCTest.setEnabled(false);
        }
        else
        {
            startMCTest.setEnabled(false);
            stopMCTest.setEnabled(true);
        }
        add(mainPanel, BorderLayout.CENTER);

        lightOneOff.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    sendMessage(1, iNetworkController.COMMAND_MSG_TYPE, LIGHTOFFCMD);

                } catch (IOException e1) {
                }
            }
        });

        lightOneOn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    sendMessage(1, iNetworkController.COMMAND_MSG_TYPE, LIGHTONCMD);
                    sendMessage(1, iNetworkController.COMMAND_MSG_TYPE, LIGHTDCUPDATECMD, 10*lightOneDCSlider.getValue());
                } catch (IOException e1) {
                }
            }
        });

        lightTwoOff.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    sendMessage(2, iNetworkController.COMMAND_MSG_TYPE, LIGHTOFFCMD);
                } catch (IOException e1) {
                }
            }
        });

        lightTwoOn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    sendMessage(2, iNetworkController.COMMAND_MSG_TYPE, LIGHTONCMD);
                    sendMessage(2, iNetworkController.COMMAND_MSG_TYPE, LIGHTDCUPDATECMD, 10*lightOneDCSlider.getValue());
                } catch (IOException e1) {
                }
            }
        });
    }
}
