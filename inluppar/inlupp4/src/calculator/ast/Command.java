package calculator.ast;

import visitor.exceptions.*;
import visitor.*;

public abstract class Command extends SymbolicExpression {
  public int getPriority() {
    return 10;
  }

  public boolean isCommand() {
    return true;
  }

  public SymbolicExpression accept(Visitor v) {
    return new Constant(5);
  }
}
