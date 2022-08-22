package calculator.ast;


import visitor.Visitor;

public class Log extends Unary {
  public Log(SymbolicExpression expression) {
    super(expression);
  }

  public String getName() {
    return "log";
  }

  public int getPriority() {
    return 10;
  }

  @Override
  public SymbolicExpression accept(Visitor v) {
      return v.visit(this);
  }

}
