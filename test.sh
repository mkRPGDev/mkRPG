##### Setup this script and get the current gh-pages branch.               #####
echo 'Setting up the script...'
# Exit with nonzero exit code if anything fails
set -e

branch_name="$( git branch -a --no-color  | awk '/^\*/{getline; print}' | sed 's/\*[^a-z]*//g')"
echo $branch_name
