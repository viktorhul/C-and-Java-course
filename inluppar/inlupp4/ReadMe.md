
# How to use symbolic calculator
Make sure to use capitalized first letter when typing commands and mathematical functions, like Sin, If, Else, Quit, Vars, etc.

## Functions
Declare a function by starting the statement with *Function* followed by the function name and zero or more arguments separated by commas. Example given below. 

```
Function name(a, b)
```

## Conditionals
If-Else conditionals are supported and types with capital letters, *If* and *Else*. Operands in the condition block is limited to variables, constants and named constants.

```
If a > b {1} Else {0}
```

## Scope
Write *{expr}* to evaluate an expression in a new scope which does not take into accout variables declared in outer scopes. 

## Build the calculator
Navigate to the directory and type 
```
make calculator
```

# Run tests
Navigate to the directory and type 
```
make test
```

## Run diff tests
Navigate to the directory of the tests. Add to *input.txt* the lines you want to feed the calculator, and to *expectedOutput.txt*, the expected output. Type 

```
make diffTest
```
