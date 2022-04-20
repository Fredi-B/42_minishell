# minishell
## Questions:
- work with branches in git?
- any resources to read before starting?
- which libft to use?
- is there a 42_tester?
- what are critical steps/inputs (others say)?
- how do we check the input?  
  - "Search and launch the right executable (based on the PATH variable or using a relative or an absolute path)."  
    - do we nee to handle /bin/ls as input or just ls?
- how to do error handling?
- how to do piping/redirections? same as in pipex?
- how to divide work? 
  - piping/redirections = pipex?
  - ctrl+ = minitalk?
- how to structure program, makefile and .h file?
- comments for functions?
- prototype functions in .c file?
- want to track progress? backlog, wip...?  
  
- open all outfiles? i.e. truncat outfiles even if they aren't the last one? ([see slack](https://42born2code.slack.com/archives/CMX2R5JSW/p1643892538020239))

## TODO before start:
- create folder(s)
- create Makefile
- create markdown

## TODO (from pdf):
It must implement the builtins:
- echo with option -n
- cd with only a relative or absolute path
- pwd with no options
- export with no options
- unset with no options
- env with no options or arguments
- exit with no options
- important!:
	- ’ inhibit all interpretation of a sequence of characters.
	- " inhibit all interpretation of a sequence of characters except for $.

- Redirections:
	- "<" should redirect input.
	- ">" should redirect output.
	- “<<” read input from the current source until a line containing only the delimiter is seen. it doesn’t need to update history!
	- “>>” should redirect output with append mode.
- Pipes | The output of each command in the pipeline is connected via a pipe to the
input of the next command.
- Environment variables ($ followed by characters) should expand to their values.
- $? should expand to the exit status of the most recently executed foreground
pipeline.
- ctrl-C ctrl-D ctrl-\ should work like in bash.
- When interactive:
	- ctrl-C print a new prompt on a newline.
	- ctrl-D exit the shell.
	- ctrl-\ do nothing.

readline function can produce some leak you don’t need to fix this. But beware your
own code should not produce leaks. You should limit yourself to the subject description.
Anything not asked is not required.
For every point, if you have any doubt take bash as a reference.

## pwd
just a print statement of the current directory
## env
just a print statement of the current envioremntal settings/folders

## checks to perform:
- input examples
- 42_tester
- leaks  

## road to minishell
Arno:
- handle ctrl -
- interactive mode
- builtins
- $  

Fred:
- lauch executables
- redirections
- pipes  

Parsing:
- get executables
- single quotes
- doubles quotes
- redirections
- pipes
- $
- builtins

## funny cmds
- echo hallo >outfile | <infile | cat  
- cat outfile | \<-- passt vllt schon 
- adf <as <adf adf adf | adf adsf <adf saf \<-- zweimal nacheinander: free(): double free detected in tcache 2   Aborted (core dumped)
- cat

## tests
### built ins
- echo $HOME"123"  
- echo -n $HOME"123"  
- cd ..  
- pwd  
- env  
- export test  
- unset test  
- exit  

### cmd
- ls  
- ls -l -a  


