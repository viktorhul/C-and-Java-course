package calculator;

import calculator.ast.*;
import calculator.parser.*;
import java.io.IOException;
import java.util.Scanner;
import java.util.Map;

public class Calculator {
  public static void main(String[] args) {
    int enteredExpressions = 0;
    int unsuccessfullyEvaluated = 0;
    int fullyEvaluated = 0;
    String prompt = ">> ";
    final CalculatorParser parser = new CalculatorParser();
    final Environment env = new Environment();

    Variable ans = new Variable("ans");
    
    Scanner sc = new Scanner(System.in);
    System.out.println("Welcome to the Symbolic Calculator!");

    while (true) {
      System.out.print(prompt);
      String input = System.console().readLine();

      try {
        SymbolicExpression parsedExpr = parser.parse(input, env);
        if (parsedExpr.isCommand()) {
          if (parsedExpr == Quit.instance()) {
            System.out.println("A nice message");
            // print statistics
            System.out.println("Entered expressions: " + enteredExpressions);
            System.out.println("Successully evaluated expressions: " + (enteredExpressions - unsuccessfullyEvaluated));
            System.out.println("Fully evaluated expressions: " + fullyEvaluated);
            sc.close();
            break;
          } else if (parsedExpr == Vars.instance()) {
            for (Map.Entry<Variable, SymbolicExpression> entry : env.entrySet()) {
              System.out.println("  " + entry.getKey() + " = " + entry.getValue());
            }
          } else if (parsedExpr == Clear.instance()) {
            env.clear();
          }
        } else {
          enteredExpressions++;
          SymbolicExpression evaluatedExpr = parsedExpr.eval(env);

          if (evaluatedExpr.isConstant()) {
            Assignment a = new Assignment(evaluatedExpr, ans);
            a.eval(env);
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
      }
    }
  }
}
