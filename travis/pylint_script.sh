cd src
echo 'Generating Pylint report'
pylint *
mv pylint_global.html ../
rm *.html
cd ..
