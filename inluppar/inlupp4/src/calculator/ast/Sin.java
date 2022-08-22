package calculator.ast;

import visitor.Visitor;

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


  @Override
  public SymbolicExpression accept(Visitor v) {
      return v.visit(this);
  }
}
