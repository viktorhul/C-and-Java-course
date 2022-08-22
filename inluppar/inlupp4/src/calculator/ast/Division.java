package calculator.ast;


import visitor.Visitor;

public class Division extends Binary {
  public Division(SymbolicExpression lhs, SymbolicExpression rhs) {
    super(lhs, rhs);
  }

  public String getName() {
    return "/";
  }

  public int getPriority() {
    return 7;
  }

  @Override
  public SymbolicExpression accept(Visitor v) {
      return v.visit(this);
  }
}
