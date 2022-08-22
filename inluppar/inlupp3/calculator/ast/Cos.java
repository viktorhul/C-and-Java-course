package calculator.ast;

public class Cos extends Unary {
  public Cos(SymbolicExpression expression) {
    super(expression);
  }

  public String getName() {
    return "cos";
  }

  public int getPriority() {
    return 10;
  }

  public SymbolicExpression eval(Environment env) {
    SymbolicExpression reducedExpr = this.expression.eval(env);
    if (reducedExpr.isConstant()) {
        return new Constant(Math.cos(reducedExpr.getValue()));
    } else {
        return new Cos(reducedExpr);
    }
  }
}
