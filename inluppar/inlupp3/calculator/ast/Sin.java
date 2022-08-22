package calculator.ast;

public class Sin extends Unary {
  public Sin(SymbolicExpression expression) {
    super(expression);
  }

  public String getName() {
    return "sin";
  }

  public int getPriority() {
    return 10;
  }

  public SymbolicExpression eval(Environment env) {
    SymbolicExpression reducedExpr = this.expression.eval(env);
    if (reducedExpr.isConstant()) {
        return new Constant(Math.sin(reducedExpr.getValue()));
    } else {
        return new Sin(reducedExpr);
    }
  }
}
