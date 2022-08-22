package calculator.ast;

public class IllegalExpressionException extends RuntimeException {
  public IllegalExpressionException(String str) {
    super(str);
  }
}
