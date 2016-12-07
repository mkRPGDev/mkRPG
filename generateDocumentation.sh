#!/bin/sh

################################################################################
# Required global variables:
# - TRAVIS_BUILD_NUMBER : The number of the current build.
# - TRAVIS_COMMIT       : The commit that the current build is testing.
# - GH_REPO_TOKEN       : Secure token to the github repository.
################################################################################

################################################################################
##### Setup this script and get the current gh-pages branch.               #####
echo 'Setting up the script...'
# Exit with nonzero exit code if anything fails
set -e

# Create a clean working directory for this script.
mkdir code_docs
cd code_docs

##### Get the current master branch
echo "Get a clone of master"

git clone -b master "https://github.com/mkRPGDev/mkRPG.git"
cd mkRPG

##### Configure git.
# Set the push default to simple i.e. push only the current branch.
git config --global push.default simple
# Pretend to be an user called Travis CI.
git config user.name "Travis CI"
git config user.email "travis@travis-ci.org"


################################################################################
##### Generate the Pylint report ###############################################
cd src
echo 'Generating Pylint report'
pylint *
mv pylint_global.html ../
rm *.html
cd ..

################################################################################
##### Generate the Doxygen code documentation and log the output.          #####
echo 'Generating Doxygen code documentation...'
doxygen Doxyfile_c
doxygen Doxyfile_py

#On le renomme pour préparer au changement de branche
mv "doc_c" "doc_c_new"
mv "doc_py" "doc_py_new"
mv "pylint_report.html" "pylint_report_new.html"
#On change de branche si on est dans master
git checkout gh-pages
#On écrase l'ancienne documentation
rm pylint_report.html
rm -r doc_c doc_py
mv "doc_c_new" "doc_c"
mv "doc_py_new" "doc_py"
mv "pylint_report_new.html" "pylint_report.html"


#On compile le tex
#echo 'Compiling tex documentation ... '
#cd doc/latex
#make
#cd ..



################################################################################
##### Upload the documentation to the gh-pages branch of the repository.   #####

echo 'Uploading documentation to the gh-pages branch...'
# Add everything in this directory (the Doxygen code documentation) to the
# gh-pages branch.
# GitHub is smart enough to know which files have changed and which files have
# stayed the same and will only update the changed files.
git add --all
echo "Status : git add has passed"

# Commit the added files with a title and description containing the Travis CI
# build number and the GitHub commit reference that issued this build.
git commit -m "Deploy code docs to GitHub Pages Travis build: ${TRAVIS_BUILD_NUMBER}" -m "Commit: ${TRAVIS_COMMIT}"
echo "Status : git commit has passed"

# Force push to the remote gh-pages branch.
git push -v --force "https://${GH_REPO_TOKEN}@github.com/mkRPGDev/mkRPG.git"
echo "Status : git push has passed"
