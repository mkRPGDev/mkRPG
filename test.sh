#!/bin/bash
branch_name="$( git branch -a --no-color  | grep -E '^\*' | sed 's/\*[^a-z]*//g')"
  echo ${branch_name}
if [ ${branch_name} = "travis" ]; then
  echo ${branch_name}
fi
