
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import javax.swing.*;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Creates the input JPanel
 * 
 * @author karol
 */
public class InputPanel extends JPanel{
    
    private final int MODE_BOOK = 0;
    private final int MODE_JOURNAL = 1;
    
    private JPanel inputPane;
    //buttonPane holds the add and reset button 
    private JPanel buttonPane = new JPanel();
    //messagesPane holds the messages text area
    private JPanel messagesPane = new JPanel();
    
    private JTextField extraField;
    private JTextField[] fields;
    private JLabel[] labels;
    private JTextArea textAreaMessage;

    private LibrarySearch library; 
    private Book bookToAdd;
    private Journal journalToAdd;
    
    private int currentMode;
    private int searchMode;
    private int panelMode;
    
    /**
     * Constructor for creating the input JPanel
     * 
     * @param labelName Label Name
     * @param type Type of JPanel to create
     * @param search Search or Not Search
     */
    public InputPanel(String labelName[],String type,boolean search){
        
        labels = new JLabel[labelName.length];
        fields = new JTextField[labelName.length-2];
       
        for (int i=0;i<labels.length;i++){
            labels[i] = new JLabel(labelName[i]);
        }
        for (int i=0;i<fields.length;i++){
            fields[i] = new JTextField();
        }
        
        inputPane = createColumnPanel(labels,fields,search); 
        if (type.equals("Messages")){
            buttonPane = createButtonPanelAdd();
        }
        else if (type.equals("Search Results")){
            buttonPane = createButtonPanelSearch();
        }
        messagesPane = createMessagePanel(type);
        /*Add the three panes which make up the add pane 
          to the add pane*/
        
        //Leave spacing between panels
        this.setLayout(new BorderLayout(50,50));
        //Leave spacing around the edges of the window
        this.setBorder(BorderFactory.createEmptyBorder(20,20,20,20));
        this.add(inputPane,BorderLayout.WEST);
        this.add(buttonPane,BorderLayout.EAST);
        this.add(messagesPane,BorderLayout.SOUTH); 
        this.switchToBookForm();
    }

