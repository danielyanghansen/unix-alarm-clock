# Tests

## Note

We have one known error, that is the clock might be one hour off because the internal hardware clock and different softwear config. the clock does not always align. This is solved by setting the alarm one hour ahead.


## Test alarm and zombies  

* Start the program. The main menu should appear.  
* Press "s", and enter the alarm time 30 seconds from now in given format.   
* After 30 seconds the alarm should make sound.   
* Press "l", and check that the list that appears does not have any alarms listed.

## Test that zombies actually disappears
* Prepare two terminals, and navigate to the directory where the program is located.
* In one of the terminals, type `ps` to check that there are no alarm processes running.
* Start the program in the second terminal. The main menu should appear. 
* Press "s", and enter the alarm time about 2 minutes from now in given format.
* While the alarm process is running, type `ps` in the first terminal window to see that there is an alarm process running.
* After the alarm has rung, type `ps` again in the first terminal window to see that the alarm process has been removed.

## Test deleting of existing and non-existing index

* Start the program. The main menu should appear. 
* Press "s", and enter the alarm time two hours from now in given format. 
* Press "s", and enter the alarm time three hours from now in given format. 
* Press "s", and enter the alarm time four hours from now in given format.  
* Press "l", and check that the list that appears contains the three entered alarms.  
* Press "c", and try canceling an index on index #5.  
* This should return "This index is already deleted" -message.   
* Now press "l", and see that nothing has changed.  
* Now press "c", and try canceling a listed index.  
* Press "l", and check that the correct alarm has been removed.   

## Test alarm array-size and exeption handling

* Start the program. The main menu should appear. 
* Press "s", and enter the alarm time two days from now in given format.  
* Repeat this 9 more times, for a total of 10 alarms.   
* Press "s" again and check if your recive "No spot availible, canceling alarm planer" -message.  
* Press "c", and try canceling an index on index #11.   
* This should return "index out of bounds, try again" -message. 


## Test invalid alarm time and program exit 

* Start the program. The main menu should appear. 
* Press "s", and enter the alarm time two hours in the **past** in given format.  
* Check that the alarm immediately rings.   
* Press "l", and check that the list is empty.   
* Press "x", and the program should exit.   




