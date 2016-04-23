
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.StringTokenizer;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * This class manages the book and journal lists and also contains 
 * the necessary methods for the search function
 * 
 * @author Karol Zdebel
 * 
 */
public class LibrarySearch {
    
    private ArrayList<Reference> referenceList = new ArrayList();
    private HashMap<String,ArrayList> keyWordMap = new HashMap();
    /**
     * Add a the book object to the list of books present in the library object
     * @param book Book object
     */
        public void addBook(Book book){
        referenceList.add(book);
    }
    
    /**
     * Add a the journal object to the list of books present in the library object
     * 
     * @param journal Journal object
     */
        public void addJournal(Journal journal){
        referenceList.add(journal);
    } 
    
    //Checks if the reference keys of a book object match any other keys present in the library, returns a boolean

    /**
     * Checks if the reference keys of a book object match any other keys present in the library
     * 
     * @param book Book object to be checked
     * @return Whether or not another object exists with the same reference key
     */
        public boolean checkSameReferenceKey(Book book){
        int i;
        
        for (i=0;i<referenceList.size();i++){
            if (referenceList.get(i).getYear() == book.getYear() && referenceList.get(i).getCallNumber().equalsIgnoreCase(book.getCallNumber())){
                return true;
            }
        }        
        return false;
    }

    /**
     * Checks if the reference keys of a journal object match any other keys present in the library, returns a boolean
     * 
     * @param journal Journal object
     * @return Whether or not another object exists with the same reference key
     */
        public boolean checkSameReferenceKey(Journal journal){
        int i;
            System.out.println("Reference list size"+referenceList.size());
        for (i=0;i<referenceList.size();i++){
            System.out.println("reference year: "+referenceList.get(i).getYear()
            +"\n Journal year: "+journal.getYear());
            if (referenceList.get(i).getYear() == journal.getYear() && referenceList.get(i).getCallNumber().equalsIgnoreCase(journal.getCallNumber())){
                return true;
            }
        }
        return false;
    }
    
    /**
     * Compares two call number string values
     * 
     * @param callNumber1 Call Number
     * @param callNumber2 Call Number
     * @return Whether the call numbers are the same
     */
        public boolean matchCallNumber(String callNumber1, String callNumber2){
        return callNumber1.toLowerCase().equals(callNumber2.toLowerCase());
    }

    /**
     * Compares a title and the keywords present
     * 
     * @param title1 Title
     * @param keyWord Keyword
     * @return Boolean based on if a keyword is found
     */
        public boolean matchTitle(String title1,String keyWord){
        StringTokenizer titleTokenizer = new StringTokenizer(title1);
        StringTokenizer keywordTokenizer = new StringTokenizer(keyWord);
        String titleToken,keywordToken;
        outerloop:
        while (titleTokenizer.hasMoreTokens()){
            titleToken = titleTokenizer.nextToken().toLowerCase();
            while(keywordTokenizer.hasMoreTokens()){
                keywordToken = keywordTokenizer.nextToken().toLowerCase();
                //If a certain keyword matches a word present in the particular title than return true
                if (titleToken.equals(keywordToken)){
                    return true;
                }
            }
            if (titleTokenizer.hasMoreTokens()){
                titleTokenizer.nextToken();
            }
        }
        //If no matches are found return false
        return false;
    }
     
        
        
    /**
     * Checks to see if the integer value itemYear is within the string value timePeriod
     * 
     * @param timePeriod Year range
     * @param itemYear Year
     * @return Boolean based on whether the item year is within the year range
     */
        public boolean matchYears(String timePeriod,int itemYear){
        String[] token = timePeriod.split("-");
        int minYear = 0;
        int maxYear = 0;
        //If two tokens are found than look through a range of years
        if (token.length == 2){
            if (!token[0].equals("") && !token[1].equals("")){
                minYear = Integer.parseInt(token[0]);
                maxYear = Integer.parseInt(token[1]);
                if (itemYear <= maxYear && itemYear >= minYear){
                    return true;
                }
            }
            else if (token[0].equals("")){
                minYear = 1000;
                maxYear = Integer.parseInt(token[1]);
                if (itemYear <= maxYear && itemYear >= minYear){
                    return true;
                }
            }
        }
        //If one token is found than only look for one year
        else if (token.length == 1){
            if (timePeriod.matches("(.*)-")){
                maxYear = 9999;
                minYear = Integer.parseInt(token[0]);
                if (itemYear <= maxYear && itemYear >= minYear){
                    return true;
                }
            }
            else{       
                minYear = Integer.parseInt(token[0]);
                if (itemYear == minYear){
                    return true;
                }
            }
        }
        //If no tokens are present or more than two tokens are present return false since the input is incorrect
        else{
            return false;
        }
        return false;
    }

