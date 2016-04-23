
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Karol Zdebel
 * 
 * This class contains all the methods needed to access and 
 * change the journal object. 
 */
public class Journal extends Reference{
    private String organization = "";

    /**
     * Journal constructor.
     * 
     * @param callNumber Call number
     * @param title Title
     * @param year Year
     * @param organization Organization
     */
    public Journal(String callNumber,String title, int year,String organization){
        super(callNumber,title,year);
        this.organization = organization;
    }
    
    /**
     * Journal constructor with no arguments.
     */
    public Journal(){
        super("","",1000);
        this.organization = "";
    }
    /**
     * Set the organization of the journal
     * 
     * @param journalOrganization Organization of the journal
     */
        public void setOrganization(String journalOrganization){
        organization = journalOrganization;
    }
    
    /**
     * Get the journal organization
     * 
     * @return Journal organization
     */
        public String getOrganization(){
            return organization;
    }

    /**
     * Write contents of the Journal to a file.
     * 
     * @param fileName File name to be written to.
     */
    public void writeToFile(String fileName){
        PrintWriter fileWrite;
        try {
            fileWrite = new PrintWriter(new BufferedWriter(new FileWriter(fileName, true)));
            
            fileWrite.println();
            fileWrite.println("type = "+ "\"journal\"");
            fileWrite.println("callnumber = "+ "\""+this.getCallNumber()+"\"");
            fileWrite.println("title = "+ "\""+this.getTitle()+"\"");
            fileWrite.println("organization = "+ "\""+this.getOrganization()+"\"");
            fileWrite.println("year = "+ "\""+this.getYear()+"\"");
            
            fileWrite.close();
        } catch (IOException ex) {
            System.out.println("Error! Writing into file failed!");
        }
    }
    /**
     * Prints the contents of the instance variables in the journal, as a string.
     * 
     * @return String of instance variables in the journal
     */
    @Override
        public String toString(){
        return super.toString() + " organization: "+organization;
    }
    
    //Compare all elements of the journal parameter to the journal object itself to see if they are equal and return a boolean

    /**
     * Compare contents of the journal object, and the other object.
     * 
     * @param otherObject Object being compared to.
     * 
     * @return Boolean based on if the contents are equal.
     */
     @Override
    public boolean equals(Object otherObject){
        if (otherObject == null){
            return false;
        } 
        else if(getClass() != otherObject.getClass()){
            return false;
        }
        else{
            Journal otherJournal = (Journal)otherObject;
            return super.equals(otherObject) && organization.equals(otherJournal.getOrganization());
        }
    }
    
}
