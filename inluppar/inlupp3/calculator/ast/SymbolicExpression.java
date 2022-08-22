package calculator.ast;

/**

*  The symbolicExpression is an abstract syntax tree which holds an expression writtten in Backus-Naur form.
*  @author Tim Solig, Alessandro Sisfontes Hultman, Viktor Hultsten
*  @version alpha 0.0.1
*  @since 2021-11-25
 */

public abstract class SymbolicExpression {
  
  /**
  * Checks whthe symbolic expressionnis a constantis ce
  * @return true if expression is constant
  */
  public boolean isConstant() {
    return false;
  }

  /**
  * Checks wheter the symbolic expression
  * Check wether expression is variable 
  * @return true if expression is a variable
  **/
  public boolean isVariable() {
    return false;
  }

  /**
  * String representation of an expression's operator.
  * @return name of operator(s) in the expression 
  **/
  public String getName() {
    throw new RuntimeException("getName() called on expression without operator");
  }

  /**
  * All subtypes to symbolic expressions comes with priority used when evaluating expressions.
  * Return priority in order to know evaluation order.
  * Negation:                 5
  * Addition/Subtraction:     6
  * Division/Multiplication:  10
  * Unary (Sin,Cos,Log,Exp,): 10  
  *  @return evaluation priority representet by an int
  **/
  public int getPriority() {
    return 0;
  }

  /**
  * Finds the value attribute of the Constant class
  *  @return value if constant, exception if not
  **/
  public double getValue()
  {
    throw new RuntimeException("Expression is not a constant");
  }
  
  /**
  * In order to find the objects that are commands, override the 
  * command classes to return true
  * @return true if instance is command
  **/
  public boolean isCommand() {
    return false;
  }

  /**
  * Evaluates a symbolic expression left recursively.
  * @return the, as far as possible, evaluated expression
  **/
  public abstract SymbolicExpression eval(Environment env);

  /**
  * Compare if two symbolic expressions are equal with respect to their respecive subtype instance(s).
  * @return true if two SymbolicExpressions are equal
  **/
  public boolean equals(Object other) {
    return false;
  }


}