    /**
     * Creates the column panel that appears in all the windows except the start window
     * 
     * @param labels
     * @param fields
     * @param search
     * @return
     */
    protected JPanel createColumnPanel(JLabel labels[],JTextField fields[],boolean search){
        
        JPanel panel = new JPanel();
        JComboBox comboBoxType = new JComboBox();
        int offSet;
        
        /*Set size of the text field based on 
         the label next to it*/
        offSet = 2;
              
        for (int i=offSet;i<labels.length;i++){
            switch(labels[i].getText()){
                //Size the field for the type
                case "Type:":
                    fields[i-offSet].setPreferredSize(new Dimension(135,20));
                    break;
                //Size the field for the call number
                case "Call No:":
                    fields[i-offSet].setPreferredSize(new Dimension(135,20));
                    break;
                //Size the field for the authors
                case "Authors:":
                    fields[i-offSet].setPreferredSize(new Dimension(220,20));
                    break;
                //Size the field for the title
                case "Title:":
                    fields[i-offSet].setPreferredSize(new Dimension(220,20));
                    break;
                //Size the field for the Publisher
                case "Publisher:":
                    fields[i-offSet].setPreferredSize(new Dimension(210,20));
                    break;
                //Size the field for the organization
                case "Organization:":
                    fields[i-offSet].setPreferredSize(new Dimension(190,20));
                    break;
                //Size the field for the year
                case "Year:":
                    fields[i-offSet].setPreferredSize(new Dimension(75,20));
                        break;
                case "Start Year:":
                    fields[i-offSet].setPreferredSize(new Dimension(75,20));
                        break;
                case "End Year:":
                    fields[i-offSet].setPreferredSize(new Dimension(75,20));
                        break;
                case "Title Keywords:":
                    fields[i-offSet].setPreferredSize(new Dimension(210,20));
                    break;
            }
        }
        
        //Set the combo box size
        comboBoxType.setPreferredSize(new Dimension(100,20));
        //Arrange labels and fields in grid bag layout
        panel.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        
        gbc.anchor = GridBagConstraints.WEST;
        gbc.fill = GridBagConstraints.NONE;
        gbc.insets = new Insets(20,0,0,0);  
        
        //Add the reference with no text field next to it first
        gbc.gridx = 0;
        gbc.gridy = 0;
        panel.add(labels[0],gbc);
        
        gbc.gridx = 0;
        gbc.gridy = 1;
        //Add all the labels
        for (int i=1;i<labels.length;i++){
            gbc.gridy = i;
            panel.add(labels[i],gbc); 
        
        }
        String[] choices = new String[]{"Book","Journal"};
        //Add the combo box
        gbc.gridx = 1;
        gbc.gridy = 1;       
        
        if (!search){
            comboBoxType = new JComboBox(choices);
            comboBoxType.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e){
                    switch(((JComboBox)e.getSource()).getSelectedItem().toString()){
                        case "Book":
                            //Switch to book layout
                            System.out.println("Book");
                            addTypeClicked("Book");
                            break;
                        case "Journal":
                            //Switch to journal layout
                            System.out.println("Journal");
                            addTypeClicked("Journal");
                            break;
                    }
                }
            });
                    
             panel.add(comboBoxType,gbc);
        }
        else{            
            extraField = new JTextField("");
            extraField.setPreferredSize(new Dimension(80,20));
            panel.add(extraField,gbc);
        }
        
        /*Add the extra text field that is only show in search*/
        gbc.gridx = 1;
        gbc.gridy = 1;
             
        gbc.gridx = 1;
        gbc.gridy = 0;  
        
        //Add all the text fields
        for (int i=0;i<fields.length;i++){
            gbc.gridy = i+2;
            panel.add(fields[i],gbc);         
        }
        
        return panel;
    }
      
    /**
     * Creates the panel that shows prompt messages
     * 
     * @param type Type of message panel to create
     * @return JPanel with the message panel
     */
    protected JPanel createMessagePanel(String type){
        JPanel panel = new JPanel();
        textAreaMessage = new JTextArea();
        
        //Add scroll pane to text area
        textAreaMessage.setEditable(false);
        JScrollPane scroll = new JScrollPane(textAreaMessage,
            JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        JLabel labelMessage = new JLabel(type);
        
        
        //Make the text area larger
        textAreaMessage.setPreferredSize(new Dimension(20,100));
        
        panel.setLayout(new BorderLayout());
        panel.add(labelMessage,BorderLayout.LINE_START);
        panel.add(scroll,BorderLayout.SOUTH);
          
        return panel;
    }
    
    /**
     * Creates Buttons that are used to add items
     * @return JPanel with the buttons
     */
    protected JPanel createButtonPanelAdd(){
        JPanel panel = new JPanel();
        JButton buttonAdd;
        JButton buttonReset;
        int inputYear;
        library = new LibrarySearch();
        
        buttonAdd = new JButton ("Add");
        buttonReset = new JButton("Reset");
        
        /*When add button is clicked*/
        buttonAdd.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e){
                boolean inputSuccess = false;
                /*Construct an item of type (Combo Box)
                  and get all other criteria from text fields*/
                try{
                    if (currentMode == MODE_BOOK){
                        /*Check criteria for valid input*/
                        if (checkYearInput(fields[5].getText()) &&
                            checkCallNumInput(fields[0].getText()) &&
                            checkTitleInput(fields[2].getText())){
                            
                            inputSuccess = true;
                        }
                        
                        /*Construct book with all text field criteria*/
                        bookToAdd = new Book(fields[0].getText(),
                            fields[3].getText(),
                            Integer.parseInt(fields[5].getText()),
                            fields[1].getText(),fields[4].getText());
                        
                        /*Add item if input is correct*/
                        if (inputSuccess){
                            library.addBook(bookToAdd);
                            resetTextAreaMessage();
                            setTextAreaMessage("Book title " +fields[2].getText() 
                                + " sucessfully added to library!\n");
                        }
                    }
                    else if (currentMode == MODE_JOURNAL){
                        for (int i=0;i<fields.length;i++){
                            System.out.println("Field "+i+" is: "
                            + fields[i].getText());
                        }
                        /*Check criteria for valid input*/
                        if (checkYearInput(fields[5].getText()) &&
                            checkCallNumInput(fields[0].getText()) &&
                            checkTitleInput(fields[2].getText())){
                            inputSuccess = true;
                        }                      
                        
                        journalToAdd = new Journal(fields[0].getText(),
                            fields[2].getText(),
                            Integer.parseInt(fields[5].getText()),
                            fields[3].getText());
                        
                        if (inputSuccess){
                            library.addJournal(journalToAdd);
                            resetTextAreaMessage();
                            setTextAreaMessage("Journal " + fields[2].getText() 
                               + " sucessfully added to library!\n");
                        }
                    }
                    
                }catch(NumberFormatException s){
                        setTextAreaMessage(textAreaMessage.getText() + 
                            "Please input a year between 1000-9999\n");
                }
            }
        });
        
        /*When reset button is clicked*/
        buttonReset.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e){
                //Set all fields to blank
                for (int i=0;i<fields.length;i++){
                    fields[i].setText("");
                }
                setTextAreaMessage("Add criteria reset!");
            }
        });
        //Add reset and add buttons to north and south of panel
        panel.setLayout(new BorderLayout());
        //Move the buttons away from the vertical edges of the screen
        panel.setBorder(BorderFactory.createEmptyBorder(50,0,50,10));
        
        panel.add(buttonAdd,BorderLayout.NORTH);
        panel.add(buttonReset,BorderLayout.SOUTH);
        
        //Create listeners here
        
        return panel;
    }

    /**
     * Creates the buttons for the search panel
     * 
     * @return JPanel with the buttons
     */
    protected JPanel createButtonPanelSearch(){
        JPanel panel = new JPanel();
        JButton buttonSearch;
        JButton buttonReset;
        ArrayList<Reference> printList = new ArrayList();
        
        buttonSearch = new JButton("Search");
        buttonReset = new JButton("Reset");
        
        /*When search button is clicked*/
        buttonSearch.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e){
                boolean inputSuccess = true;
                String timePeriod = "";
                int startYear = 0;
                int endYear = 0;
                /*Search for an item of type (Combo Box)
                 and get all other criteria from text fields*/
                if (fields[1].getText().equals("")){
                    startYear = 1000;
                }
                if (fields[2].getText().equals("")){
                    endYear = 9999;
                    if (startYear == 1000){
                        timePeriod = "";
                    }
                }
                
                for (int i=0;i<fields.length;i++){
                    System.out.println("Field "+i + "= "+fields[i].getText());
                }
                /*Check criteria for valid input*/
                if (startYear > endYear){
                    inputSuccess = false;
                    setTextAreaMessage(textAreaMessage.getText() +
                            "Start Year cannot be greater than End Year!");
                }
                if (inputSuccess){
                     if (startYear == 1000 && endYear == 9999){
                         
                     }
                     else{
                         timePeriod = "" + startYear + "-" + endYear;
                     }
                     timePeriod = "9000";
                     System.out.println("Extra field: " + extraField.getText());
                     System.out.println("Time period" + timePeriod);
                     /*Search for book with given criteria and 
                       print the results in the message box*/
                    //library.searchHash(extraField.getText(),
                    //fields[0].getText(),timePeriod);
                }
            }
        });
        
        /*When reset button is clicked*/
        buttonReset.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e){
                //Set all fields to blank
                for (int i=0;i<fields.length;i++){
                    fields[i].setText("");
                }
                setTextAreaMessage("Search criteria reset!");
            }
        });
        
        //Add reset and add buttons to north and south of panel
        panel.setLayout(new BorderLayout());
        //Move the buttons away from the vertical edges of the screen
        panel.setBorder(BorderFactory.createEmptyBorder(50,0,50,10));
        
        panel.add(buttonSearch,BorderLayout.NORTH);
        panel.add(buttonReset,BorderLayout.SOUTH);
        
        //Create listeners here
        
        return panel;
    }
    private void addTypeClicked(String name){
        switch(name){
            case "Book":
                switchToBookForm();
                break;
            case "Journal":
                switchToJournalForm();
                break;
            default:
                System.out.println("menuButtonClicked is called with an"
                        + "invalid menu button name! Exiting");
                System.exit(0);
        }
    }

    /**
     * Switches from Journal layout to book layout
     * 
     */
    public void switchToBookForm(){
        
        /*Reset message box*/
        resetTextAreaMessage();
        
        /*Reset old field content*/
        for (int i=0;i<fields.length;i++){
            fields[i].setText("");
        }
        /*Remove all journal specific labels and fields
          Show all book specific labels and fields*/
        for (int i=0;i<labels.length;i++){
            switch (labels[i].getText()){
                //Hide organization
                case "Organization:":
                    labels[i].setEnabled(false);
                    labels[i].setVisible(false);
                    fields[i-2].setEnabled(false);
                    fields[i-2].setVisible(false);
                    break;
                //Show publisher
                case "Publisher:":
                    labels[i].setEnabled(true);
                    labels[i].setVisible(true);
                    fields[i-2].setEnabled(true);
                    fields[i-2].setVisible(true);
                    break;
                //Show authors
                case "Authors:":
                    labels[i].setEnabled(true);
                    labels[i].setVisible(true);
                    fields[i-2].setEnabled(true);
                    fields[i-2].setVisible(true);
                    break;
            }
        }
        currentMode = MODE_BOOK;
    }

    /**
     *S witches from Book layout to Journal layout
     */
    public void switchToJournalForm(){
        
        /*Reset message box*/
        resetTextAreaMessage();
        
        /*Reset old field content*/
        for (int i=0;i<fields.length;i++){
            fields[i].setText("");
        }
        /*Remove all book specific labels and fields
          Show all journal specific labels and fields*/
        for (int i=0;i<labels.length;i++){
            switch (labels[i].getText()){
                //Hide organization
                case "Organization:":
                    labels[i].setEnabled(true);
                    labels[i].setVisible(true);
                    fields[i-2].setEnabled(true);
                    fields[i-2].setVisible(true);
                    break;
                //Show publisher
                case "Publisher:":
                    labels[i].setEnabled(false);
                    labels[i].setVisible(false);
                    fields[i-2].setEnabled(false);
                    fields[i-2].setVisible(false);
                    break;
                //Show authors
                case "Authors:":
                    labels[i].setEnabled(false);
                    labels[i].setVisible(false);
                    fields[i-2].setEnabled(false);
                    fields[i-2].setVisible(false);
                    break;
            }
        }
        
        currentMode = MODE_JOURNAL;
    }
    
    private void setTextAreaMessage(String string){
        textAreaMessage.setText(string);
    }
    private void resetTextAreaMessage(){
        textAreaMessage.setText("");
    }
    private boolean checkYearInput(String year){
        if (Integer.parseInt(year) > 9999 
            || Integer.parseInt(year) < 1000){
            setTextAreaMessage(textAreaMessage.getText() + 
                "Please input a year between 1000-9999 before adding!\n");
            return false;
        }
        return true;
    }
    private boolean checkCallNumInput(String callNum){
        if (callNum.equals("")){
            setTextAreaMessage(textAreaMessage.getText() + 
                "A call number must be input!\n");
            return false;
        }
        return true;
    }
    private boolean checkTitleInput(String title){
        if (title.equals("")){
            setTextAreaMessage(textAreaMessage.getText() + 
                "A title must be input!\n");
            return false;
        }
        return true;
    }
    private void printArrayList(ArrayList<Reference> list){
        resetTextAreaMessage();
        setTextAreaMessage("Matched the following items:\n");
        for (int i=0;i<list.size();i++){
            setTextAreaMessage(list.get(i).toString()+"\n");
        }
    }
}
