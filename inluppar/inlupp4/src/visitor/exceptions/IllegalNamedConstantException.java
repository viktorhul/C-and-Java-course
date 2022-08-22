package visitor.exceptions;

public class IllegalNamedConstantException extends RuntimeException{
   
    public IllegalNamedConstantException(String msg){
        super(msg);
    }
}
