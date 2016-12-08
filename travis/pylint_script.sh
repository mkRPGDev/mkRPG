## Ce script a pour but d'utiliser pylint sur notre code python
cd src
echo 'Generating Pylint report'
pylint *
mv pylint_global.html ../
rm *.html
cd ..
