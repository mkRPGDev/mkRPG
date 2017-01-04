## Ce script génère la documentation du C et du Python à l'aide des Doxyfile
echo 'Generating Doxygen code documentation...'
doxygen travis/Doxyfile_c
doxygen travis/Doxyfile_py
