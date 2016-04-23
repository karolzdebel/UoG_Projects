#!/usr/bin/python3

###################################################################
# xcal.py -- Front-end GUI for calcomp.
# Last updated:  1:01 PM Match-18-16
# 
# Name: Karol Zdebel
# Student Number: 0892519
# Contact: kzdebel@mail.uoguelph.ca
###################################################################

import subprocess
import Cal
import tkinter
import os
import sys
import re
import mysql.connector
import string
import datetime
from mysql.connector import Error
from tkinter import *
from tkinter import ttk
from fileinput import filename
from tkinter import filedialog
		
class CheckList(Toplevel):
	
	def todoListDone(self):
		self.destroy()
	
	def destroySelf(self,event):
		self.destroy()
	
	def __init__(self,master):
		super(CheckList,self).__init__()
		
		self.master = master
		
		#bind ESC to exit
		self.bind("<Escape>",self.destroySelf)
		
		#Add done and cancel buttons
		butFrame = Frame(self)
		done = Button(butFrame,text="Done",command=self.todoListDone)
		done.pack(side=LEFT)
		cancel = Button(butFrame,text="Cancel",command=self.destroy)
		cancel.pack(side=LEFT)
		
		#Create top labels
		labelFrame = Frame(self)
		
		labNum = Label(labelFrame,text="No.",width=6,font="-size 9 -weight bold")
		labProps = Label(labelFrame,text="Props",width=6,font="-size 9 -weight bold")
		labSubs = Label(labelFrame,text="Subs",width=6,font="-size 9 -weight bold")
		labSummary = Label(labelFrame,text="Summary",width=23,font="-size 9 -weight bold")
		
		labNum.pack(side=LEFT)
		labProps.pack(side=LEFT)
		labSubs.pack(side=LEFT)
		labSummary.pack(side=LEFT)
		
		#Get all todo's
		self.todos = []

		for i in self.master.main.compList:
			
			if (i[0] == "VTODO"):
				self.todos.append(i)

		#Create checkbox for ever todo
		checkFrame = Frame(self)
		
		counter = 0
		check = []
		for tup in self.todos:
			totalText = ""
			rowFrame = Frame(checkFrame)
			
			colCounter=1
			var = counter+1
			
			#Create label for every value in tuple
			for val in tup:
				
				#Don't print VTODO its redundant
				if (val != "VTODO"):
					rowLabel = Label(rowFrame,text = str(val))
					rowLabel.grid(row=0,column=colCounter)
					
					#Set appropriate width
					if (colCounter == 3):
						rowLabel.config(width='25')
					else:
						rowLabel.config(width='6')
					
					colCounter+=1
				 
			check.append(Checkbutton(checkFrame,text=str(counter+1)
							,command=self.selectCheck,variable=var))
			
			check[counter].grid(row=counter,column=0,sticky=W)
			rowFrame.grid(row=counter,column=1,sticky=W)
			counter+=1
			
		labelFrame.grid(row=0,sticky=W)
		checkFrame.grid(row=1,sticky=W)
		butFrame.grid(row=2)
		
	def selectCheck(self):
		print("selected!")
		
