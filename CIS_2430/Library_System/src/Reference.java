/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author karol
 */
public abstract class Reference {
    private String callNumber;
    private String title;
    private int year;
    
    /**
     * Reference constructor.
     * 
     * @param callNumber Call number
     * @param title Title
     * @param year Year
     */
    public Reference(String callNumber,String title, int year){
        this.callNumber = callNumber;
        this.title = title;
        this.year = year;
    }
    
    /**
     * Reference constructor with no arguments.
     * 
     */
    public Reference(){
        this.callNumber = "";
        this.title = "";
        this.year = 1000;
    }
    
    /**
     * Sets the call number of the reference.
     * 
     * @param callNumber Call number
     */
    public void setCallNumber(String callNumber){
        this.callNumber = callNumber;
    }

    /**
     * Sets the year of the reference.
     * 
     * @param year Year
     */
    public void setYear(int year){
        this.year = year;
    }
    
    /**
     * Sets the title of the reference.
     * 
     * @param title Title
     */
    public void setTitle(String title){
        this.title = title;
    }
    
    /**
     * Gets the call number of the reference.
     * 
     * @return Call number of the reference
     */
    public String getCallNumber(){
        return this.callNumber;
    }
    
    /**
     * Gets the year of the reference.
     * 
     * @return Year of the reference
     */
    public int getYear(){
        return this.year;
    }
    
    /**
     * Gets the year of the reference.
     * 
     * @return Title of the reference
     */
    public String getTitle(){
        return this.title;
    }
    
    /**
     * Prints the contents of the instance variables in the reference, as a string.
     * 
     * @return String of instance variables in the reference
     */
    @Override
    public String toString(){
        return "call number: "+callNumber + " title: "+title+" year: "+year;
    }
    
    /**
     * Compare contents of the reference object, and the other object.
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
            Reference otherReference = (Reference)otherObject;
            return callNumber.equals(otherReference.getCallNumber()) && title.equals(otherReference.getTitle()) && year == otherReference.getYear();
        }
    }
}
