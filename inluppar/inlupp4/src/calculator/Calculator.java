package calculator;

import calculator.ast.*;
import calculator.parser.*;
import java.io.IOException;
import java.util.Scanner;
import java.util.Map;
import java.util.*;

import visitor.*;
import visitor.exceptions.*;

public class Calculator {
  public static void main(String[] args) {

    int enteredExpressions = 0;
    int unsuccessfullyEvaluated = 0;
    int fullyEvaluated = 0;
    String prompt = ">> ";

    final CalculatorParser parser = new CalculatorParser();

    final Environment env = new Environment();
    final HashMap<String, FunctionDefinition> functions = new HashMap<String, FunctionDefinition>();

    final EvaluationVisitor visitor = new EvaluationVisitor();
    final NamedConstantCheckerVisitor namedConstantChecker = new NamedConstantCheckerVisitor();
    final ReassignmentVisitor reassignmentChecker = new ReassignmentVisitor(env);

    Variable ans = new Variable("ans");

    Scanner sc = new Scanner(System.in);

    System.out.println("Welcome to the Symbolic Calculator!");

    while (true) {
      
      System.out.print(prompt);
      String input = null;

      try {
        input = sc.nextLine();
        // String input = System.console().readLine();
        
      } catch (NoSuchElementException e) { //FÖRKLARAR SEN
        break;
      }
      
      
      try {
        SymbolicExpression parsedExpr = parser.parse(input, env, functions);
        
        if (parsedExpr instanceof FunctionDefinition) {
          FunctionDefinition newFunction = (FunctionDefinition) parsedExpr;
          visitor.evaluate(newFunction,env,functions); // TODO: ta bort tror jag
          
          while (true) {
            System.out.print("  ");
            input = sc.nextLine();
            // input = System.console().readLine();

            if (input.equals("end")){
              break;
            }
            
            
            SymbolicExpression nextLineExpr = parser.parse(input, new Environment(), functions);
            newFunction.getSequence().addExpr(nextLineExpr);
          
          }
        }

        try {
          namedConstantChecker.check(parsedExpr);
          reassignmentChecker.check(parsedExpr);

        } catch (IllegalReassignmentException e) {
          System.out.println(e.getMessage());
          // System.out.println("BIGGIE BOI reassignmentChecker.ToString()");
          System.out.println(reassignmentChecker.ToString());
          enteredExpressions++;
          continue;

        } catch (IllegalNamedConstantException e) {
          System.out.println(e.getMessage());
          System.out.println(namedConstantChecker.ToString());
          enteredExpressions++;
          unsuccessfullyEvaluated++;
          continue;
        }

        if (parsedExpr.isCommand()) {
          if (parsedExpr == Quit.instance()) {

            System.out.println("Thank you for your visit! Hope you learned a lesson! WE did at least.");
            System.out.println("Entered expressions: " + enteredExpressions);
            System.out.println("Successully evaluated expressions: " + (enteredExpressions - unsuccessfullyEvaluated));
            System.out.println("Fully evaluated expressions: " + fullyEvaluated);
            sc.close();
            break;

          } else if (parsedExpr == Vars.instance()) {
            System.out.println(env);

          } else if (parsedExpr == Clear.instance()) {
            env.clear();
          }
        } else {

          enteredExpressions++;

          SymbolicExpression evaluatedExpr = visitor.evaluate(parsedExpr, env, functions);

          if (evaluatedExpr.isConstant()) {
            // SymbolicExpression a = new Assignment(evaluatedExpr, ans);
            visitor.evaluate(new Assignment(evaluatedExpr, ans), env, functions);
          }

          if (evaluatedExpr.isConstant() || evaluatedExpr.isVariable()) {
            fullyEvaluated++;
          }
          System.out.println(evaluatedExpr);
        }
      } catch (SyntaxErrorException e) {
        System.out.println(e.getMessage());
      } catch (IOException e) {
        System.out.println(e.getMessage());
      } catch (IllegalExpressionException e) {
        System.out.println(e.getMessage());
        unsuccessfullyEvaluated++;
      } catch (Exception e) {
        System.out.println(e.getMessage());
      }
    }
  }
}
