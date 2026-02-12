README03.txt
============

Project: Assignment 03
Language: C++


--------------------------------------------------
1. COMPILATION
--------------------------------------------------

Open terminal inside the project folder.

Compile the program using:

    g++ assg03.cpp -o assg03.exe

If you are using Linux / WSL:

    g++ assg03.cpp -o assg03


--------------------------------------------------
2. PROGRAM INPUT FORMAT
--------------------------------------------------

The program expects the following command line arguments:

    assg03.exe <input_file> <N> <CGPT_prompts> <GEM_prompts> <c1> <c2> [--caseA]

Where:

    input_file   = assignment input file
    N            = number of students in group
    CGPT_prompts = total ChatGPT prompts per day (group subscription)
    GEM_prompts  = total Gemini prompts per day (group subscription)
    c1           = cost per ChatGPT prompt
    c2           = cost per Gemini prompt
    --caseA      = optional flag (one assignment per student per day)

If --caseA is NOT provided, program runs Case-B automatically.


--------------------------------------------------
3. RUN INSTRUCTIONS (WINDOWS / VS CODE TERMINAL)
--------------------------------------------------

Case-B (multiple assignments allowed):

    .\assg03.exe input.txt 3 10 8 2 3

Case-A (one assignment per student):

    .\assg03.exe input.txt 3 10 8 2 3 --caseA


--------------------------------------------------
4. RUN INSTRUCTIONS (LINUX / WSL)
--------------------------------------------------

Case-B:

    ./assg03 input.txt 3 10 8 2 3

Case-A:

    ./assg03 input.txt 3 10 8 2 3 --caseA


--------------------------------------------------
5. OUTPUT
--------------------------------------------------

Program prints:

    DFS Days and Nodes explored
    DFBB Days and Nodes explored
    A* Days and Nodes explored


--------------------------------------------------
6. NOTES
--------------------------------------------------

Even indexed assignments -> ChatGPT
Odd indexed assignments  -> Gemini

Input file format is same as Assignment-01.
Lines starting with '%' are ignored.
