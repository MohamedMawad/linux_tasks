# List of linux APPs:  
This Repo has some linux applications, like simple shell, some linux commands, etc    

## Build Command:  
`cd LINUX_TASKS`  
`make APP=<application_name>`  
  - Example:  
    - `make APP=femtoShell`  

## Run Command: 
`cd LINUX_TASKS/bin`  
`./<application_name>`  
  - Example:  
    - `./femtoShell`  

## APPs:  
**1. femtoShell APP:**  
  - Simple Shell that supports the following features:  
    * Interactive colored prompt with username, hostname and current path.  
    * Parse the command line and run all the external commands with the command line args like ls, pwd, ... etc.    
    * Parse = sign and add the name and value to the process local variables.  
    * Parse $ sign and expand the value of the specified name after $ from the local/environment variables or ignore it if it is not found on local or environment variables.    
    * Built in commands:  
      1. exit: terminate the femtoShell program.    
      2. cd: change directory command that supports ~ sign or no args(home dir) and - sign(previous path).  
      3. set: print all local variables with their values.  
      4. unset: remove one or more specified local variable names.  
      5. export: move one or more local variables to the environment variables.   
  - femtoShell Output Example:  
```
moawad@moawad-VB:~/linux_tasks$ ./bin/femtoShell 
moawad@moawad-VB:/home/moawad/linux_tasks# pwd
/home/moawad/linux_tasks
moawad@moawad-VB:/home/moawad/linux_tasks# cd ~
moawad@moawad-VB:/home/moawad# cd -
moawad@moawad-VB:/home/moawad/linux_tasks# var1=123
moawad@moawad-VB:/home/moawad/linux_tasks# var2=456
moawad@moawad-VB:/home/moawad/linux_tasks# set
var2=456
var1=123
moawad@moawad-VB:/home/moawad/linux_tasks# var3=$var2 var4=$var1 set
var4=123
var3=456
var2=456
var1=123
moawad@moawad-VB:/home/moawad/linux_tasks# unset var3 var4
moawad@moawad-VB:/home/moawad/linux_tasks# set
var2=456
var1=123
moawad@moawad-VB:/home/moawad/linux_tasks# export var1
moawad@moawad-VB:/home/moawad/linux_tasks# printenv var1
123
moawad@moawad-VB:/home/moawad/linux_tasks# echo $var82

moawad@moawad-VB:/home/moawad/linux_tasks# echo $var2
456
moawad@moawad-VB:/home/moawad/linux_tasks# echo $HOME
/home/moawad
moawad@moawad-VB:/home/moawad/linux_tasks# exit
moawad@moawad-VB:~/linux_tasks$  
```
**2. echo command:**  
  - used to print a user string on stdout  
  - echo Output Example:
```
moawad@moawad-VB:~/linux_tasks/bin$ ./echo Hello World  
Hello World   
moawad@moawad-VB:~/linux_tasks/bin$   
```
**3. pwd command:**  
  - used to print current working directory  
  - pwd Output Example:
```
moawad@moawad-VB:~/linux_tasks/bin$ ./pwd
/home/moawad/linux_tasks/bin  
```
**4. cp command:**  
  - copy a file to another file and keep the original one  
  - cp Output Example:
```
moawad@moawad-VB:~/linux_tasks/bin$ ls  
cp  test_file  
moawad@moawad-VB:~/linux_tasks/bin$ ./cp test_file test_file_cp  
moawad@moawad-VB:~/linux_tasks/bin$ ls  
cp  test_file  test_file_cp  
```
**5. mv command:**  
  - move a file to another file and remove the original one  
  - mv Output Example:
```
moawad@moawad-VB:~/linux_tasks/bin$ ls  
mv  test_file  
moawad@moawad-VB:~/linux_tasks/bin$ ./mv test_file test_file_mv  
moawad@moawad-VB:~/linux_tasks/bin$ ls  
mv  test_file_mv   
```