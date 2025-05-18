<div align="center">
  <h1>M127 - Parallel Computing Systems</h1>
</div>
<div align="center">
  <h2>Pthreads - OpenMP</h2>
</div>
<div align="center">
  <h2>Spring Semester 2024–2025</h2>
</div>

[Panagiota Gyftou - cs22400025](https://github.com/panagiotagyft) </br>
gyftoupan@di.uoa.gr </br>

## Project Structure

<pre><code>
📦exercise_1/ 
├── 📂include/ 
│ └── *.h   # Header files
├── 📂modules/ 
│ └── *.c   # Source code files 
├── 📂src/ 
│ ├── main.c 
│ └── 📄Makefile
├── 📄Makefile
📦exercise_2/ 
📦exercise_3/ 
📦exercise_4/ 
📦exercise_5/ 
📦exercise_6/ 
📦exercise_7/ 
📦exercise_8/ 
📦exercise_9/
📄README.md
📄Report_7115112400025.pdf </code></pre>
## Introduction - Problem

This helper script automates building and running every exercise in the Parallel Computing Systems handout.
It will compile the serial, Pthreads and OpenMP versions, execute each benchmark with your chosen parameters, and collect timing and output data.


# Test the program utility

Run the following script to execute all the exercises!

**Also check for individual [program runs](#run-examples) and [ memory leaks](#valgrind)**

## Compile & Run

### 1. Compile the Program

To compile the program, use the following command:

```bash
# Compile the main program 
make 
```

### 2. Run the Program

If you [compiled](###-1.-Compile-the-Program) the files from the previous section you can run:</br>
`$ ./my_program *ARGS`

\*[ARGS](#args)

You can also compile and run through the Makefile:

- `run` to compile and run the main program and tests
- `run` to compile and run only the main program
- `run-tests` to compile and run only the tests

Here is the command to compile & run:

```bash
# Compile and run the main program and tests
make run ARGS="*ARGS"
```

\*[ARGS](#args)

**`Don't forget to give the arguments through the ARGS variable!`**
</br>
</br>
Visit the Makefile files to get more info about the commands available (valgrind, run etc)

#### Run Examples:

```bash
# exercise 1
make run ARGS="-d 100000000 -t 4"   # -d(darts) -t(threads)

# exercise 2
make run ARGS="-i 100000000 -t 4"   # -i(iterations) -t(threads)

# exercise 3
make run ARGS="-i 100000000 -t 4"   # -i(iterations) -t(threads)

# exercise 4
make run ARGS="-i 1000000 -t 4"   # -i(iterations) -t(threads)

# exercise 5
make run ARGS="4 10000 10000"        # Default execution (threads) (m) (n)
make run ARGS="4 10000 10000 full"   # (threads) (m) (n) full
make run ARGS="4 10000 10000 upper"  # (threads) (m) (n) upper
### Notes on Exercise 5
- If a **fifth argument** is provided:
  - `"full"` disables the upper-triangular optimization and performs full matrix multiplication.
  - `"upper"` enables optimization by multiplying only the upper triangular part of the matrix.

# exercise 6
## -- serial --
make run ARGS="-g 1000 -gs 64 -impl 0 -t 1"   # -g(generations) -gs(grid size)  -impl(serial/parallel) -t(threads)

## -- parallel --
make run ARGS="-g 1000 -gs 64 -impl 1 -t 3" 

# exercise 7
## -- serial - row_wise --
make run ARGS="-d 10000 -sp 0 -rc 0 -t 1"  #-d(dimension) -sp(serial/parallel) -rc(row/column wise) -t(threads)

## -- serial - column_wise --
make run ARGS="-d 10000 -sp 0 -rc 1 -t 1"  #-d(dimension) -sp(serial/parallel) -rc(row/column wise) -t(threads)

## -- parallel - column_wise --
make run ARGS="-d 10000 -sp 1 -rc 1 -t 2"  #-d(dimension) -sp(serial/parallel) -rc(row/column wise) -t(threads)


# exercise 8
## -- serial --
make run ARGS="-s 1000 -sp 64 -impl 0 -t 1"   # -s(size) -sp(serial/parallel) -t(threads)

## -- parallel --
make run ARGS="-s 1000 -sp 64 -impl 1 -t 4" 


# exercise 9
## -- serial --
make run ARGS="-g 1000 -gs 64 -impl 0 -t 1"   # -g(generations) -gs(grid size)  -impl(serial/parallel) -t(threads)

## -- parallel --
make run ARGS="-g 1000 -gs 64 -impl 1 -t 3" 

```
