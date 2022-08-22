package calculator.ast;

import visitor.Visitor;

public abstract class Binary extends SymbolicExpression {
  protected SymbolicExpression lhs = null; // Binary, Unary, Atom
  protected SymbolicExpression rhs = null;

  public Binary(SymbolicExpression lhs, SymbolicExpression rhs) {
    this.lhs = lhs;
    this.rhs = rhs;
  }

  public SymbolicExpression getlhs() {
    return this.lhs;
  }

  public SymbolicExpression getrhs() {
    return this.rhs;
  }

  /**
  * Renders the expression recursively into a human readable format
  * @return string format of expression
  **/
  public String toString() {
    String result = "";

    if (this.lhs.getPriority() < this.getPriority()) {
      result += "(" + this.lhs + ")";
    } else {
      result += this.lhs;
    }

    result += " " + this.getName() + " ";

    if (this.rhs.getPriority() < this.getPriority()) {
      result += "(" + this.rhs + ")";
    } else {
      result += this.rhs;
    }
    
    return result;
  }
  
  public boolean equals(Object other) {
    if(other instanceof Binary) {
      Binary tmp = (Binary) other;
      return this.lhs.equals(tmp.lhs) && this.rhs.equals(tmp.rhs);
    }
    else {
      return false;
    }
  }
}
