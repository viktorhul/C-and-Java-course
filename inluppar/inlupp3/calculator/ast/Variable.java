package calculator.ast;

public class Variable extends Atom {
  private String identifier;

  public Variable(String identifier) {
    this.identifier = identifier;
  }

  public String toString() {
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

  public SymbolicExpression eval(Environment env) {
    if (env.containsKey(this)) {
      return env.get(this);
    }
    else {
      return this;
    }
  }

}
