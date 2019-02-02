# Overview

The code is for implementation of the research paper:

When Exascale Computing Meets Network Coding: Scalable and Fault-Tolerant Computation for Arbitrary-precision Arithmetic

# Dependencies

[Hadoop 2.9.1](http://hadoop.apache.org/docs/r2.9.1/index.html)

# Usage

Prime number setting is in `CRT.java` file and does not recommend to change.
Code model and fail map number setting is in `config.properties`, specific details can refer to `run.bash`.
`Run.bash` is a script program for testing the code.

### Step 0

Install hadoop 2.9.1 and make sure [this](https://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/SingleCluster.html) can work correctly.

### Step 1

Create a file `config.properties` and write as:

    OPTION=false
    FAILED_NUMBER=0
    
You can also change the settings later.

### Step 2

Compile and run the entire program:

    hadoop com.sun.tools.javac.Main CRTMatrixD.java CRTMapper.java CRTReducer.java CRT.java
    jar cf CRT.jar CRT*.class
    hadoop jar CRT.jar CRT /user/[username]/crt/input /user/[username]/crt/output

### Step 3

The output file can be seen by:

    hadoop fs -cat /user/[username]/crt/output/part-r-00000

Other information can be seen in logs.

# Result

The average time for each mapper and the entire running time of the simulations for different number of failed mappers are compared below.

### Average computation time for mappers.
![Average computation time for mappers](Pic/img1.jpg)

### Entire time of the simulations.
![Entire time of the simulations](Pic/img2.jpg)