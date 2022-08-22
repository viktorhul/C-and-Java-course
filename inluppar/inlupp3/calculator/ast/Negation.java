package calculator.ast;

public class Negation extends Unary {
  public Negation(SymbolicExpression expression) {
    super(expression);
  }

  public String getName() {
    return "-";
  }

  public int getPriority() {
    return 5;
  }

  public String toString() {
    String result = "-";
    if (this.expression.getPriority() < this.getPriority()) {
      result += "(" + this.expression + ")";
    } else {
      result += this.expression;
    }

    return result;
  }

  public SymbolicExpression eval(Environment env) {
    SymbolicExpression reducedExpr = this.expression.eval(env);
    if (reducedExpr.isConstant()) {
        return new Constant((-1) * reducedExpr.getValue());
    } else {
        return new Negation(reducedExpr);
    }
  }
}
