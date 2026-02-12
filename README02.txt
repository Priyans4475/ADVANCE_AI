README.txt
==========

Project: Assignment 02
Language: C++

1. Earliest Completion Time
--------------------------------------------------

Given the group size and number of prompts per student per day,
the program determines the earliest time to complete all assignments.

Run using:

    assg02.exe <input_file> <group_size> <prompts_per_day>

Example:

    assg02.exe input.txt 3 5


--------------------------------------------------
2. Minimum Prompts Per Student Per Day
--------------------------------------------------

Given the group size and the number of days,
the program determines the minimum number of prompts per student per day.

Run using:

    assg02.exe <input_file> <group_size> <number_of_days> --findK

Example:

    assg02.exe input.txt 3 7 --findK


--------------------------------------------------
3. 6 AM Exchange Scenario
--------------------------------------------------

In this scenario, solutions of completed assignments are shared
with other students only at 6 AM on the next day.

To enable this rule, add the --exchange flag.

Examples:

    assg02.exe input.txt 3 5 --exc
    assg02.exe input.txt 3 7 --findK --exc

