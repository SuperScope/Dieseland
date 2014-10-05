Dieseland Coliseum
==============

Unreal Project for UCF Game Workshop Fall 2014


Things to Remember
--------------
* Always create a branch when working on a feature, never work from the master branch
* Be as descriptive as possible in commit comments
* Always submit pull requests, do not merge your own branch with the master branch without approval
* You must rebuild and compile the project with every major branch sync/download. generating new VS or Xcode projects may also be required
* Do not edit the .gitignore file, if changes need to be made, inform the Technical Director

Getting Started
--------------
1. Install the GitHub client for [Windows] (https://windows.github.com/) or [Mac] (https://mac.github.com/)
2. Install Unreal 4
3. Open the GitHub client
4. Clone this repository from "SuperScope" (Right click to "Clone to..." a different directory)
5. Navigate to the directory on your computer where it was cloned
6. Proceed to the correct Compiling and Building section

Official Documentation for Compiling
--------------
* Below is just an outline specific to this project, I recommend referring to the official Unreal walkthrough [here] (https://docs.unrealengine.com/latest/INT/Programming/Development/CompilingProjects/index.html)

Compiling and Building (Windows)
--------------
1. Install [Microsoft Visual Studio 2013] (http://www.microsoft.com/en-us/download/details.aspx?id=43729)
2. Right click on the .uproject file in the main folder
3. Select "Generate visual studio project files"
4. Open the generated .sln file in Visual Studio 2013
5. Wait for Visual Studio to generate required files
6. Ensure that your solution configuration is Development - Windows
7. Hit F7 or right click on "Dieseland" in the Solution Explorer and click "Build"
8. Wait for building to finish
9. You can now open the .uproject file in the Unreal Editor

Compiling and Building (Mac)
--------------
1. Install [Xcode 5] (https://developer.apple.com/xcode/downloads/)
2. Right click on the .uproject file in the main folder
3. Select "Generate Xcode project file"
4. Open the generated .xcodeproj file in Xcode 5
5. Wait for the project to open
6. On the top toolbar, ensure that DieselandEditor - Mac > My Mac 64 bit is selected as the active scheme
7. Navigate to Project > Build For > Profiling
8. Wait for building to finish
9. You can now open the .uproject file in the Unreal Editor

Branching
--------------
1. For every issue or trello card, create a new branch
2. On Mac, navigate to the branches tab. On Windows, click the branches tab on the top right
3. Create a new branch off of master and name it appropriately
4. Ensure that you have switched your local files to the new branch
5. Do work. (See Committing)
6. Submit a pull request to merge your project with the main project (see Pull Requests)

Deleting Branches
--------------
* Note - this will delete the branch from your local as well as the server, and as such should only be done once the server branch has been merged

1. Server branches will beleted by the lead technical director
2. When a server branch is deleted, it will remain listed on your local machine
3. To delete this branch, click on the branches icon IN YOUR DESKTOP CLIENT
4. Click delete branch


Committing
--------------
1. Do work
2. Open up the GitHub client
3. Open up the correct repository listed on your local machine
4. Ensure the listed changes are correct
5. Ensure that you are on the correct branch
6. In the commit box, type a brief title of your changes
7. (Optional) Type a more detailed description in the comment box
8. Commit the changes
9. Hit the Sync button

Pull Requests
--------------
1. Navigate to the team's repository on github
2. Click on the Pull Requests tab
3. Click the "New pull request" button
4. Ensure the base branch is set to "master"
5. Set the compare branch to the branch you've been working on
6. Review the changes for accuracy
7. Click "Create Pull Request"
8. Leave a title and comment describing the work done
9. Click "Send pull request"

