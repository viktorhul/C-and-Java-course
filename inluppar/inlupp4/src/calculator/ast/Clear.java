package calculator.ast;

public class Clear extends Command {
  private static final Clear theInstance = new Clear();

  private Clear() {
  }

  /**
  * Returns the instance
  @return the instance
  **/
  public static Clear instance() {
    return theInstance;
  }
}
