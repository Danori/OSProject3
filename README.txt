Simply extract the contents to whatever directory, navigate to the directory,
and run the command:

make

This will compile the reader / writer concurrency program into an executable
named rwmain. Ensure either the given scenarios.txt, or one you have created,
is in the same directory as the executable. The format of scenarios.txt is
as follows:

- Each line may contain up to 128 'R' or 'W' characters.
- The order of the R's and W's determines the order the threads will be
  created, from left to right.
- The file may contain however many thread scenarios you wish to run.

Below is an example format of a scenarios.txt:

WWWWWWWWWWWWWWW
RRRRRRRRRRRRRRR
WRRRRRRRRRR
RRRRRRRWRRRRRRR
RRRRRRRRRRRRRRW
RRRRWRRWRRRWR
RRWRWRWRRRRRWRR
RRRRWRRRWRRRRWW
WRRRRR
WWWWWRRRRRRRRRR

Run the program with:

./rwmain