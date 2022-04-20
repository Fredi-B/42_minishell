# Minishell  

## Funktionen  
  
  ### signal / sigaction  

  - sigaction prefered ([see here](https://www.youtube.com/watch?v=83M5-NPDeWs&list=PL7_TuD9ZDMhg5uLHLyd8em13XBKfjzCzR))  
    - more options, informations..  
  
## Git
- git in cmdline gibt ne schöne Übersicht 
- git config --global user.name "Fredi-B" 
- git config --global user.email "fredibechtoldt@outlook.com" 
  - Dann weiß git wer ich bin 
- git branch 
  - Shows all branches 
  - git branch \<new branch name\> 
  - git branch –m \<new name for branch\> 
  - git branch –d \<branch name\> 
    - Deletes branch (-D forces it)  
  - git push --set-upstream origin \<branch name\>  
    - enables pushing a branch
  - git push origin --delete \<remoteBranchName\>  
      - delete branch remotely
- git checkout \<branch to switch to\> 
  - git switch \<branch to switch to\> 
    - Is recommended, but not working on mac. checkout is more versatile 
- git merge \<branch to merge\> 
  - While in branch you want to merge in  
- git reset --soft Head~
  - undoing last commit (add number at the end to undo x commits)
  - git reset --soft \<hashnumber\>
    - go back to specified commit, keep changes locally
	  - --hard go back, dismiss changes
- git pull --rebase  
  - if error msg: reconcile divergent branches when trying to push:

  
  ## Parsing
  ### lore (dt. Hintergrund)
  #### Tokenizer
  - Ein __Tokenizer__ (auch __lexikalischer Scanner__, kurz __Lexer__) ist ein Computerprogramm zur Zerlegung von Plain text in Folgen von logisch zusammengehörigen Einheiten, so genannte Token.  
  - Ein Tokenizer kann Bestandteil eines Parsers sein und hat dort vorverarbeitende Funktion. Er erkennt innerhalb der Eingabe Schlüsselwörter, Bezeichner, Operatoren und Konstanten. Diese bestehen aus mehreren Zeichen, bilden aber jeweils logische Einheiten, sogenannte Token. Diese werden an den Parser zu weiteren Verarbeitung (d. h. syntaktischen Analyse) weitergereicht.([see wiki](https://de.wikipedia.org/wiki/Tokenizer))