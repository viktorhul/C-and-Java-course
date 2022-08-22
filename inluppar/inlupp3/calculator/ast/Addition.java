package calculator.ast;

public class Addition extends Binary {
  public Addition(SymbolicExpression lhs, SymbolicExpression rhs) {
    super(lhs, rhs);
  }

  public String getName() {
    return "+";
  }

  public int getPriority() {
    return 6;
  }

  public SymbolicExpression eval(Environment env) {
    SymbolicExpression reducedLhs = this.lhs.eval(env);
    SymbolicExpression reducedRhs = this.rhs.eval(env);

    if (reducedLhs.isConstant() && reducedRhs.isConstant()) {
      return new Constant(reducedLhs.getValue() + reducedRhs.getValue());
    }
    else {
      return new Addition(reducedLhs, reducedRhs);
    }
  }

  public boolean equals(Object other) {
    if (other instanceof Addition) {
      return super.equals(other);
    }
    else {
      return false;
    }
  }
}
