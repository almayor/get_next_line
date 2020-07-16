#!/bin/sh
# This program is needed primarily because the Norme
# (the code standard @ School 42) restricts the comment format to the one that
# isn't recognised by Doxygen. Hence, a pre-processing with perl is needed.
# This script also performs post-doxygen clean-up.

command -v doxygen >/dev/null
if [ $? -ne 0 ]; then
	echo "Please install Doxygen!"
else
	sed -i '1s#^#///\\file\n#' get_next_line.c
	sed -i '1s#^#///\\file\n#' get_next_line.h
	perl -i -pe 's#^\/\*$#/\*\*#' get_next_line.c
	perl -i -pe 's#^\/\*$#/\*\*#' get_next_line.h
	doxygen docs/.doxygen/Doxyfile
	sed -i '1d' get_next_line.c
	sed -i '1d' get_next_line.h
	perl -i -pe 's#^\/\*\*$#/\*#' get_next_line.c
	perl -i -pe 's#^\/\*\*$#/\*#' get_next_line.h
fi
