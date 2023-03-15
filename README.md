# schduling_problem_using_SAT

## How to compile
(1) cd sat/test
    
(2) Simply type 
```
make
```
in the Linux terminal.

The  .o files and the binary would be created.

## How to run
Create a ```data.txt``` and a ```output.txt``` in the ```test``` folder

then type 
```
./satTest ./data.txt ./output.txt
```

the data in the ```data.txt``` must match the format below 
(1) the first line is the number of interviewers

(2) the second line is the number of interviewees

(3) the next n lines is the free time for interviewer in work days (index 0 is Monday, index 1 is Tuesday ...), (n is the number of interviewers), (0 represents the time is not avalible, otherwise is 1)

(4) the next m lines is the free time for interviewee in work days (index 0 is Monday, index 1 is Tuesday ... and index 5,6,7 is the groups which interviewee will come to interview), (m is the number of interviewees), (for the first 5 terms, 0 represents the time is not avalible, otherwise is 1)

if it is satisfiable, the output will save in ```output.txt```

## How to generate a random schedule
you can use the random generation program in folder ```my_gen_data```

(1) type ```make``` in folder ```my_gen_data```

(2) create a blank ```data.txt``` in folder ```test```

(3) in folder ```my_gen_data``` type
```
./gen ../test/data.txt
```
then input the number of groups and number of interviewees