class MenuBar(Menu):
	
	saved = 0
	fileName = ""
	main = ""
	eventSelect = 0
	todoSelect = 0
	discard = 0
	queryActive = False
		
	def setFileName(self,name):
		self.fileName = name
		
	def getFileName(self,name):
		return self.filename
	
	def sdiscard(self):

		self.discard = 1
		self.modpop.destroy()
		self.fileOpen()
		
	def cancel(self):
		self.discard = 0
		self.modpop.destroy()
	
	def fileOpen(self):
		
		if (self.main.saved == 1 or self.discard == 1):
			self.discard = 0
			name = ""
			name = filedialog.askopenfilename(filetypes=(("Calendar files", "*.ics")
														,("All files", "*")))
				
			result = []
			
			#If name is found read file
			if (name != ""):
			
				self.main.compWindow.clearTree()
				
				self.main.setFileName(name)
				self.main.setTitle(name)
				
				#Add limited shortcuts only once file open
				self.main.bind("<Control-s>", self.fileSaveShort)
				self.main.bind("<Control-t>", self.todoListShort)
		
				status = Cal.readFile(name,result)
				
				#Print error found
				if (status != 'OK'):
					self.main.logWindow.printMessage(status)
				
				#Run caltool -info and display in log
				else:
					
					#Enable the menu
					self.enableMenu()
					self.enableDataStore()
					
					#Store pointer and tuple list
					self.main.pcal = result[0]
					self.main.compList = result[1]
					
					command = "./caltool -info < "+name
					out = subprocess.Popen \
							(command, shell=True
							, stdout=subprocess.PIPE
							, stderr=subprocess.PIPE)
							
					for line in out.stdout.readlines():
						self.main.logWindow.printMessage(line.decode("utf-8"))
					
					self.main.logWindow.printMessage("\n")
					
					#Fill treeview
					self.main.compWindow.fillTree(self.main.compList)
					
					#Enable extract buttons
					self.main.compWindow.showExtractButtons()
				
		elif (self.main.saved == 0):
			
			#If unsaved create modal popup
			self.modpop = Toplevel()
			self.modpop.bind("<Escape>", self.destroyModalPrompt)
			self.modpop.wm_title("Warning")
			
			msg = Message(self.modpop,text="Warning unsaved changes exist. Discard the changes?")
			
			discardBut = Button(self.modpop,text="Discard",command=self.sdiscard)
			cancelBut = Button(self.modpop,text="Cancel",command=self.cancel)
			
			msg.pack(side=TOP)
			discardBut.pack(side=LEFT)
			cancelBut.pack(side=RIGHT)
			
			self.modpop.grab_set()
			self.modpop.transient(self.main)
			self.modpop.wait_window(self.modpop)
			
			self.modpop.mainloop()
		
	
	def destroyModalPrompt(self,event):
		self.modpop.destroy()
	
	def fileOpenShort(self,event):
		self.fileOpen()

	def lineCount(self,name):
	    lines = 0
	    
	    for line in open(self.main.getFileName(),"r"):
	    	lines += 1
	    return lines

	def fileSave(self):
		
		if (self.main.getTitle() == ""):
			self.main.logWindow.printMessage("Error, no file open for saving.\n")

		else:		
			status = Cal.writeFile(self.main.getFileName() \
						,self.main.pcal,self.main.calIndex)
			
			#Print error to log
			if(status != 'OK'):
				self.main.logWindow.printMessage(status)
			
			else:
				lineNum = self.lineCount(self.main.getTitle())
				message = "Wrote "+ str(lineNum) + " lines to " +self.main.getFileName()+" successfully.\n"
				self.main.logWindow.printMessage(message)
				
				#Set to saved
				self.main.setSaved()
				
	def fileSaveShort(self,event):
		self.fileSave()
		
	def fileSaveAs(self):
		
		if (self.main.getTitle() == ""):
			self.main.logWindow.printMessage("Error, no file open for saving.\n")
			
		else:
			name = filedialog.askopenfilename(filetypes=(("Calendar files", "*.ics")
														,("All files", "**")))
			
			#Make sure name was received
			if (name != ""):
				
				status = Cal.writeFile(name \
						,self.main.pcal,self.main.calIndex)
				
				#Print error to log
				if(status != 'OK'):
					self.main.logWindow.printMessage(status)
	
				else:
					#Change title to new file plus astrix still technically unsaved
					if (self.main.saved == 0):
						self.main.setTitle(name+"*")
					else:
						self.main.setTitle(name)
					
					lineNum = self.lineCount(self.main.getTitle())
					message = "Wrote "+ str(lineNum) + " lines to " +self.main.getTitle()+" successfully.\n"
					self.main.logWindow.printMessage(message)
					
					#Set to saved
					self.main.setSaved()
				
	def fileCombine(self):
		name = filedialog.askopenfilename(filetypes=(("Calendar files", "*.ics")
													,("All files", "**")))
		
		#Make sure name was received
		if (name != ""):
			Cal.writeFile("logfile.tmp",self.main.pcal,self.main.calIndex)
			command = "./caltool -combine logfile.tmp < "+name
			out = subprocess.Popen(command, shell=True
								, stdout=subprocess.PIPE
								, stderr=subprocess.PIPE)
			
			total = ""
			for line in out.stdout.readlines():
				total += (line.decode("utf-8"))
					
			with open("logfile.tmp", 'w') as file:
				file.write(total)
					
			result = []
			
			status = Cal.readFile("logfile.tmp",result)
			
			#Print error found
			if (status != 'OK'):
				self.main.logWindow.printMessage(status)
 			
			#Run caltool -info and display in log
			else:
				#Clear FVP
				self.main.compWindow.clearTree()
				
				#Store pointer and tuple list
				self.main.pcal = result[0]
				self.main.compList = result[1]
				
				#Fill treeview
				self.main.compWindow.fillTree(self.main.compList)
				
				#Set unsaved
				self.main.setUnsaved()
				

			#Remove file
			try:
				os.remove("logfile.tmp")
			except OSError:
		   		pass
		
	def filterFromTo(self):
		fromDate = self.fromInput.get()
		toDate = self.toInput.get()
		value = ""
		
		if (toDate == ""):
			self.main.logWindow.printMessage("Error: missing to date.\n")
			self.filPop.destroy()
		
		if (fromDate == ""):
			self.main.logWindow.printMessage("Error: missing from date\n")
			self.filPop.destroy()
		
		if (self.eventSelect == 1):
			value = 'e'
			
		if (self.todoSelect == 1):
			value = 't'
		
		command = "./caltool -filter " +value \
			+" from "+fromDate+" to "+toDate+" < "+self.main.getFileName();
		out = subprocess.Popen \
			(command, shell=True
			, stdout=subprocess.PIPE
			, stderr=subprocess.PIPE)
		
		#Check for errors
		errstr = ""
		for line in out.stderr.readlines():
				errstr += line.decode("utf-8")
		
		#Get all the results	
		if (errstr == ""):	
			self.main.logWindow.printMessage("Filtered successfully!\n")
			
			str = ""		
			for line in out.stdout.readlines():
				str += line.decode("utf-8")
				
			#Write results to file
			with open("logfile.tmp", 'w') as file:
				file.write(str)
			
			result = []
			#Read the file
			status = Cal.readFile("logfile.tmp",result)
			
			#Print error found
			if (status != 'OK'):
				self.main.logWindow.printMessage(status)
			
			#Run caltool -info and display in log
			else:
				#Clear FVP
				self.main.compWindow.clearTree()
	
				#Store pointer and tuple list
				self.main.pcal = result[0]
				self.main.compList = result[1]
				
				#Fill treeview
				self.main.compWindow.fillTree(self.main.compList)
				
				#Set unsaved
				self.main.setUnsaved()
			
			#Remove file
			try:
				os.remove("logfile.tmp")
			except OSError:
				pass
			
		else:	
			self.main.logWindow.printMessage(errstr+"\n")
	
		self.filPop.destroy()
	
	def eventsRadioClicked(self):
		self.filterBut.config(state="normal")
		
		self.eventSelect = 1
		self.todoSelect = 0
	
	def todoRadioClicked(self):
		self.filterBut.config(state="normal")
		
		self.eventSelect = 0
		self.todoSelect = 1
	
	def destroyFilterPop(self,event):
		self.filPop.destroy()
	
	def fileFilter(self):
		
		#Create modal popup
		self.filPop = Toplevel()
		
		#Bind ESC key
		self.filPop.bind("<Escape>",self.destroyFilterPop)
		
		#Center window
		x = (self.filPop.winfo_screenwidth() - self.filPop.winfo_reqwidth()) / 2
		y = (self.filPop.winfo_screenheight() - self.filPop.winfo_reqheight()) / 2
		self.filPop.geometry("+%d+%d" % (x, y))
 		
		#Add event options
		eventFrame = Frame(self.filPop)
		self.radEvents = Radiobutton(eventFrame,text="Events",value=1
							,command=self.eventsRadioClicked)
		self.radEvents.deselect()
		self.radEvents.pack(side=LEFT)
  		
		fromLabel = Label(eventFrame,text="From:")
		fromLabel.pack(side=LEFT)
  
		self.fromInput = Entry(eventFrame)
		self.fromInput.pack(side=LEFT)
  		
		toLabel = Label(eventFrame,text="To:")
		toLabel.pack(side=LEFT)
  		
		self.toInput = Entry(eventFrame)
		self.toInput.pack(side=LEFT)
 		
		#Add todo options
		todoFrame = Frame(self.filPop)
		self.radTodo = Radiobutton(todoFrame,text="Todo",value=2
						,command=self.todoRadioClicked)
		self.radTodo.pack(side=LEFT)
		self.radTodo.deselect()
		
		#Add Filter and Cancel buttons
		butFrame = Frame(self.filPop)
		self.filterBut = Button(butFrame,text="Filter",command=self.filterFromTo,state=DISABLED)
		self.filterBut.pack(side=LEFT)
		cancelBut = Button(butFrame,text="Cancel",command=self.filPop.destroy)
		cancelBut.pack(side=LEFT)
		
		eventFrame.pack(side=TOP)
		todoFrame.pack(side=LEFT)
		butFrame.pack(side=BOTTOM)
		
		#Make window modal
		self.filPop.grab_set()
		self.filPop.transient(self.main)
		self.filPop.wait_window(self.filPop)
		
		self.filPop.mainloop()
		
	def fileExit(self):
		self.main.showPrompt()
	
	def fileExitShort(self,event):
		self.fileExit()
		
	def todoPopup(self):
		#Create secondary window(Height set to arbitrary values for now)
		self.todoListPop = CheckList(self)
		self.todoListPop.wm_title("To-do List")
		self.todoListPop.minsize(400,200)
		
		#Center window
		x = (self.todoListPop.winfo_screenwidth() 
			- self.todoListPop.winfo_reqwidth()) / 2
		y = (self.todoListPop.winfo_screenheight() 
			- self.todoListPop.winfo_reqheight()) / 2
		self.todoListPop.geometry("+%d+%d" % (x, y))
		
		self.todoListPop.mainloop()
		
	def todoList(self):
		self.todoPopup()
	
	def todoListShort(self,event):
		self.todoList()	
		
	def destroyUndoPop(self,event):
		self.undoPop.destroy()
		
	def todoUndo(self):
		
		self.undoPop = Toplevel()
		self.undoPop.minsize(220,150)
		self.undoPop.wm_title("Undo")
		
		#Bind ESC to quit
		self.undoPop.bind("<Escape>",self.destroyUndoPop)
		
		#Create message
		msg = Message(self.undoPop,text="Warning: all Todo components removed since the last save will be restored!")
		msg.config(width='200')
		
		#Create buttons
		butFrame = Frame(self.undoPop)
		undo = Button(butFrame,text="Undo",command=self.undoPop.destroy)
		cancel = Button(butFrame,text="Cancel",command=self.undoPop.destroy)
		undo.pack(side=LEFT)
		cancel.pack(side=RIGHT)
	
		butFrame.pack(side=BOTTOM)
		msg.pack(side=TOP)
		
		#Make window modal
		self.undoPop.grab_set()
		self.undoPop.transient(self.main)
		self.undoPop.wait_window(self.undoPop)
		
	
	def todoUndoShort(self,event):
		self.todoUndo()
		
	def helpDateMask(self):
		datemsk = filedialog.askopenfilename(filetypes=(("All files", "*")
													,("Calendar files", "*.ics")))
		
		if (datemsk != ""):
			os.environ['DATEMSK'] = datemsk
			self.main.logWindow.printMessage("Datemask variable set successfully!\n")
		
	def destroyAboutPrompt(self,event):
		self.aboutPop.destroy()
	
	def helpAbout(self):
		
		#Create popup window
		self.aboutPop = Toplevel()
		pop = self.aboutPop
		pop.minsize(200,75)
		
		#Bind ESC key to quitting window
		self.aboutPop.bind("<Escape>",self.destroyAboutPrompt)
		
		#Create OK Button
		ok = Button(pop,text="Ok",command=pop.destroy)
		
		#Center the window
		x = (pop.winfo_screenwidth() - pop.winfo_reqwidth()) / 2
		y = (pop.winfo_screenheight() - pop.winfo_reqheight()) / 2
		pop.geometry("+%d+%d" % (x, y))
		
		#Create labels
		name = Label(pop,text="Xcal")
		author = Label(pop,text="Author: Karol Zdebel")
		ver = Label(pop,text="Compatible with iCalendar V2.0")
		
		name.pack()
		author.pack()
		ver.pack()
		ok.pack(side=BOTTOM)
				
		pop.mainloop()
		
		print("Get info about xcal")
	
	def enableMenu(self):
		for i in range (0,6):
			self.fileMenu.entryconfig(i,state=NORMAL)
		
		for i in range(0,3):
			self.todoMenu.entryconfig(i,state=NORMAL)
	
		for i in range(0,3):
			self.helpMenu.entryconfig(i,state=NORMAL)
	
	def enableDataStore(self):
		for i in range (0,3):
			self.databaseMenu.entryconfig(i,state=NORMAL)
	
	def dataStoreAll(self):
		data = []
		Cal.getData(self.main.pcal,self.main.calIndex,data)
	
		#Store all organizers
		for organizer in data[0]:
			
			#Check if organizer is already in table
			if (not self.main.inTable(organizer[0])):
				self.main.addOrganizer(organizer)
		
		#Store all event information
		for event in data[1]:

			if (self.main.eventInTable(event[0],event[1])):
				continue
			
			#Get org_id
			self.main.cursor.execute("SELECT org_id FROM ORGANIZER WHERE name=%s",(event[3],))
			id = self.main.cursor.fetchone()
			
			#Clear cursor
			for i in self.main.cursor:
				j = i
			
			self.main.addEvent(event,id[0])
			
		#Store all events
		for todo in data[2]:
			
			if (self.main.todoInTable(todo[0])):
				continue
			
			#Get org_id
			self.main.cursor.execute("SELECT org_id FROM ORGANIZER WHERE name=%s",(todo[2],))	
			id = self.main.cursor.fetchone()
			
			#Clear cursor
			for i in self.main.cursor:
				j = i
			
			self.main.addTodo(todo,id[0])
			
		#Enable clear if not empty
		if (not self.main.isEmptyTable()):
			self.main.menuBar.enableClear()
			
		self.main.logWindow.printMessage(self.main.getStatus())
		
	def dataStoreSelected(self):
		
		dict = self.main.compWindow.compTree.item(self.main.compWindow.selectItem)
		index = (dict['text'])
		tuple = (index,)
		list = [tuple]
		
		data = []
		Cal.getData(self.main.pcal,list,data)
		
		#Store all organizers
		if (len(data[0]) > 0):
			for organizer in data[0]:
				
				#Check if organizer is already in table
				if (not self.main.inTable(organizer[0])):
					self.main.addOrganizer(organizer)
		
		#Store all event information
		for event in data[1]:
			
			if (event is None):
				continue
			
			if (self.main.eventInTable(event[0],event[1])):
				continue
			
			#Get org_id
			self.main.cursor.execute("SELECT org_id FROM ORGANIZER WHERE name=%s",(event[3],))
			id = self.main.cursor.fetchone()
			
			if (id is None):
				continue
			
			#Clear cursor
			for i in self.main.cursor:
				j = i
			
			self.main.addEvent(event,id[0])
			
		#Store all events
		for todo in data[2]:
			if (todo is None):
				continue
			
			if (self.main.todoInTable(todo[0])):
				continue
			
			#Get org_id
			self.main.cursor.execute("SELECT org_id FROM ORGANIZER WHERE name=%s",(todo[2],))	
			id = self.main.cursor.fetchone()
			
			#Clear cursor
			for i in self.main.cursor:
				j = i
		
			self.main.addTodo(todo,id[0])
	
		#Enable clear if not empty
		if (not self.main.isEmptyTable()):
			self.main.menuBar.enableClear()
			
		self.main.logWindow.printMessage(self.main.getStatus())
		
	def dataClear(self):
		#clear database
		self.main.clearAllTable()
		self.main.logWindow.printMessage(self.main.getStatus())
		self.main.menuBar.disableClear()
		
	def dataStatus(self):
		#database status
		self.main.logWindow.printMessage(self.main.getStatus())
		
	def selectCustom(self):
		self.main.runStatement(self.entryCustom.get())
		self.querylog.printMessage("Performed custom SQL Statement '"+self.entryCustom.get()+"'\n")
		self.querylog.printMessage("\n--------------------------------------------------------\n")
		
	def selectDisplay(self):
		summaries = self.main.getSummaries(self.entryDisplay.get())
		self.querylog.printMessage("Found the following items for "+self.entryDisplay.get()+":\n\n")
		
		for sum in summaries:
			self.querylog.printMessage("  -"+sum+"\n")
			
		self.querylog.printMessage("\n--------------------------------------------------------\n")
	
	def selectLocation(self):
		num = self.main.locationCount(self.entryLocation.get())
		self.querylog.printMessage("Found "+str(num)+" events taking place in "+self.entryLocation.get()+"\n\n")
		self.querylog.printMessage("\n--------------------------------------------------------\n")
	
	def selectDate(self):
		summaries = self.main.getEventsBefore(self.entryDate.get())
		
		self.querylog.printMessage("Found the following events taking place before "+self.entryDate.get()+"\n\n")
		for i in summaries:
			self.querylog.printMessage("   -"+i[0]+"\n\n")
		self.querylog.printMessage("\n--------------------------------------------------------\n")
		
	def selectPriority(self):
		num = self.main.priorityCount(self.entryPriority.get())
		self.querylog.printMessage("Found "+str(num)+" todos with priority "+self.entryPriority.get()+"\n\n")
		self.querylog.printMessage("\n--------------------------------------------------------\n")
		
	def selectSummary(self):
		info = self.main.matchSummaries(self.entrySummary.get())
		self.querylog.printMessage("Found the following information for event summary '"+self.entrySummary.get()+"'\n\n")
		
		for i in range(0,len(info[0])):
			self.querylog.printMessage("location: "+info[1][i]+"\n")
				
			self.querylog.printMessage("date: "+info[0][i].strftime("%Y-%m-%d")+"\n\n")
				
				
		
		self.querylog.printMessage("\n--------------------------------------------------------\n")
		
	def submitQuery(self):

		val = self.selected.get()
		if (val == '0'):
			self.selectDisplay()
			
		if (val == '1'):
			self.selectLocation()
			
		if (val == '2'):
			self.selectDate()
			
		if (val == '3'):
			self.selectPriority()
		
		if (val == '4'):
			self.selectSummary()
			
		if (val == '5'):
			self.selectCustom()
		
	def helpQuery(self):
		self.helppop = Tk()
		
		message = Message(self.helppop,text=self.main.describeTables())
		message.config(width=900)
		message.pack(side=BOTTOM)
				
	def deactivateQuery(self):
		print("Deactivated query to False")
		self.queryActive = False
					
	def dataQuery(self):	
		self.querypop = Toplevel()
		self.querylog = LogWindow(self.querypop)
		
		self.querymain = Frame(self.querypop)
		choices = Frame(self.querymain)
		
		#Create "Display the items of organizer.." choice
		display = Frame(choices)
		labDisplay = Label(display,text="Display the items of organizer: ")
		labDisplay.pack(side=LEFT)
		
		self.selected = tkinter.StringVar() 
		
		self.entryDisplay = Entry(display)
		self.entryDisplay.pack(side=LEFT)
		
		self.butDisplay = Radiobutton(display,value=0,variable=self.selected)
		self.butDisplay.pack(side=LEFT)
		display.pack(side=TOP)
		
		#Create "How many events take place in .." choice
		location = Frame(choices)
		labLocation = Label(location,text="How many events take place in: ")
		labLocation.pack(side=LEFT)
		
		self.entryLocation = Entry(location)
		self.entryLocation.pack(side=LEFT)
		
		self.butLocation = Radiobutton(location,value=1,variable=self.selected)
		self.butLocation.pack(side=LEFT)
		location.pack(side=TOP)
	
		#Create "How many events take place before.." choice
		date = Frame(choices)
		labDate = Label(date,text="Display events before(YYYY-MM-DD): ")
		labDate.pack(side=LEFT)
		
		self.entryDate = Entry(date)
		self.entryDate.pack(side=LEFT)
		
		self.butDate = Radiobutton(date,value=2,variable=self.selected)
		self.butDate.pack(side=LEFT)
		date.pack(side=TOP)

		#Create "How many TODOs have priority.." choice
		priority = Frame(choices)
		labPriority = Label(priority,text="How many to-do's have priority: ")
		labPriority.pack(side=LEFT)
		
		self.entryPriority = Entry(priority)
		self.entryPriority.pack(side=LEFT)
		
		self.butPriority = Radiobutton(priority,value=3,variable=self.selected)
		self.butPriority.pack(side=LEFT)
		priority.pack(side=TOP)

		#Create "Display events with summary: " choice
		summary = Frame(choices)
		labSummary = Label(summary,text="Display events with summary: ")
		labSummary.pack(side=LEFT)
		
		self.entrySummary = Entry(summary)
		self.entrySummary.pack(side=LEFT)
		
		self.butSummary = Radiobutton(summary,value=4,variable=self.selected)
		self.butSummary.pack(side=LEFT)
		summary.pack(side=TOP)
		
		#Create custom " choice
		custom = Frame(choices)
		labCustom = Label(custom,text="Custom SQL statement: ")
		labCustom.pack(side=LEFT)
		
		self.entryCustom = Entry(custom)
		self.entryCustom.insert(0,"SELECT")
		self.entryCustom.pack(side=LEFT)
		
		self.butCustom = Radiobutton(custom,value=5,variable=self.selected)
		self.butCustom.pack(side=LEFT)
		custom.pack(side=TOP)
		
		choices.pack(side=TOP)
		
		butFrame = Frame(self.querymain)
		
		#Submit button
		self.butSubmit = Button(butFrame,text="Submit",command=self.submitQuery)
		self.butSubmit.pack(side=RIGHT)
		
		#Help Button
		self.butHelp = Button(butFrame,text="Help",command=self.helpQuery)
		self.butHelp.pack(side=LEFT)
		
		butFrame.pack(side=BOTTOM)
		self.querymain.pack(side=TOP)	
		self.querylog.pack(side=BOTTOM)
			
	def enableClear(self):
		self.databaseMenu.entryconfig(3,state=NORMAL)
		
	def disableClear(self):
		self.databaseMenu.entryconfig(3,state=DISABLED)
	
	def __init__(self,main):
		super(MenuBar,self).__init__(main)
		
		self.main = main
		
		#Create file menu
		self.fileMenu = Menu(self)
		self.fileMenu.add_command(label="Open",command=self.fileOpen
							, accelerator="Ctrl+O")
		self.fileMenu.add_command(label="Save",command=self.fileSave
							, accelerator="Ctrl+S",state = DISABLED)
		self.fileMenu.add_command(label="Save As",command=self.fileSaveAs
							,state = DISABLED)
		self.fileMenu.add_command(label="Combine",command=self.fileCombine
							,state = DISABLED)
		self.fileMenu.add_command(label="Filter",command=self.fileFilter
							,state = DISABLED)
		self.fileMenu.add_command(label="Exit",command=self.fileExit
							, accelerator="Ctrl+X")
		
		#Create todo menu
		self.todoMenu = Menu(self)
		self.todoMenu.add_command(label="To-do List",command=self.todoList
							, accelerator="Ctrl+T",state = DISABLED)
		self.todoMenu.add_command(label="Undo",command=self.todoUndo
							, accelerator="Ctrl+Z",state = DISABLED)
		
		#Create help menu
		self.helpMenu = Menu(self)
		self.helpMenu.add_command(label="Date Mask",command=self.helpDateMask)
		self.helpMenu.add_command(label="About xCal",command=self.helpAbout )
		
		#Create database menu
		self.databaseMenu = Menu(self)
		self.databaseMenu.add_command(label="Store All",command=self.dataStoreAll
									, state = DISABLED)
		self.databaseMenu.add_command(label="Store Selected"
									,command=self.dataStoreSelected, state = DISABLED)
		
		#Activate clear menu if database table is not empty
		self.databaseMenu.add_command(label="Clear",command=self.dataClear)
		if (self.main.isEmptyTable()):
			self.disableClear()
		
		self.databaseMenu.add_command(label="Status",command=self.dataStatus)
		self.databaseMenu.add_command(label="Query",command=self.dataQuery)
		
		
		#Activate starting hotkeys
		self.main.bind("<Control-o>", self.fileOpenShort)
		self.main.bind("<Control-x>", self.fileExitShort)
		
		#Add menus to main menu
		self.add_cascade(label="File",menu=self.fileMenu)
		self.add_cascade(label="Todo",menu=self.todoMenu)
		self.add_cascade(label="Help",menu=self.helpMenu)
		self.add_cascade(label="Database",menu=self.databaseMenu)

