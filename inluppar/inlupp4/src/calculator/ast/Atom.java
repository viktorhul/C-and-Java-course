package calculator.ast;

public abstract class Atom extends SymbolicExpression {
  public int getPriority() {
    return 10;
  }
}
