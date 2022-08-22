package calculator.ast;

import calculator.ast.*;

import java.util.*;

import visitor.*;

public class FunctionDefinition extends SymbolicExpression{
    private String name;
    private ArrayList <String> parameters;
    private Sequence body;


    public FunctionDefinition(String name) {
        this.parameters = new ArrayList <String> ();
        this.name = name;
        this.body = new Sequence();
    }

    public String toString(){
        return "Function " + getName() + " was defined!";
    }

    public String getName() {
        return this.name;
    }
    
    public void addParameter(String param){
        this.parameters.add(param);
    }

    public ArrayList <String> getParameters(){
        return this.parameters;
    }

    public Sequence getSequence(){
        return this.body;
    }

    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}