    /**
     * Search through the book and journal list and match objects based on the parameters
     * Add all objects which matched to the matched list and print them to the user
     * 
     * @param callNumber Call Number
     * @param timePeriod Time Period
     */
        public void Search(String callNumber,String timePeriod){
        ArrayList<Reference> itemsMatched = new ArrayList<>();
        int matchCount;
        int i;
        //Search through all journals
        for (i=0;i<referenceList.size();i++){
            matchCount = 0;
            
            //Match call number
            
            if (!callNumber.equals("")){
                if (matchCallNumber(referenceList.get(i).getCallNumber(),callNumber)){
                    matchCount++;
                    System.out.println("Matched call number");
                }
            }
            else{
                matchCount++;
            }
            //Match years
            if (!timePeriod.equals("")){
                if (matchYears(timePeriod,referenceList.get(i).getYear())){
                    matchCount++;
                }
            }
            else{
                matchCount++;
            }
            //If the two criteria matched than add the item to the matched list
            if (matchCount == 2){
                itemsMatched.add(referenceList.get(i));
            }
            
        }
        //Display all items present in the matched list
        System.out.println("Found the following items: ");
        
        for (i=0;i<itemsMatched.size();i++){
            System.out.println(itemsMatched.get(i).toString());
        }
    }

    /**
     * Search for items using keywords given in a hash map
     * 
     * @param callNumber Call number being searched for.
     * @param titleKeywords Keywords being searched for.
     * @param timePeriod Time period being searched for.
     */
    public ArrayList<Reference> searchHash(String callNumber,String titleKeywords,String timePeriod){
        String keywords[];
        int matchCount = 0;
        int matchedPos = 0;
        boolean foundKey = false;
        ArrayList<Reference> matchedList = new ArrayList();
        keywords = titleKeywords.split(" ");
        for (int i=0;i<keywords.length;i++){
            //If the keyword is found within the hash map perform 
            //hash search otherwise perform linear search
            if (keyWordMap.containsKey(keywords[i])){
                foundKey = true;
                //For however many indexes the keyword has in the hash map,
                //compare each index with the parameters
                for (int j=0;j<keyWordMap.get(keywords[i]).size();j++){
                    matchCount = 0;
                    matchedPos = ((int)keyWordMap.get(keywords[i]).get(j));
                    if (timePeriod.equals("")){
                        matchCount++;
                    }
                    else if (matchYears(timePeriod,referenceList.get(matchedPos).getYear())){
                        matchCount++;
                    }
                    if (callNumber.equals("")){
                        matchCount++;
                    }
                    else if (matchCallNumber(callNumber,referenceList.get(matchedPos).getCallNumber())){
                        matchCount++;
                    }
                    System.out.println("Found the following items:");
                    if (matchCount == 2){
                        matchedList.add(referenceList.get(matchedPos));
                    }
                }
            }
        }


        return matchedList;
    }

