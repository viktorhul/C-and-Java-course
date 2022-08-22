all:
	# @clear
	# @rm -rf classes #Tim wasnt Here
	@mkdir classes
	@javac -d classes calculator/ast/*.java
	@javac -d classes calculator/parser/*.java
	@javac -d classes calculator/*.java
	# @make run

run:
	@java -cp classes calculator.Calculator

test:
	@javac -d classes calculator/tests/Test.java
	@java -cp classes calculator.tests.Test

clean:
	@rm -rf classes
