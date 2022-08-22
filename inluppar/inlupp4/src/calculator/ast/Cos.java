package calculator.ast;

import visitor.Visitor;


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

  @Override
  public SymbolicExpression accept(Visitor v) {
      return v.visit(this);
  }
}
