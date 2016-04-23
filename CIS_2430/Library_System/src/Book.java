
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
 * change the book object. 
 */
public class Book extends Reference{
    private String authors = "";
    private String publisher = "";
    
    /**
     * Book constructor.
     * 
     * @param callNumber Call number
     * @param title Title
     * @param year Year
     * @param authors Authors
     * @param publisher Publisher
     */
    public Book(String callNumber,String title, int year,String authors,String publisher){
        super(callNumber,title,year);
        this.authors = authors;
        this.publisher = publisher;
    }
    
    /**
     * Book constructor with no arguments
     *  
     */
    public Book(){
        super("","",1000);
        this.authors = "";
        this.publisher = "";
    }
    
    /**
     * Sets the authors of the book.
     * 
     * @param bookAuthors Authors of the book
     */
        public void setAuthors(String bookAuthors){
        authors = bookAuthors;
    }

    /**
     * Sets the publisher of the book.
     * 
     * @param bookPublisher Publisher of the book
     */
        public void setPublisher(String bookPublisher){
        publisher = bookPublisher;
    }

    /**
     * Gets the authors of the book and return a string.
     * 
     * @return Authors
     */
        public String getAuthors(){
            return authors;
    }

    /**
     * Gets the publisher of the book.
     * 
     * @return Publisher
     */
        public String getPublisher(){
            return publisher;
    }
        
    /**
     *  Write contents of the Book to a file.
     * 
     * @param fileName File name to be written to.
     */
    public void writeToFile(String fileName){
        PrintWriter fileWrite;
        try {
            fileWrite = new PrintWriter(new BufferedWriter(new FileWriter(fileName, true)));
            
            //write code to print info to the file in the format:
            //<ItemName> by <itemCreator> for <Price>
            fileWrite.println();
            fileWrite.println("type = "+ "\"book\"");
            fileWrite.println("callnumber = "+ "\""+this.getCallNumber()+"\"");
            fileWrite.println("authors = "+ "\""+this.getAuthors()+"\"");
            fileWrite.println("title = "+ "\""+this.getTitle()+"\"");
            fileWrite.println("publisher = "+ "\""+this.getPublisher()+"\"");
            fileWrite.println("year = "+ "\""+this.getYear()+"\"");
            
            fileWrite.close();
        } catch (IOException ex) {
            System.out.println("Error! Writing into file failed!");
        }
    }

    /**
     * Prints the contents of the instance variables in the book object, as a string.
     * 
     * @return String value of the instance variables of the book
     */
    @Override
        public String toString(){
        return super.toString() + " authors: " + this.getAuthors() + " publisher: "+ publisher;
    }

    /**
     * Compare contents of the book object, and the other object.
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
            Book otherBook = (Book)otherObject;
            return super.equals(otherObject) && authors.equals(otherBook.getAuthors()) && publisher.equals(otherBook.getPublisher());
        }
    }
}