class CompWindow(Frame):

	treeIndex = 0
	selected = 0

	def fillTree(self,compList):
		num = 0;
		
# 		self.compTree.insert('', 'end', text="Hello")
# 		self.compTree.insert('', '1', text="Hello")
		self.main.calIndex = []
		for tup in compList:
			self.treeIndex+=1
			indexTup = (self.treeIndex,)
			self.main.calIndex.append(indexTup)
			self.compTree.insert('','end',values = tup,text=self.treeIndex)
	
	#Add buttons to button frame
	
	def clearTree(self):
		for i in self.compTree.get_children():
			self.compTree.delete(i)
		self.treeIndex=0;
	
	def createButtons(self):
		self.showSelectedBut = Button(self.butFrame,text="Show Selected" \
									,command=self.showSelected, state = DISABLED)
		self.showSelectedBut.pack(side=LEFT)
		
		self.extractEventBut = Button(self.butFrame,text="Extract Events" \
									,command=self.extractEvents, state = DISABLED)
		self.extractEventBut.pack(side=LEFT)
		
		self.extractXpropBut = Button(self.butFrame,text="Extract X-props" \
									,command=self.extractXprops,state = DISABLED)
		self.extractXpropBut.pack(side=LEFT)
		
	def hideExtractButtons(self):
		self.extractEventBut.config(state="disabled")
		self.extractXpropBut.config(state="disabled")
	
	def hideSelectButton(self):
		self.showSelectedBut.config(state="disabled")
	
	def showSelectButton(self):
		self.showSelectedBut.config(state="normal")
	
	def showExtractButtons(self):
		self.extractEventBut.config(state="normal")
		self.extractXpropBut.config(state="normal")
		
	def readFile(self,name):
		with open(name, 'r') as file:
			content=file.read()
	
		return content
		
	#show selected component
	def showSelected(self):
		
		dict = self.compTree.item(self.selectItem)
		index = (dict['text'])
		tuple = (index,)
		list = [tuple]
		
		status = Cal.writeFile("logfile.tmp" \
			,self.main.pcal,list)
		
		#Error found	
		if (status != 'OK'):
			self.main.logWindow.printMessage(status+"\n")
			
		#Write file results to log
		else:
			data = self.readFile("logfile.tmp")
			self.main.logWindow.printMessage(data+"\n")
			
		#Remove file
		try:
			os.remove("logfile.tmp")
		except OSError:
			pass
	
	def extractEvents(self):
		
		command = "./caltool -extract e < "+self.main.getFileName()
		out = subprocess.Popen(command, shell=True
							, stdout=subprocess.PIPE
							, stderr=subprocess.PIPE)
		
		#Check for errors
		if (len(out.stderr.readlines()) > 0):
			self.main.logWindow.printMessage("Error while extracting: ")
			for line in out.stderr.readlines():
				self.main.logWindow.printMessage(line.decode("utf-8"))
		
		else:
			self.main.logWindow.printMessage("Extracted following event(s):\n")				
			for line in out.stdout.readlines():
				self.main.logWindow.printMessage(line.decode("utf-8"))
						
			self.main.logWindow.printMessage("\n")
		
	def extractXprops(self):
	
		command = "./caltool -extract x < "+self.main.getFileName()
		out = subprocess.Popen(command, shell=True
							, stdout=subprocess.PIPE
							, stderr=subprocess.PIPE)
		
		self.main.logWindow.printMessage("Extracted following x-prop(s):\n")				
		for line in out.stdout.readlines():
			self.main.logWindow.printMessage(line.decode("utf-8"))
					
		self.main.logWindow.printMessage("\n")
	
	def onSelected(self,event):
				
		self.showSelectButton()
		self.selectItem = self.compTree.focus()
		
	def __init__(self,main):
		super(CompWindow,self).__init__(main)
		
		self.main = main
		
		#Create button frame
		self.butFrame = Frame(self,height=30)
		self.butFrame.pack(side=TOP)
		self.butFrame.pack(fill=X)
		self.createButtons()
		
		#Add treeview
		self.compTree = ttk.Treeview(self)
		
		self.compTree["columns"]=("name","props","subs","summary")
		self.compTree.column("#0",anchor="w",width=50)
		self.compTree.column("name",width=100)
		self.compTree.column("props",width=50)
		self.compTree.column("subs",width=50)
		self.compTree.column("summary",width=150)
		self.compTree.heading("#0",text='No.',anchor = 'w')
		self.compTree.heading("name",text="Name")
		self.compTree.heading("props",text="Props")
		self.compTree.heading("subs",text="Subs")
		self.compTree.heading("summary",text="Summary")
		
		self.compTree.pack(side=LEFT)
		
		#Create selection
		self.compTree.bind("<ButtonRelease-1>", self.onSelected)
		
		#Add scroll bar to tree view
		scrollbar = ttk.Scrollbar(self,orient='vertical',command= self.compTree.yview)
		self.compTree.configure(yscrollcommand=scrollbar.set)
		scrollbar.pack(side="right",fill="both")

