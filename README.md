# minishell
A (mini) shell application built as an excersize to better understand process creation and management, forking, and signals.

## Build
`git clone --recurse-submodules https://github.com/jacksonwb/minishell.git; cd minishell; make`

## Functionality
The following built-ins are provided:
* echo
* cd
* setenv
* unsetenv
* env
* exit

`~` and `$` expansion is managed

Muliple commands can be executed by seperating statements with a `;`

