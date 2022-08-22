package calculator.ast;

public class Vars extends Command {
    private static final Vars theInstance = new Vars();
    
    private Vars() {}

    /**
    * Returns the instance
    @return the instance
    **/
    public static Vars instance() {
        return theInstance;
    }
}