    /**
     * Parses the file and stores the data in the reference list.
     * 
     * @param fileName Name of file to be read from.
     */
    public void readFile(String fileName){
        BufferedReader reader;
        String string;
        String type="";
        boolean journalAdded = false;
        boolean bookAdded = false;
        String token = "";
        String value = "";
        String splitString[] = new String[1];
        Reference item;
        Book bookToAdd = new Book();
        Journal journalToAdd = new Journal();
       //Try to open the file, if exception is thrown go to catch
        try{
            reader = new BufferedReader(new FileReader(fileName));
            while ((string = reader.readLine()) != null){
                splitString = string.split("=");
                //If the type is a book create book object, if its a journal create journal object
                if (splitString[0].matches("(.*)type(.*)")){
                    if (splitString[1].matches("(.*)book(.*)")){
                        bookToAdd = new Book();
                        bookAdded = true;
                        journalAdded = false;
                    }
                    else if (splitString[1].matches("(.*)journal(.*)")){
                        journalToAdd = new Journal();
                        bookAdded = false;
                        journalAdded = true;
                    }
                }
                //Match the first token with call number, title, organization etc. and set its value accordingly
                //to what is between quotations
                else{
                    splitString = string.split("=");
                    if(splitString.length > 1){
                        type = splitString[0];
                        token = splitString[1];
                        splitString = token.split("\"",-1);
                        value = ""; 
                        //Parse value between quotations
                        for (int i=1;i<splitString.length;i++){
                            if (splitString.length > 2 && i < splitString.length-2){
                                value+=splitString[i]+"\"";
                            }
                            else{
                                value+=splitString[i];
                            }
                        }
                        //Store data inside the book object or journal object
                        if (bookAdded){
                            storeData(bookToAdd,type,value);
                        }
                        else if(journalAdded){
                            storeData(journalToAdd,type,value);
                        }
                        //Since year is the last parameter, if it is parsed, add the object to reference list
                        if (type.matches("(.*)year(.*)")){                   
                            if (bookAdded){
                                referenceList.add(bookToAdd);
                            }
                            else if(journalAdded){
                                referenceList.add(journalToAdd);
                            }
                        }
                    }

                }
                
            }
            mapKeyWords();
        }
        catch(IOException c){
            System.out.println("Unable to parse file! Error!");
        }
    }

    
    /**
     * Write the current reference list to a file.
     * 
     * @param fileName File name to be written to.
     */
    public void writeListToFile(String fileName){
        Book book = new Book();
        Journal journal = new Journal();
        //Write every elemenent inside the reference list to a file using
        //the write method in either the journal or book object depending
        //on the class of the object
        for (int i=0;i<referenceList.size();i++){
            if (referenceList.get(i).getClass() ==  book.getClass()){
                Book writeBook = new Book();
                writeBook = (Book)referenceList.get(i);
                writeBook.writeToFile(fileName);
            }
            else if (referenceList.get(i).getClass() == journal.getClass()){
                Journal writeJournal = new Journal();
                writeJournal = (Journal)referenceList.get(i);
                writeJournal.writeToFile(fileName);
            }
        }
    }
    
    //Add all the keywords as keys in the hashmap, and an arraylist with the
    //indexes of those keywords as the value in the hashmap
    private void mapKeyWords(){
        String keyWords[];
        ArrayList<Integer> keyWordIndex = new ArrayList();
        for (int i=0;i<referenceList.size();i++) {
            keyWords = referenceList.get(i).getTitle().split(" ");
            for (String keyWord : keyWords) {
                if(keyWordMap.containsKey(keyWord)){
                    keyWordMap.get(keyWord).add(i);
                }
                else{
                    keyWordIndex = new ArrayList();
                    keyWordIndex.add(i);
                    keyWordMap.put(keyWord, keyWordIndex);
                }
            }
        }
    }
    
    //Store the parameters inside the book object
    private void storeData(Book book,String component,String data){
        switch(component){
            case "callnumber ":
                book.setCallNumber(data);
                break;
            case "authors ":
                book.setAuthors(data);
                break;
            case "title ":
                book.setTitle(data);
                break;
            case "publisher ":
                book.setPublisher(data);
                break;
            case "year ":
                try{
                    book.setYear(Integer.parseInt(data));
                }
                catch(NumberFormatException c){
                    
                }
                break;
        }
    }
    
    //Store the parameters inside the journal object
    private void storeData(Journal journal,String component,String data){
        switch(component){
            case "callnumber ":
                journal.setCallNumber(data);
                break;
            case "title ":
                journal.setTitle(data);
                break;
            case "year ":
                try{
                journal.setYear(Integer.parseInt(data));
                }
                catch(NumberFormatException c){
                }
                break;
            case "organization ":
                journal.setOrganization(data);
        }
    }
    
}
