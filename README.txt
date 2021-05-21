Info:
Code by Geir Anderson
Help received from Alex, Jared, Allie
Used the man pages
and size for buffer in strings idea from (found in format date and readable size):
https://www.geeksforgeeks.org/how-to-find-size-of-array-in-cc-without-using-sizeof-operator/

Notes:

Makefile was altered to include dcurses.

All functions appear to work almost as instructed.
    Recursion seems to break during cases of directories inside of directories.

Works for viewing the current directory

Compile:

make

Running:

Default printing of the current directory with no sorting or filter

./feeble

./feeble -f file_name       (this does show another directory but all data is wrong, except for the file names)

./feeble -p prefix_here     (filter for a prefix)

./feeble -s suffix_here     (filter for a suffix)

./feeble -m                 (sort by size)

./feeble -d                 (sort by date)

./feeble -r                 (recursive size view) *CAUTION! CAN SEG FAULT ON MULTI DIR SITUATIONS!!!*

-f -s and -p require an input after each one
-m and -d do not
-m and -d are incompatible and the last one to be typed will be used
any combination can be used, as long as inputs are given where needed
