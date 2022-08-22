package calculator.ast;

public class Subtraction extends Binary {
  public Subtraction(SymbolicExpression lhs, SymbolicExpression rhs) {
    super(lhs, rhs);
  }

  public String getName() {
    return "-";
  }

  public int getPriority() {
    return 6;
  }

  public SymbolicExpression eval(Environment env) {
    SymbolicExpression reducedLhs = this.lhs.eval(env);
    SymbolicExpression reducedRhs = this.rhs.eval(env);

    if (reducedLhs.isConstant() && reducedRhs.isConstant()) {
      return new Constant(reducedLhs.getValue() - reducedRhs.getValue());
    }
    else {
      return new Subtraction(reducedLhs, reducedRhs);
    }
  }
}
