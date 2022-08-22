package calculator.ast;

import visitor.Visitor;

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
  
  @Override
  public SymbolicExpression accept(Visitor v) {
      return v.visit(this);
  }
}
