package calculator.ast;

import visitor.Visitor;

public class Constant extends Atom {
  private double value;

  public Constant(double value) {
    this.value = value;
  }

  public boolean isConstant() {
    return true;
  }

  public double getValue() {
    return this.value;
  }

  public boolean equals(Object other) {
    if (other instanceof Constant) {
      return this.value == ((Constant) other).value;
    } else {
      return false;
    }
  }

  public boolean leq(Constant c){
    return this.getValue() <= c.getValue();
  }

  public boolean geq(Constant c){
    return this.getValue() >= c.getValue();
  }

  public boolean lt(Constant c){
    return this.getValue() < c.getValue();
  }

  public boolean gt(Constant c){
    return this.getValue() > c.getValue();
  }


  
  public String toString() {
    return String.valueOf(this.value);
  }

  @Override
  public SymbolicExpression accept(Visitor v) {
    return v.visit(this);
  }
}
