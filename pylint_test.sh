cd src
echo 'Generating Pylint report'
pylint * --rcfile=pylintrc -f html > pylint_report.html
mv pylint_report.html ../
cd ..
