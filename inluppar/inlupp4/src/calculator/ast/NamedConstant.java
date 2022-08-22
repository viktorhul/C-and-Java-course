package calculator.ast;

import visitor.Visitor;

public class NamedConstant extends Atom {
  private String name;
  private double value;

  public NamedConstant(String name, Double value) {
    this.name = name;
    this.value = value;
  }

  public String toString(){
    return this.name;
  }
  
  public String getName() {
    return "namedConstant";
  }

  public boolean isConstant() {
    return this instanceof NamedConstant;
  }

  public boolean isNamedConstant() {
    return true;
  }

  // @Override
  public SymbolicExpression accept(Visitor v) {
      return v.visit(this); 
  }
}

