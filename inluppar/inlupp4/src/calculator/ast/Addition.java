package calculator.ast;

import visitor.Visitor;
import java.lang.Override;

public class Addition extends Binary {
  public Addition(SymbolicExpression lhs, SymbolicExpression rhs) {
    super(lhs, rhs);
  }

  public String getName() {
    return "+";
  }

  public int getPriority() {
    return 6;
  }

  public boolean equals(Object other) {
    if (other instanceof Addition) {
      return super.equals(other);
    }
    else {
      return false;
    }
  }

  @Override
  public SymbolicExpression accept(Visitor v) {
      return v.visit(this);
  }
}
