# OSStats and Piping

Goals:


getstats.c prints out interesting statistics
about the environment in which it is run, or statistics about a single
process running in that environment.

gopipe.c  executes up to four shell instructions, piping the output of one to the input of the   next.


Part 1: getstats.c

Every Unix filesystem includes a /proc directory.  This directory
contains a great deal of information, as text files, that relate to
the properties of the operating system – its memory usage, its kernel
version number, scheduling and filesystem information, and so on.
Details about processes running on the system are also available in
subdirectories of the /proc directory.

If run without arguments, the program should print to the console
the following information, in order (one item per line):

The model name of the CPU
The number of cores
The version of Linux running in the environment
The total memory available to the system, in kilobytes
The 'uptime' (the amount of time the system has been running, expressed in terms of days, hours, minutes and seconds).


If run with a numerical argument, the program should print to the
console information about the corresponding process:
The name of the process
The console command that started the process (if any)
The number of threads running in the process
The total number of context switches that have occurred during the running of the process.

For example, if I execute the following command on jupyterhub:
./getstats
my output is:

model name      : Intel(R) Xeon(R) CPU E5-2640 v2 @ 2.00GHz
cpu cores       : 8
Linux version 4.15.0-169-generic (buildd@lcy02-amd64-015) (gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)) #177-Ubuntu SMP Thu Feb 3 10:50:38 UTC 2022
MemTotal:       131942328 kB
Uptime: 73 days, 2 hours, 46 minutes, 37 seconds

./getstats 1
my output is:

Process number: 1
Name:   tini
Filename (if any): tini
Threads:        1
Total context switches: 314

./getstats -1
My output is:

Process number -1 not found.


Note that your solution must work correctly for process numbers other
that 1 and -1.

Hints


All the information you need is in the /proc directory or one of
its subfolders, are in contained in ordinary text files.
Remember to use fopen() and fclose() properly.


You may need to read strings from these files and extract
information from them in order to produce some of the statistics
you need.  The libraries that have been included into the starter
getstats.c file may help you with this.


Part 2: gopipe.c

Unix commands that are executed in a shell can take advantage of
'piping' – that is, the output of one command can be used as the input
of another.  For example, the command ls -1 will list the contents
of the current directory, one entry per line.  The command wc -l
counts the number of lines in the input, and outputs that number as an
integer.  We can specify that the output of ls -1 should be used as
the input of wc -l by joining the two commands with a pipe:
ls -1 | wc -l
The resulting compound command prints the number of entries in the
current directory.

gopipe.c  accepts up to four individual
commands, one per line (pressing enter or return at the end of each).
The user may indicate that they have no more commands to enter by
pressing return on a line without entering a command first.  If this
happens, or if the user enters four commands, your program must then
execute each of the four instructions, transferring the output of one
to the input of the next.  The output of the final command in the
sequence must be sent to the console.  For example, if the program is
run as follows:
./gopipe
and then the following is entered (where the ⏎ symbol indicates
pressing enter):
/bin/ls -1 ⏎
/bin/wc -l ⏎
⏎
The program should print out the number of directory entries in the
current directory (as an integer) and then quit.
The program should only execute as many instructions as were entered
(a maximum of four) – if the enter key is pressed without first
entering a command, the program should simply quit.  If the user
enters four commands, the program should not prompt for a fifth, but
should immediately begin executing the commands.
Test your program with inputs of various lengths – chains of zero,
one, two, three or four commands.  The console output in your program
should be identical to what you get at the shell prompt if you enter
the same commands separated by pipes.  For example, the shell command:
gcc --help | grep dump | tr '[:lower:]' '[:upper:]' | sort 
prints all three lines from the gcc help that contain the word 'dump',
converted to uppercase and sorted alphabetically:

  -DUMPMACHINE             DISPLAY THE COMPILER'S TARGET PROCESSOR.
  -DUMPSPECS               DISPLAY ALL OF THE BUILT IN SPEC STRINGS.
  -DUMPVERSION             DISPLAY THE VERSION OF THE COMPILER.
