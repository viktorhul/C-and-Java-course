JUNIT = lib/junit.jar

TEST_DIR = test

TEST_FILE = AllTests

CLASS_DIR = bin/classes

CP = -cp
D = -d
FLAG = -Xdiags:verbose

all: clean suite

clean: 
	@clear
	@rm -rf $(CLASS_DIR)


compileASTandVisitor: 
	@javac $(CP) $(CLASS_DIR) $(D) $(CLASS_DIR) $(FLAG) src/calculator/ast/*.java src/visitor/*.java src/visitor/exceptions/*.java src/calculator/parser/*.java 

compileParser:
	@javac $(CP) $(CLASS_DIR) $(D) $(CLASS_DIR) src/calculator/parser/*.java

compileCalculator:
	@javac $(CP) $(CLASS_DIR) $(D) $(CLASS_DIR) src/calculator/Calculator.java


compileTest: 
	@javac $(CP) $(CLASS_DIR) $(D) $(CLASS_DIR) $(TEST_DIR)/testTemplates.java
	@javac $(CP) $(JUNIT):$(CLASS_DIR) $(D) $(CLASS_DIR) $(TEST_DIR)/$(TEST_FILE).java

compileAll: compileASTandVisitor compileCalculator compileParser compileTest

test: compileTest
	@java -jar $(JUNIT) $(CP) $(JUNIT):$(CLASS_DIR) -c test.$(TEST_FILE) 


diffTest:
	@java $(CP) $(CLASS_DIR) calculator.Calculator < test/input.txt > test/output.txt 
	diff test/output.txt test/expectedOutput.txt


calculator: compileASTandVisitor compileCalculator compileParser
	@java $(CP) $(CLASS_DIR) calculator.Calculator


.PHONY: clean all runTest test diffTest
