package calculator.ast;

public class Exp extends Unary {
  public Exp(SymbolicExpression expression) {
    super(expression);
  }

  public String getName() {
    return "^";
  }

  public int getPriority() {
    return 10;
  }
  
  public SymbolicExpression eval(Environment env) {
    SymbolicExpression reducedExpr = this.expression.eval(env);
    if (reducedExpr.isConstant()) {
        return new Constant(Math.exp(reducedExpr.getValue()));
    } else {
        return new Exp(reducedExpr);
    }
  }
}
