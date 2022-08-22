package calculator.tests;

import calculator.ast.*;

public class Test {

  public static void testPrinting(String expected, SymbolicExpression e) {
    assert(e == null);
    if (expected.equals("" + e)) {
        System.out.println("Passed: " + e);
    } else {
        System.out.println("Error: expected '" + expected + "' but got '" + e + "'");
    }
  }

  public static void testEvaluating(SymbolicExpression expected, SymbolicExpression e, Environment env) {
    SymbolicExpression r = e.eval(env);
    if (r.equals(expected)) {
        System.out.println("Passed: " + e + " = " + expected);
    } else {
        System.out.println("Error: expected '" + expected + "' but got '" + r + "'");
    }
  }

  public static void main(String[] args) {
    // 4 + 6 == 10
    testEvaluating(
      new Constant(10),
      new Addition(
        new Constant(4),
        new Constant(6)
      ),
      new Environment()
    );
    
    // 10 = x = y
    // x + x == 20
    testEvaluating(
      new Constant(20),
      new Addition(
        new Assignment(
          new Assignment(
            new Constant(10),
            new Variable("x")
          ),
          new Variable("y")
        ),
        new Variable("x")
      ),
      new Environment()
    );

    // 5 = x
    // 1 + 2 + 1 + x == 9
    testEvaluating(
      new Constant(9),
      new Addition(
        new Addition(
          new Constant(1),
          new Constant(2)
        ),
        new Addition(
          new Constant(1),
          new Assignment(
            new Constant(5),
            new Variable("x")
          )
        )
      ),
      new Environment()
    );
    
    // (2 + 3) * (1 + 4) == 25
    Multiplication m1 = new Multiplication(
      new Addition(
        new Constant(2),
        new Constant(3)
      ),
      new Addition(
        new Constant(1),
        new Constant(4)
      )
    );
    
    testEvaluating(
      new Constant(25),
      m1,
      new Environment()
    );

    // 10 = y
    // -y == -10
    Constant c1 = new Constant(10);
    Variable v1 = new Variable("y");
    Assignment a1 = new Assignment(c1, v1);
    Negation n1 = new Negation(v1);
    Constant c2 = new Constant(-10);
    Environment env = new Environment();

    testEvaluating(c1, a1, env);
    testEvaluating(c2, n1, env);
  }
}
