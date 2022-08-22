package calculator.ast;

public class Assignment extends Binary {
  public Assignment(SymbolicExpression lhs, SymbolicExpression rhs) {
    super(lhs, rhs);
  }

  public String getName() {
    return "=";
  }

  public int getPriority() {
    return 4;
  }

  public SymbolicExpression eval(Environment env) throws IllegalExpressionException {
    if (rhs.isConstant()) {
      throw new IllegalExpressionException("Error: cannot redefine named constant");
    }

    if (rhs.toString() == "ans" && env.containsKey(rhs)) {
      env.remove(rhs);
    }

    if (env.containsKey(rhs)) {
      throw new IllegalExpressionException("Error: variable already declared");
    }

    Variable var = (Variable)this.rhs;
    SymbolicExpression reducedLhs = this.lhs.eval(env);
    env.put(var, reducedLhs);
    return reducedLhs;
  }
}
