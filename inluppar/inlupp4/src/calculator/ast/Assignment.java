package calculator.ast;

import visitor.Visitor;

public class Assignment extends Binary {
  public Assignment(SymbolicExpression lhs, SymbolicExpression rhs) {
    super(lhs, rhs);
  }

  public String getName() {
    return "=";
  }

  public int getPriority() {
    return 4;
  }

  @Override
  public SymbolicExpression accept(Visitor v) {
      return v.visit(this);
  }
}
