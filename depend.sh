#!/bin/sh
# Inspiration from Peter Miller's "Recursive Make Considered Harmful", 1997, AUUGN Journal of AUUG Inc., 19(1), pp. 14-25
# Example runtime:
#   $ ./depend.sh DIR_TO_SOURCE $(GCC_ARGUMENT)
#   $ ./depend.sh test -Isrc -Itest test/unit1.cpp

DIR="$1"
shift 1
case "$DIR" in 
"" | ".")
	#echo "Case 1: No directory or thec current working directory."
	# Passes all remaining arguments to gcc, and then parses with sed.
	gcc -MM -MG "$@" | sed -e ’s@ˆ\(.*\)\.o:@\1.d \1.o:@’
	;;
*)
	#echo "Case 2: Any other directory"
	# Passes all remaining arguments to gcc, and then parses with sed.
	gcc -MM -MG "$@" | sed -e "s@ˆ\(.*\)\.o:@$DIR/\1.d $DIR/\1.o:@" | sed "s:^:$DIR/:"
	;;
esac
