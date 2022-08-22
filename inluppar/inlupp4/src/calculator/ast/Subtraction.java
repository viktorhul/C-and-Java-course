package calculator.ast;

import visitor.Visitor;

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


  @Override
  public SymbolicExpression accept(Visitor v) {
      return v.visit(this);
  }
}
