package calculator.ast;

import calculator.ast.*;
import visitor.*;

public class Conditional extends SymbolicExpression {
    //IF
    private SymbolicExpression leftIdentifier;
    private String operation;
    private SymbolicExpression rightIdentifier;
    private Scope trueValue;
    private Scope falseValue;
    //Then
    //Else

    public Conditional(
        SymbolicExpression leftIdentifier, 
        String operation, 
        SymbolicExpression rightIdentifier, 
        SymbolicExpression trueValue, 
        SymbolicExpression falseValue){

        this.leftIdentifier = leftIdentifier;
        this.operation = operation;
        this.rightIdentifier = rightIdentifier;
        this.trueValue = (Scope) trueValue;
        this.falseValue = (Scope) falseValue;
    }

    public String getOp(){
        return this.operation;
    }

    public SymbolicExpression getrhs(){
        return this.rightIdentifier;
    }

    public SymbolicExpression getlhs(){
       return  this.leftIdentifier;
    }

    public Scope getTrueValue() {
        return this.trueValue;
    }   

    public Scope getFalseValue() {
        return this.falseValue;
    }
    
    public SymbolicExpression accept(Visitor v){
        return v.visit(this);
    }
}
