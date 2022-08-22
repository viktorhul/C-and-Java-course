package calculator.ast;

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

  public SymbolicExpression eval(Environment env) {
    return this;
  }

  public boolean equals(Object other) {
    if (other instanceof Constant) {
      return this.value == ((Constant)other).value;
    }
    else {
      return false;
    }
  }

  public String toString() {
    return String.valueOf(this.value);
  }
}
