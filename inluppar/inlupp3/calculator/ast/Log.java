package calculator.ast;

public class Log extends Unary {
  public Log(SymbolicExpression expression) {
    super(expression);
  }

  public String getName() {
    return "log";
  }

  public int getPriority() {
    return 10;
  }

  public SymbolicExpression eval(Environment env) {
    SymbolicExpression reducedExpr = this.expression.eval(env);
    if (reducedExpr.isConstant()) {
        return new Constant(Math.log(reducedExpr.getValue()));
    } else {
        return new Log(reducedExpr);
    }
  }
}
