#!/bin/bash
echo "hi there im now resetting your master branch to our master state"
echo "enter the remotename of vivis repository (may be vivigit or similar)"
read remote

echo "first of all we save the work"
git stash > /dev/null
echo now we switch to your master branch
git switch master
echo now we are resetting the master branch to the newest state of vivis master branch
git fetch $remote master > /dev/null
git reset --hard $remote/master
echo "Done if you had work that was stashed you can apply it with
'git stash apply 0'
or you can pop it with
'git stash pop 0'"
