package calculator.ast;

public class Quit extends Command {
    private static final Quit theInstance = new Quit();
    
    private Quit() {}

    /**
    * Returns the instance
    @return the instance
    **/
    public static Quit instance() {
        return theInstance;
    }

    // TODO: Quit the program
}
