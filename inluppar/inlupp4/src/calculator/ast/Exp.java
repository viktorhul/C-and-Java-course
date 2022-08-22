package calculator.ast;


import visitor.Visitor;

public class Exp extends Unary {
  public Exp(SymbolicExpression expression) {
    super(expression);
  }

  public String getName() {
    return "^";
  }

  public int getPriority() {
    return 10;
  }
  
  @Override
  public SymbolicExpression accept(Visitor v) {
      return v.visit(this);
  }
}
