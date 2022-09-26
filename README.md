# List of linux APPs:  
This Repo has some linux applications, like simple shell, some linux commands, etc    

## Build Command:  
`cd LINUX_TASKS`  
`make APP=<application_name>`  

## Run Command: 
`cd LINUX_TASKS/bin`  
`./<application_name>`  

## APPs:  
**1. femtoShell APP:**  
  - Simple Shell that supports echo for user input and exit command  
  - femtoShell Output Example:
```
Femto_Shell_Prompt:# Hello   
You Said: Hello   
Femto_Shell_Prompt:# Test the shell  
You Said: Test the shell  
Femto_Shell_Prompt:# exit  
Good Bye :)  
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