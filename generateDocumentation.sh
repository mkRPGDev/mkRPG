#!/bin/sh
################################################################################
# Title         : generateDocumentationAndDeploy.sh
# Date created  : 2016/02/22
# Notes         :
__AUTHOR__="Jeroen de Bruijn"
# Preconditions:
# - Packages doxygen doxygen-doc doxygen-latex doxygen-gui graphviz
#   must be installed.
# - Doxygen configuration file must have the destination directory empty and
#   source code directory with a $(TRAVIS_BUILD_DIR) prefix.
# - An gh-pages branch should already exist. See below for mor info on hoe to
#   create a gh-pages branch.
#
# Required global variables:
# - TRAVIS_BUILD_NUMBER : The number of the current build.
# - TRAVIS_COMMIT       : The commit that the current build is testing.
# - GH_REPO_NAME        : The name of the repository.
# - GH_REPO_REF         : The GitHub reference to the repository.
# - GH_REPO_TOKEN       : Secure token to the github repository.
################################################################################

################################################################################
##### Setup this script and get the current gh-pages branch.               #####
echo 'Setting up the script...'
# Exit with nonzero exit code if anything fails
set -e


# On recupère le nom de la branche dans laquelle on se trouve
branch_name="$( git branch -a --no-color  | awk '/^\*/{getline; print}' | sed 's/\*[^a-z]*//g')"
echo $branch_name

# Create a clean working directory for this script.
mkdir code_docs
cd code_docs

# Get the current master branch
echo "Get a clone of $branch_name"

git clone -b $branch_name "https://${GH_REPO_REF}"
cd $GH_REPO_NAME

##### Configure git.
# Set the push default to simple i.e. push only the current branch.
git config --global push.default simple
# Pretend to be an user called Travis CI.
git config user.name "Travis CI"
git config user.email "travis@travis-ci.org"



################################################################################
##### Generate the Doxygen code documentation and log the output.          #####
echo 'Generating Doxygen code documentation...'
doxygen Doxyfile

#On le renomme pour préparer au changement de branche
mv "doc" "doc_new"
#On change de branche si on est dans master
git checkout gh-pages
#On écrase l'ancienne documentation
rm -r doc
mv "doc_new" "doc"


#On compile le tex
#echo 'Compiling tex documentation ... '
#cd doc/latex
#make
#cd ..


################################################################################
##### Upload the documentation to the gh-pages branch of the repository.   #####
# Only upload if Doxygen successfully created the documentation.
# Check this by verifying that the html directory and the file html/index.html
# both exist. This is a good indication that Doxygen did it's work.


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
# The ouput is redirected to /dev/null to hide any sensitive credential data
# that might otherwise be exposed.

git push -v --force "https://${GH_REPO_TOKEN}@${GH_REPO_REF}"
echo "Status : git push has passed"
