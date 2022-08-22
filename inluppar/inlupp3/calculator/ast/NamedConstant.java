package calculator.ast;

public class NamedConstant extends Atom {
  private String name;
  private double value;

  public NamedConstant(String name, Double value) {
    this.name = name;
    this.value = value;
  }

  public String getName() {
    return "namedConstant";
  }

  public boolean isConstant() {
    return this instanceof NamedConstant;
  }

  public SymbolicExpression eval(Environment env) throws IllegalExpressionException {
    Double result = Constants.namedConstants.get(this.name);
    if (result == null) {
      throw new IllegalExpressionException(this.name + " is not a named constant");
    }
    return new Constant(result);
  } 
}
