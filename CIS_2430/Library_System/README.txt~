GENERAL PROBLEM

	Expand on the library program by including GUI and handling exceptions as well as implementing inheritence.

ASSUMPTIONS AND LIMITATIONS

	The user must input a call number, year and title for any journal or book that is to be added into the library.

	Keywords provided during the search must be seperated by a space in order to get accurate results

BUILDING AND TESTING PROGRAM

	To build the project first export the folder Assignment_3. Open the command prompt and navigate to the Assignment_3 		
	folder which you unpackaged.
	From the inside of the folder Assignment_3 type the following command in command prompt: "ant jar", this will build
	the file into a jar file. Navigate to the directory dist by typing: "cd dist" from the root folder.
	Now to run the file enter the command: java -jar Assignment3.jar.

	Make sure you install the ant library in order to be able to build the file. An alternative way to run the program 		
	is to open the project through netbeans and run it there.

IMPROVEMENTS
	
	Several improvements could have been made to the program in order to improve user experience and coding style.
	Firstly, the search function could have been made a lot more functional. The interface could have been formatted
	better and more message prompts could have been implemented.

TEST PLAN:

Tesing Add To Library

	1. Launch the program.
	2. Go to the add window.
	3. Add a book and follow through with the test cases below.
	4. Ad a journal and follow through with the test cases below.
	
	Testing Call Number Input
	1. Enter a blank call number.
	2. Enter a call number with only spaces.
	3. Enter a call number with a a number a letter and a space.

	Check that getting publisher works
	1.Repeat steps 1-3 from testing title input section

	Testing Title Input
	1. Input an empty title with no characters or spaces.
	2. Input only spaces for the title.
	3. Input a word "test" as the title.

	Testing Year Input
	1. Enter a negative number as the year.
	2. Enter a blank year.
	3. Enter a number that exceeds 9999.
	4. Enter a number that is less than 1000.
	5. Enter "test1" as input.
	6. Enter "1999" as input.

	Testing Unique Reference Keys
	1. Input the exact same call number and year for two a combination of two books, two journals, and one of each.



Testing Library GUI Search

	1. Launch the program.
	2. Go to add.
	3. Add a book and a journal by following the instructions with unique fields and take note of the fields you input.
	4. Go to search.
	 
	Testing Search Call Number
	1. Input the call number of one of the items you input.
	2. Leave the rest of the search criteria empty
	3. Make sure the item matched.
	4. Input a different call number than any of the items you input than repeat steps 2,3.
	5. Leave the call number field empty along with the rest of the fields and make sure the item displayed.

	Testing Title Search
	1.Repeat steps 1-5 in section Testing Search Call Number but instead input the title.
	2. Input two keywords as the search criteria which one of exists inside one of the items titles you added, make sure the item displays.
	3. Repeat step 2 with different capitalization of letters.
	4. Input two keywords as the search criteria, of which only one matches with an item in the reference list. Make sure the item matches.
	5. Input two keywords as the search criteria, of which none match with an item in the reference list. Make sure no items match.
	6. Input two keywords as the search criteria, each keyword being present in both items that have been added. Make sure both items match.

	Time Period Search
	1.Leave it blank and make sure all items display.
	2.Input a year that matches of the items.
	4.Input 9999 and 1000 as the start and end years respectively.
	5.Input 1000 and 9999 as the start and end years respectively.
	
	Combined Search
	1. Input a call number, keyword, and time period that matches the criteria of one of the items you added. Make sure the items match.
	2. Input a call number and keyword that matches the criteria of one of the items you added. Input a year period that does
	   not match that item. Make sure the item does not match. Repeat this step for every combination of call number, keyword, and year period.
	3. Input only two criteria out of the three criteria (call number, title, year period), that match an item you added to the list
	   make sure the item matches.

GUI Testing

	1. Switch between every component of the menu to make sure windows switch smoothly
	2. Input text that exceeds the characters that can fit inside a text field
	3. Use the quit function to test that the program quits.
	4. Use the reset function inside add and search to make sure all the fields can be reset to blank.














