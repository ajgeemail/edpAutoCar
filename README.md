# git_edpAutoCar
Uni project for autonomous obstacle avoiding RC car. Ideally all the code for the project exists on here, and will be updated whenever changes are made to any of the files. 

Commit and push:
- each time a change has been made to your code and it's in a working condition (so you can revert to this working condition if it breaks after other changes) 
- and at the end of each session (so it's the most up to date copy on here).

## GETTING GIT AND GITHUB:
1. Download and install git as per tutorial:
https://www.youtube.com/watch?v=albr1o7Z1nw - I use git bash instead of unix like he does in this tutorial so that's what the instructions below are based on, should work either way though.

2. Create GITHUB account.

## START INSTRUCTIONS:
1. To get repository from here to your PC - copy clone URL (from green clone or download button) from the top right of this github page
2. Navigate in your windows explorer to your chosen destination folder (where you want the repository to be)
3. Right click in window and open Git Bash
4. enter "git clone *PASTE COPIED URL HERE*" - this will clone the github repository to your directory

**AFTER CHANGES MADE READY TO BE UPLOADED FROM PC TO GITHUB (i.e. YOU'VE PUT YOUR CODE IN THE FOLDER OR UPDATED THE CODE YOU ALREADY PUT THERE**

5. Open Git Bash from inside git_edpAutoCar repository as above.
6. Enter "git status" *COMPARES YOUR CODE TO GITHUB VERSION*
7. Enter "git add ." - to add everything OR "git add *filename*" - to add specific file *(ADDS UNTRACKED CHANGES/NEW FILES READY TO BE COMMITTED)*
8. Enter "git commit -m "Quick description of commit" " *(COMMITS ALL TRACKED/ADDED CHANGED TO LOCAL DIRECTORY)*
9. Enter "git push" *(PUSHES COMMITTED CHANGES TO GITHUB)*

**WHEN CHANGES MADE ON GITHUB READY TO BE DOWNLOADED FROM GITHUB TO PC**

10. enter "git pull" *(PULLS ALL CHANGES DOWN FROM GITHUB TO LOCAL DIRECTORY)*

## WORKFLOW INSTRUCTIONS
1. Before starting work run git pull to get everyone's updated work
2. If conflict occurs, go into conflict file, resolve conflict by updating code to contain correct changes and commit/push again
3. Commit and push everything to *GITHUB* regularly to ensure good history tracking
4. git pull regularly to get other user's changes

## NOTES
(i) If accidentally typed commit without message and entered the commit screen, press **esc** > type **:wq**

What is git:

Git is a version control system which allows you to log different versions of a directory as changes are made, which will allow you to revert to old versions easily if you need to. It's kind of like a bunch of snapshots of the directory at different points in time.

What is github:

Github is kind of like storing stuff on a cloud so everyone will have access to whatever is in it, but you have full control over when things get updated (or pushed) to the site, rather than a standard cloud which might automatically upload changes made to files befor you're ready for that, especially an issue if more than one person are working on the same file for example.

What is a branch:

Branches allow you to fork off of the original/default branch called the master branch. This means you can make changes and commits to the branches without affecting the master one until you are ready, when you are ready you 'merge' the branch with the master.
