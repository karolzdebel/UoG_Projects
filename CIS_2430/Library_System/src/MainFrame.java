
import java.awt.CardLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Creates the JFrame that is used throughout this program
 * 
 * @author karol
 */
public class MainFrame extends JFrame{
    
    private JPanel cards = new JPanel(new CardLayout());
    
    private InputPanel addPanel;
    private InputPanel searchPanel;
    private StartPanel startPanel;
    
    static final int START_SCREEN_WIDTH = 600;
    static final int START_SCREEN_HEIGHT = 400;
    static final int ADD_WINDOW_HEIGHT = 800;
    static final int ADD_WINDOW_WIDTH = 600;
    static final int PANEL_BOOK = 1;
    static final int PANEL_JOURNAL = 2;
    static final String PANE_ADD = "add";
    static final String PANE_START = "start";
    static final String PANE_SEARCH = "search";
    /**
     * @param args the command line arguments
     */

    /**
     * Creates the JFrame along with the initial screen and menu
     * 
     */

    public void createGUI(){
        JFrame frame = new JFrame("Library Search");
        JMenuBar menu;
        CardLayout cardLayout;
        String[] addLabelName = new String[]{"Adding a reference:","Type:","Call No:","Authors:"
            ,"Title:","Organization:","Publisher:","Year:"};
        String[] searchLabelName = new String[]{"Searching references:","Call No","Title Keywords:","Start Year:"
            ,"End Year:"};
        addPanel = new InputPanel(addLabelName,"Messages",false);
        searchPanel = new InputPanel(searchLabelName,"Search Results",true);
        startPanel = new StartPanel();
        
        //Add layouts to cards
        cards.add(startPanel,PANE_START);
        cards.add(addPanel,PANE_ADD);
        cards.add(searchPanel,PANE_SEARCH);
        
        //Create the components menu
        menu = createComponentMenu();
        frame.setJMenuBar(menu);
        
        //Create all the panes and add them to the cards layout
        
        //Add all the cards to the frame and show first window
        frame.add(cards);
        //Get the cards current layout to avoid casting
        cardLayout = (CardLayout)cards.getLayout();
        cardLayout.show(cards, PANE_START);
        
        //Set Up JFrame and show it
        //frame.setSize(cards.getSize());
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);
    }
    
    private JMenuBar createComponentMenu(){
        JMenuBar menuBar = new JMenuBar();
        JMenu menu = new JMenu("Commands");
        String[] menuItemNames = {"Add","Search","Quit"};
        JMenuItem menuItem = new JMenuItem();
        
        //Create menu with add,search,and quit as options
        for (int i=0;i<menuItemNames.length;i++){
            menuItem = new JMenuItem(menuItemNames[i]);
            
            menuItem.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e){
                    switch(((JMenuItem)e.getSource()).getText()){
                        case "Add":
                            menuButtonClicked("Add");
                            break;
                        case "Search":
                            menuButtonClicked("Search");
                            break;
                        case "Quit":
                            menuButtonClicked("Quit");
                            break;
                    }
                }
            });
            menu.add(menuItem);
        }
        
        menuBar.add(menu);
        
        return menuBar;
    }

    /**
     * Handles what happens if a certain menu button was clicked
     * 
     * @param name Name of button clicked
     */
    public void menuButtonClicked(String name){
        CardLayout layout = (CardLayout)cards.getLayout();
        switch(name){
            case "Add":
                //Switch to add panel
                layout.show(cards,PANE_ADD);
                break;
            //Switch to search panel
            case "Search":
                //Switch to search layout
                layout.show(cards,PANE_SEARCH);
                break;
            case "Quit":
                //Quit the program
                System.exit(1);
                break;
            default:
                System.out.println("menuButtonClicked is called with an"
                        + "invalid menu button name! Exiting");
                System.exit(0);
        }
    }
}
