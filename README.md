# ERPLAG-Compiler
The language is designed for the course CS F363 and the name of the language is motivated by the drive to ERadicate PLAGiarism. The language has been evolving to provide a minimal feature set for the project in Compiler Construction course under the instructor [Dr. Vandana Agarwal](https://universe.bits-pilani.ac.in/pilani/vandana/profile). 

### Group Members:

`THE MATH_CS boiz`

1. [Rajan Sahu](https://github.com/RAJAN13-blip)(2019B4A70572P)
2. [Ayush Agarwal](https://github.com/Ayush00700)(2019B4A70652P)
3. [Vasu Swaroop](https://github.com/Vhaatever)(2019B4A70638P)
4. [Yash Goyal](https://github.com/YasHGoyaL27)(2019B4A70638P)
5. [A Sudarshan](https://github.com/ASudu)(2019B4A70638P)

### Group Number: 20

### Setup / Usage

`Requirements`:

**LINUX/macOS**

- GCC version 9.4.0 
- UBUNTU LTS 20.04.4.

**WINDOWS**

- MinGW

# Clone the repository (clone GPLAN separately in server if it doesn't work!)

```
git clone https://github.com/Ayush00700/ERPLAG-Compiler.git 
```

# Check GCC Version and Ubuntu Version/ dependencies for react frontend

compatible to GCC version 9.4.0  on Ubuntu 20.04.4.

Check gcc version

```
gcc --version
```

If not installed, install

```
sudo apt-get install gcc:9.4.0
```


### Compilation

Compile code using 
```
make
```

### Execution

Run the application: 
```
$./stage1exe testcase.txt  parsetreeOutFile.txt  size_of_buffer
```

### File Structure

```
|-- lexer.c
|-- parser.c
|-- driver.c
|-- main.c
|-- Makefile
|-- lexerDef.h
|-- lexer.h
|-- parserDef.h
|-- parser.h
|-- tokeninfo.h
|-- setADT.c
|-- hashtable.c
|-- documents          # Important Documents
|   |-- grammar_analysis.pdf       # Explaining Grammar with FIRST and FOLLOW sets.
|   |-- Language Specifications.pdf     # Complete Languauge Specifications.
|   |-- CS_F363.pdf     # Handout of Course.
|   |-- Compiler Master Document.docs     # Master doc for reference of checkpoints and doubts
|-- images             # Frontend Reactjs Application
|   |-- dfa_front.png   # DFA front page
|   |-- dfa_back.png  # DFA back page
|   |-- grammar.pdf       # Contains grammar rules, First Sets, Follow Sets
|-- testcases          # Important Documents
|   |-- code_test_case1.txt       # Test case 1 (without errors).
|   |-- code_test_case2.txt       # Test case 2 (without errors).
|   |-- code_test_case3.txt       # Test case 3 (without errors).
|   |-- code_test_case4.txt       # Test case 4 (without errors).
|   |-- code_test_case5.txt       # Test case 5 (without errors).
|   |-- code_test_case6.txt       # Test case 6 (without errors).
|-- grammar.txt         #Complete grammar rules in specific format
|-- .gitignore      #For Github, ignoring independent files.
```

### Functionalities Implemented

(a) FIRST and FOLLOW set have been automated

(b)Both lexical and syntax analysis modules implemented

(c) Module works for all test cases

(d) Module handles errors efficiently and gives correct Parse Tree

(e) No Segmentation Fault

(f)`INORDER TRAVERSAL FOR N-ary TREE`
    Leftmost child ‐‐> parent node‐‐> remaining siblings (excluding the leftmost child)

### Detailed Description of FILES

1. File `lexer.c` : This file contains following functions
```
`FILE *getStream(FILE *fp)`: This function takes the input from the file pointed to by file pointer 'fp'. This file is the source code written in the given language. The function uses efficient technique to bring the fixed sized piece of source code into the memory for processing so as to avoid intensive I/O operations mixed with CPU intensive tasks. 

The function also maintains the file pointer after every access so that it can get more data into the memory on demand. The implementation can also be combined with getNextToken() implementation as per the convenience of the team.

`getNextToken(  )`: This function reads the input character stream and uses efficient mechanism to recognize lexemes. The function tokenizes the lexeme appropriately and returns all relevant information it collects in this phase (lexical analysis phase) encapsulated as tokenInfo and passes to the parser on demand. The function also displays all lexical errors appropriately.

`removeComments(char *testcaseFile, char *cleanFile)`: This function is an additional plug in to clean the source code by removal of comments. The function takes as input the source code and writes the clean code in the file appropriately. Ensure that the line numbers of code in the cleanFile are same as original line numbers of the same code in testcaseFile. [Note: The function is invoked only once through your driver file to showcase the comment removal. However, your lexer does not really pick inputs from comment removed file. For showcasing your lexer's ability, directly take input from user source code]
```
2. File parser.c : This file contains following functions
```
`ComputeFirstAndFollowSets (grammar G, FirstAndFollow F)`: This function takes as input the grammar G, computes FIRST and FOLLOW information and populates the appropriate data structure F. First and Follow set automation must be attempted, keeping in view the programming confidence of the team members and the available time with the teams. The credit for the above is only 4 marks out of 45 marks reserved for stage 1 module. If teams opt not to develop the module for computation of First and follow sets, the same can be computed manually and information be populated in the data structure appropriately. However, all members of the team must understand that any new grammar rule for any new construct will then require their expertise in computing FIRST and FOLLOW sets manually (especially during online exam). Note: While First and Follow computation from grammar can be skipped at the cost of 4 marks, and data structure F can be populated manually, it is yet mandatory to populate the parse table automatically using the following function.

`createParseTable(FirstAndFollow F, table T)`: This function takes as input the FIRST and FOLLOW information in F to populate the table T appropriately. 

`parseInputSourceCode(char *testcaseFile, table T)`: This function takes as input the source code file and parses using the rules as per the predictive parse table T and returns a parse tree. The function gets the tokens using lexical analysis interface and establishes the syntactic structure of the input source code using rules in T. The function must report all errors appropriately (with line numbers) if the source code is syntactically incorrect. If the source code is correct then the token and all its relevant information is added to the parse tree. The start symbol of the grammar is the root of the parse tree and the tree grows as the syntax analysis moves in top down way.

The function must display a message "Input source code is syntactically correct..........." for successful parsing. 

`printParseTree(parseTree PT, char *outfile)`: This function provides an interface for observing the correctness of the creation of parse tree. The function prints the parse tree in inorder in the file outfile.

The output is such that each line of the file outfile must contain the information corresponding to the currently visited node of the parse tree in the following format

lexeme CurrentNode lineno tokenName valueIfNumber  parentNodeSymbol isLeafNode(yes/no) NodeSymbol

The lexeme of the current node is printed when it is the leaf node else a dummy string of characters "‐‐‐‐" is printed. The line number is one of the information collected by the lexical analyzer during single pass of the source code. The token name corresponding to the current node is printed third. If the lexeme is an integer or real number, then its value computed by the lexical analyzer should be printed at the fourth place. Print the grammar symbol (non-terminal symbol) of the parent node of the currently visited node appropriately at fifth place (for the root node print ROOT for parent symbol) . The sixth column is for printing yes or no appropriately. Print the non-terminal symbol of the node being currently visited at the 7th place, if the node is not the leaf node [Print the actual non-terminal symbol and not the enumerated values for the non-terminal]. Ensure appropriate justification so that the columns appear neat and straight. 
```

3. `driver.c` : As usual, drives the flow of execution to solve the given problem. Take the input file name and buffer size at command line.

Press option for the defined task (Use a while loop to receive option choices till option 0 is pressed. Ensure independence of working of all options e.g. if option 3 is pressed, option 2 is not needed) 
```
0 : For exit 

1 : For removal of comments ‐ print the comment free code on the console (Ensure that the line numbers of original code are preserved) 

2 : For printing the token list (on the console) generated by the lexer. This option performs lexical analysis and prints all tokens and lexemes line number wise. Here, the tokens are not passed to the parser, but printed on the console only. Each token appears in a new line along with the corresponding lexeme and line number. (invoke only lexer)

The format for printing each token is as follows.

 Line_number          lexeme           Token_name 

Also print the lexical errors with lexemes and line numbers appropriately.

3 : For parsing to verify the syntactic correctness of the input source code and printing the parse tree appropriately. This option prints all errors - lexical and syntactic, line number wise, on the console and prints parse tree in the file as mentioned in the command line below. (Invoke both lexer and parser) . 

4: For printing (on the console) the total time taken by your stage 1 code of lexer and parser to verify the syntactic correctness. Use <time.h> file as follows
```

4. `lexerDef.h` : Contains all data definitions used in lexer.c

5. `lexer.h` : Contains function prototype declarations of functions in lexer.c

6. `parserDef.h` : Contains all definitions for data types such as grammar, table, parseTree etc. used in parser.c

7. `parser.h` : Contains function prototype declarations of functions in parser.c

8. `makefile` : This file uses GNU make utility, which determines automatically which pieces of a large program need to be recompiled, and issues the commands to recompile them. The correctness of your make file depends on file dependencies used correctly.

### More Organized Useful Links and Tips
- Read about MakeFILE
    - MAKEFILE Docs: http://www.gnu.org/software/make/manual/make.html
- Compiler Master Document
    - Group 20 Master Document for Checkpoints and doubts. (if docs useless check source code): https://docs.google.com/document/d/1oK50FhVqL7UVcHQ9-tFCd5QYU0-ufKB7BWihiRdb2jE/edit?usp=sharing

### Known Problems and Future Scopes


### Doubts



### Left to Implement

- Stage 1 Submission
    - Error Handling in Parser.
    - Documentation.
    - Code Cleaning.

- Stage 2 Submission
    - Semantic Analysis