class LogWindow(Frame):
	
	def clearLog(self):
		self.textPad.delete(1.0,END)
	
	def printMessage(self,message):
		self.textPad.insert(END,message)
		self.textPad.see(END)
	
	def __init__(self,main):
		super(LogWindow,self).__init__(main)
		
		#Add label
		title = Label(self,text="Log Panel")
		title.pack()
		
		#Add text area
		self.textPad = Text(self,height=10,width=57)
		
		#Add scroll bar
		scrollbar = ttk.Scrollbar(self,orient='vertical',command= self.textPad.yview)
		self.textPad.configure(yscrollcommand=scrollbar.set)
		scrollbar.pack(side="right",fill="both")
		
		#Add clear button
		clearBut = Button(self,text="Clear",command=self.clearLog,width=45)
		clearBut.pack(side=BOTTOM)
		
		self.textPad.pack(side=LEFT)

class Xcal(Tk):
	
	logWindow = ""
	menuBar = ""
	compWindow = ""
	compList = []
	calIndex = []
	pcal = ""
	winTitle = ""
	saved = 1
	fileName = ""
	
	def setUnsaved(self):
		self.setTitle(self.getFileName()+"*")
		self.wm_title(self.getTitle())
		self.saved = 0
	
	def setSaved(self):
		if (self.saved == 0):
			title = self.getTitle()
			title = title[:-1]
			self.setTitle(title)
			self.saved = 1
	
	def getTitle(self):
		return self.winTitle
	
	def setTitle(self,name):
		self.wm_title(name)
		self.winTitle = name
 		
	def getFileName(self):
		return self.fileName
	
	def setFileName(self,name):
		self.fileName = name
 		
	def destroyShowPrompt(self,event):
		self.showPrompt.destroy()
		
	def showPrompt(self):
		self.showPrompt = Toplevel()
		self.showPrompt.wm_title("Quit")
		yes = Button(self.showPrompt,text="Yes",command=exit)
		no = Button(self.showPrompt,text="No",command=self.showPrompt.destroy)
		
		#Allow close with ESC key
		self.showPrompt.bind("<Escape>", self.destroyShowPrompt)
		
		yes.pack(side=LEFT)
		no.pack(side=RIGHT)
		
		msg = Message(self.showPrompt,text="Are you sure you'd like to quit?")
		msg.pack(side=TOP)
		
		x = (self.showPrompt.winfo_screenwidth() - self.showPrompt.winfo_reqwidth()) / 2
		y = (self.showPrompt.winfo_screenheight() - self.showPrompt.winfo_reqheight()) / 2
		self.showPrompt.geometry("+%d+%d" % (x, y))
		
		self.showPrompt.mainloop()
		
	def exit(self):
		sys.exit()
		
	def setDateMask(self):
		self.datemskpop.destroy()
		self.menuBar.helpDateMask()
		
	def destroyMskPop(self,event):
		self.datemskpop.destroy()
		
	def checkDatemask(self):
		if (os.environ.get('DATEMSK') == None):
			#Open prompt
			self.datemskpop = Toplevel()
			self.datemskpop.minsize(200,150)
			
			#Bind ESC to quit
			self.datemskpop.bind("<Escape>",self.destroyMskPop)
			
			#Center the window
			x = (self.datemskpop.winfo_screenwidth() \
				- self.datemskpop.winfo_reqwidth()) / 2
			y = (self.datemskpop.winfo_screenheight() \
				- self.datemskpop.winfo_reqheight()) / 2
			self.datemskpop.geometry("+%d+%d" % (x, y))
		
			#Add message
			msg = Message(self.datemskpop,text="DATEMSK variable is not set." \
						+ "Would you like to set this variable now?",width='175')
			msg.pack(side=TOP)
			
			#Add choice buttons
			notNow = Button(self.datemskpop,text="Not Now",command=self.datemskpop.destroy)
			yes = Button(self.datemskpop,text="Yes",command=self.setDateMask)
			
			yes.pack(side=LEFT)
			notNow.pack(side=RIGHT)
			
			#Keep prompt on top of other windows
			self.datemskpop.attributes("-topmost",True)
				
	def getPassword(self):
		password = input("Please enter database password:")
		return password
			
	def connectHost(self,user,host,password):
		try:
			self.cnx = mysql.connector.connect(user=user, password=password
	                              ,host=host
	                              ,database = user)
			return True
			
		except Error as e:
			return False
		
	def createOrganizerTable(self,cursor):
	    TABLE = (
	            "CREATE TABLE IF NOT EXISTS ORGANIZER ("
	            "org_id INT AUTO_INCREMENT, "
	            "name VARCHAR(60) NOT NULL, "
	            "contact VARCHAR(60) NOT NULL, "
	            "PRIMARY KEY (org_id))" 
	            )
	
	    cursor.execute(TABLE)
	    
	def createEventTable(self,cursor):
	    TABLE = (
	        "CREATE TABLE IF NOT EXISTS EVENT ("
	        "event_id INT AUTO_INCREMENT, "
	        "summary VARCHAR(60) NOT NULL, "
	        "start_time DATETIME NOT NULL, "
	        "location VARCHAR(60), "
	        "organizer INT,"
	        "FOREIGN KEY (organizer)"
	        "    REFERENCES ORGANIZER(org_id)"
	        "    ON DELETE CASCADE,"
	        "PRIMARY KEY (event_id) )" 
	        )
	
	    cursor.execute(TABLE)
	    
	def createTodoTable(self,cursor):
	    TABLE = (
	             "CREATE TABLE IF NOT EXISTS TODO ("
	             "todo_id INT AUTO_INCREMENT, "
	             "summary VARCHAR(60) NOT NULL, "
	             "priority SMALLINT, "
	             "organizer INT, "
	             "FOREIGN KEY (organizer)"
	             "    REFERENCES ORGANIZER(org_id)"
	             "    ON DELETE CASCADE,"
	             "PRIMARY KEY (todo_id) )"
	             )
	    
	    cursor.execute(TABLE)
			
	def addTodo(self, data,organizer):
		info = []
		for i in range(0,2):
			info.append(data[i])
		info.append(organizer)
		
		add = ("INSERT INTO TODO "
			"(summary,priority,organizer)"
			  "VALUES (%s,%s,%s)")
	    
		self.cursor.execute(add,info)
		self.cnx.commit()
		
	def addEvent(self,data,organizer):
		info = []
		for i in range(0,3):
			info.append(data[i])
			
		info.append(organizer)

		add = ("INSERT INTO EVENT "
			"(summary,start_time,location,organizer)"
			"VALUES (%s,%s,%s,%s)")
		self.cursor.execute(add,info)
		self.cnx.commit()
		
	def addOrganizer(self,data):
		add = ("INSERT INTO ORGANIZER "
				"(name,contact)"
				"VALUES (%s,%s)")
		self.cursor.execute(add,data)
		self.cnx.commit()
			
	def isEmptyTable(self):
		self.cursor.execute("SELECT * FROM ORGANIZER ")
		return (str(len(self.cursor.fetchall())) == "0")
			
	def inTable(self,name):
		self.cursor.execute("SELECT * FROM ORGANIZER WHERE (name = %s)",(name,))
		count = 0
		for i in self.cursor:
			count+=1
			
		if (count > 0):
			return True
		else:
			return False
		
	def eventInTable(self,summary,time):
		
		self.cursor.execute("SELECT * FROM EVENT WHERE (summary = %s)",(summary,))
		countSum = 0
		for i in self.cursor:
			countSum+=1
		
		self.cursor.execute("SELECT * FROM EVENT WHERE (start_time = %s)",(time,))
		countTime = 0
		for i in self.cursor:
			countTime +=1
			
		if (countSum == 0 and countTime == 0):
			return False
		else:
			return True
	
	def runStatement(self,statement):
		self.cursor.execute(statement)
		for i in self.cursor:
			j=1
	
	def todoInTable(self,summary):
		self.cursor.execute("SELECT * FROM TODO WHERE (summary = %s)",(summary,))
		countSum = 0
		for i in self.cursor:
			countSum+=1
			
		if (countSum > 0):
			return True

		else:
			return False
		
	def describeOrganizer(self):
		self.cursor.execute("DESCRIBE ORGANIZER")
		res = self.cursor.fetchall()
		
		columns = []
		
		for i in res:
			name = i[0]
			type = i[1]
			canNull = i[2]
			if (i[3] == "PRI"):
				key = "primary key"
			if (i[3] == "MUL"):
				key = "foreign key"
			else:
				key = "not a key"
			default = i[4]
			if (i[5] == "auto_increment"):
				autoinc = "does auto increment"
			else:
				autoinc = "does not auto increment"			
			
			columns.append("column: "+str(name)+  \
				" | type: "+str(type)+" | key: " \
			 	+str(key)+ " | default: "+str(default)+" | "+str(autoinc)+"\n")
			
		return columns
		
	def describeTodo(self):
		self.cursor.execute("DESCRIBE TODO")
		res = self.cursor.fetchall()
		
		columns = []
		
		for i in res:
			name = i[0]
			type = i[1]
			canNull = i[2]
			if (i[3] == "PRI"):
				key = "primary key"
			if (i[3] == "MUL"):
				key = "foreign key"
			else:
				key = "not a key"
			default = i[4]
			if (i[5] == "auto_increment"):
				autoinc = "does auto increment"
			else:
				autoinc = "does not auto increment"			
			
			columns.append("column: "+str(name)+  \
				" | type: "+str(type)+" | key: " \
			 	+str(key)+ " | default: "+str(default)+" | "+str(autoinc)+"\n")
			
		return columns
		 	
	def describeEvent(self):
		self.cursor.execute("DESCRIBE EVENT")
		res = self.cursor.fetchall()
		
		columns = []
		
		for i in res:
			name = i[0]
			type = i[1]
			canNull = i[2]
			if (i[3] == "PRI"):
				key = "primary key"
			if (i[3] == "MUL"):
				key = "foreign key"
			else:
				key = "not"
			default = i[4]
			if (i[5] == "auto_increment"):
				autoinc = "does auto increment"
			else:
				autoinc = "does not auto increment"
			
			columns.append("column: "+str(name)+  \
				" | type: "+str(type)+" | key: " \
			 	+str(key)+ " | default: "+str(default)+" | "+str(autoinc)+"\n")
			
		return columns
			
	def describeTables(self):
		org = self.describeOrganizer()
		orgDesc = "ORGANIZER Table Information:\n"
		for column in org:
			orgDesc += column
			
		orgDesc += "\n\n"
		
		event = self.describeEvent()
		eventDesc = "EVENT Table Information:\n"
		for column in event:
			eventDesc += column
			
		eventDesc += "\n\n"
		
		todo = self.describeTodo()
		todoDesc = "TODO Table Information:\n"
		for column in todo:
			todoDesc += column	
		
		return orgDesc+eventDesc+todoDesc
		
	def clearTodo(self,cursor):
	    cursor.execute("DELETE FROM TODO")
	    cursor.execute("ALTER TABLE TODO AUTO_INCREMENT = 1")
	    
	def clearEvent(self,cursor):
	    cursor.execute("DELETE FROM EVENT")
	    cursor.execute("ALTER TABLE EVENT AUTO_INCREMENT = 1")
	
	def clearOrganizer(self,cursor):
	    cursor.execute("DELETE FROM ORGANIZER")
	    cursor.execute("ALTER TABLE ORGANIZER AUTO_INCREMENT = 1")
	
	def clearAllTable(self):
	    self.clearTodo(self.cursor)
	    self.clearEvent(self.cursor)
	    self.clearOrganizer(self.cursor)
	    
	def getStatus(self):
		self.cursor.execute("SELECT * FROM ORGANIZER")
		orgNum = len(self.cursor.fetchall())
		
		self.cursor.execute("SELECT * FROM EVENT")
		eventNum = len(self.cursor.fetchall())
		
		self.cursor.execute("SELECT * FROM TODO")
		todoNum = len(self.cursor.fetchall())
		
		return ("Database has "+str(orgNum)+" organizers,"+str(eventNum)+
		        " events,"+str(todoNum)+" to-do items.\n")
	    
	def getSummaries(self,name):
		summaries = []
    	
    	#get org_id
		self.cursor.execute("SELECT org_id FROM ORGANIZER WHERE name=%s",(name,))
		res = self.cursor.fetchone()
		id = ""
		if (res is not None):
			id = str(res[0])
    	
		for i in self.cursor:
			j = 1
    	
		if (len(id) > 0):
			
			self.cursor.execute("SELECT summary FROM EVENT WHERE organizer=%s",(id[0],))
			for result in self.cursor:
				summaries.append(str(result[0]))
			
			self.cursor.execute("SELECT summary FROM TODO WHERE organizer=%s",(id[0],))
			for result in self.cursor:
				summaries.append(str(result[0]))
		
		return summaries

	def locationCount(self,name):
  
		self.cursor.execute("SELECT * FROM EVENT WHERE location=%s",(name,))
		count = 0
		for i in self.cursor:
			count+=1

		return count

	def priorityCount(self,val):
		self.cursor.execute("SELECT * FROM TODO WHERE priority=%s",(val,))
		count = 0
		for i in self.cursor:
			count+=1

		return count

	def matchSummaries(self,summary):
		
		results = []
		location = []
		time = []
		self.cursor.execute("SELECT * FROM EVENT WHERE summary=%s",(summary,))
		for i in self.cursor:
			location.append(i[3])
			time.append(i[2])
			print("location: "+str(i[2])+" time: "+str(i[3]))
			
		results.append(time)
		results.append(location)
		
		return results

	def getEventsBefore(self,date):
		
		results = []
		
		dt = datetime.datetime.strptime(date,"%Y-%m-%d")	
		self.cursor.execute("SELECT summary FROM EVENT WHERE (start_time < %s)",(dt,))
		for i in self.cursor:
			results.append(i)
			
		return results
	
	def __init__(self):
		if (len(sys.argv) < 2 or len(sys.argv) > 3):
			print("Please enter 2 to 3 arguments")
			exit()
		
		#Get host if one was specified
		if (len(sys.argv) == 3):
			self.host = str(sys.argv[2])
		
		if (len(sys.argv) == 2):
			self.host = "dursley.socs.uoguelph.ca"
		
		#Get username from argument one
		self.userName = str(sys.argv[1])
		
		#Prompt for password to database
		self.password = self.getPassword()
		
		#Try to connect to database
		count = 0
		
		#Attempt to connect 3 times
		while (not self.connectHost(self.userName,self.host,self.password)):
			print("Could not connect to database, try again.")
			
			if (count is 2):
				exit()
			
			self.password = self.getPassword()
			count+=1
		
		self.cursor = self.cnx.cursor()	
		
		#Create tables
		self.createOrganizerTable(self.cursor)
		self.createTodoTable(self.cursor)
		self.createEventTable(self.cursor)
 		
		super(Xcal,self).__init__()
		
		#Create main window and set default name
		self.wm_title("xcal")
		self.minsize(width=430,height=500)
		
		#Override exit button
		self.protocol("WM_DELETE_WINDOW", self.showPrompt)
		
		#center window
		x = ((self.winfo_screenwidth() - self.winfo_reqwidth()) / 2)-215
		y = ((self.winfo_screenheight() - self.winfo_reqheight()) / 2)-250
		self.geometry("+%d+%d" % (x, y))
		
		#Add main menu bar
		self.menuBar = MenuBar(self)
		self.config(menu=self.menuBar)
		
		#Add component window
		self.compWindow = CompWindow(self)
		self.compWindow.pack(side=TOP)
		
		#Add log window
		self.logWindow = LogWindow(self)
		self.logWindow.pack(side=BOTTOM)
		
		#Check for DATEMSK 
		self.checkDatemask()
		
		#Run the program
		self.mainloop()
		
		

xcal = Xcal()
