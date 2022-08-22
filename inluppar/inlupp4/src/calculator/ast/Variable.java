package calculator.ast;

import visitor.Visitor;

public class Variable extends Atom implements Comparable{
  private String identifier;

  public Variable(String identifier) {
    this.identifier = identifier;
  }

  public String toString() {
    return this.identifier;
  }

  public String getName() {
    return this.identifier;
  }

  public boolean equals(Object other) {
    if(other instanceof Variable) {
      return this.identifier.equals(((Variable)other).identifier);
    }
    else {
      return false;
    }
  }

  public boolean isVariable() {
    return true;
  }

  public int hashCode() {
    return this.identifier.hashCode();
  }



  @Override
  public int compareTo(Object v1){
    if (v1 instanceof Variable) {
      return this.identifier.compareTo(v1.toString());
    } else {
      return 0;
    }
  }

  @Override
  public SymbolicExpression accept(Visitor v) {
      return v.visit(this);
  }

}
