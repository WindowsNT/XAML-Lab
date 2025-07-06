git checkout --orphan newBranch
git add -A  
git commit -m "Update"
git branch -D master  
git branch -m master  
git push -f origin master  
git gc --aggressive --prune=all 
git push --set-upstream origin master
 