# minishell

A mini shell implementation that provides a command-line interface with basic functionalities.

## Requirements

- UNIX-like operating system (Linux, macOS)
- GCC compiler
- readline library

## Installation

- Clone the repository:  
```
git clone <git@github.com:Fredi-B/42_minishell.git>
``` 
- Navigate to the project directory: `cd 42_minishell`
- Compile the source code: `make`

## Usage

- Run the minishell: `./minishell`
- Enter commands at the prompt and press Enter to execute them.
### Use the following built-in commands:
- `echo [-n] [message]`: Display a message (with the option -n to omit the trailing newline).
- `cd [directory]`: Change the current directory to the specified directory.
- `pwd`: Print the current working directory.
- `export`: List all environment variables.
- `env`: Display the current environment.
- `unset [variable]`: Remove the specified environment variable.
- `exit`: Exit the minishell.
### Other commands
- The minishell supports executing commands by entering the command name (based on the PATH variable or using a relative or absolute path).
### Redirections:
- `< [file]`: Redirect input from a file.
- `> [file]`: Redirect output to a file (overwrite if the file already exists).
- `>> [file]`: Redirect output to a file (append if the file already exists).
- `<< [delimiter]`: Read input until a line containing the delimiter is seen (does not update the history).
- Pipes `|`: Connect the output of one command as the input of the next command.
- Environment variables: Use `$[variable]` to expand to their values.
- `$?`: Expand to the exit status of the most recently executed foreground pipeline.
### Signals:
- `ctrl-C`: Display a new prompt on a new line.
- `ctrl-D`: Exit the minishell.
- `ctrl-\`: Do nothing.

## Project Guidelines

- The minishell should display a prompt when waiting for a new command.
- It should have a working history feature.
- It should not interpret unclosed quotes or special characters not required by the subject.
- Handling of single quotes ' and double quotes " should prevent interpretation of meta-characters.
- Only one global variable should be used.
- The implementation should follow behavior similar to bash for ctrl-C, ctrl-D, and ctrl-.
- The readline() function may cause memory leaks, but your own code should not have memory leaks.

Note: Limit yourself to the requirements mentioned in the subject. If in doubt about a requirement, refer to bash as a reference.