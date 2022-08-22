package calculator.ast;

public abstract class Unary extends SymbolicExpression {
  protected SymbolicExpression expression;

  public Unary(SymbolicExpression expression) {
    this.expression = expression;
  }

  public SymbolicExpression getExpr(){
    return this.expression;
  }

  public boolean equals(Object other) {
    if (other instanceof Unary) {
      return this.expression.equals(((Unary)other).expression);
    }
    else {
      return false;
    }
  }

  public String toString() {
    /// Note how the call to toString() is not necessary
    return this.getName() + " " + this.expression.toString();
  }
}
