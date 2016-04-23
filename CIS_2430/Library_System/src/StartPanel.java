
import javax.swing.JLabel;
import javax.swing.JPanel;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author karol
 */
public class StartPanel extends JPanel{
    
    /**
     * Creates the Panel for the starting window
     * 
     */
    public StartPanel(){
           JLabel introText = new JLabel("<html>Welcome to Library Search.<br><br>"
                   + "<html>Choose a command from the \"Commands\" menu above for<br>"
                   + "<html>adding a reference, searching references, or quitting the<br>"
                   + "<html>program.</html>");

           this.add(introText);
    }
}
