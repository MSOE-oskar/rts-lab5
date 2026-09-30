import edu.msoe.swe4211.core.network.NetworkController;
import edu.msoe.swe4211.core.network.iNetworkController;
import edu.msoe.swe4211.lightcontroller.pcgui.GUI;

/**
 * This program will display a GUI that will then send single int messages to a device using a socket connection.
 * This is the main.  The bulk of the implementation is in the detail classes.
 * 
 * @author wws
 *
 */
public class GUIBasedMainProgram {

	/**
	 * the entry point for this program
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		iNetworkController nwc = new NetworkController();
		@SuppressWarnings("unused")
		GUI programGUI = new GUI(nwc);
	}
}
