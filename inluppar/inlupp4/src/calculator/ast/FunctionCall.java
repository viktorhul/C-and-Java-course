package calculator.ast;

import calculator.ast.*;

import java.util.*;

import visitor.*;

public class FunctionCall extends SymbolicExpression{
    String identifier;
    ArrayList <SymbolicExpression> arguments;

    public FunctionCall(String identifier){
        this.identifier = identifier;
        this.arguments = new ArrayList <SymbolicExpression> ();
    }

    public String getName() {
        return this.identifier;
    }

    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    public ArrayList <SymbolicExpression> getArguments(){
        return this.arguments;
    }

    public void addArgument(SymbolicExpression arg){
        this.arguments.add(arg);
    }    
}
