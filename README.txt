You have an empty repository
To get started you will need to run these commands in your terminal.
New to Git? Learn the basic Git commands
Configure Git for the first time
git config --global user.name "[USER NAME]"
git config --global user.email "[Bosch email]"
Working with your repository
I just want to clone this repository
If you want to simply clone this empty repository then run this command in your terminal.
git clone https://[NT-USER]@sourcecode.socialcoding.bosch.com/scm/~lud1ga/boschdayxdkstartkit.git
My code is ready to be pushed
If you already have code ready to be pushed to this repository then run this in your terminal.
cd existing-project
git init
git add --all
git commit -m "Initial Commit"
git remote add origin https://[NT-USER]@sourcecode.socialcoding.bosch.com/scm/~lud1ga/boschdayxdkstartkit.git
git push -u origin master
My code is already tracked by Git
If your code is already tracked by Git then set this repository as your "origin" to push to.
cd existing-project
git remote set-url origin https://[NT-USER]@sourcecode.socialcoding.bosch.com/scm/~lud1ga/boschdayxdkstartkit.git
git push -u origin --all
git push origin --tags
