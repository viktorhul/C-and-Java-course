package calculator.ast;

public abstract class Command extends SymbolicExpression {
  public int getPriority() {
    return 10;
  }

  public SymbolicExpression eval(Environment env) {
    throw new RuntimeException("Commands can't be evaluated!");
  }

  public boolean isCommand() {
    return true;
  }
}
