Git global setup:

        git config --global user.name "Chunmei"

        git config --global user.email "chunmei.xia@eisti.fr"


Create a new repository:

        git clone git@gitlab.etude.eisti.fr:system_parallele/APP_scheduler.git

        cd APP_scheduler

        touch README.md

        git add README.md

        git commit -m "add README"

        git push -u origin master

Existing folder or Git repository:

        cd existing_folder

        git init

        git remote add origin git@gitlab.etude.eisti.fr:system_parallele/APP_scheduler.git

        git add .

        git commit
 
        git push -u origin master