package calculator.ast;

import calculator.ast.*;

import visitor.*;

public class Scope extends SymbolicExpression {
    private SymbolicExpression expr;

    public Scope(SymbolicExpression expr) {
        this.expr = expr;
    }

    public SymbolicExpression getExpr(){
        return this.expr;
    }

    public SymbolicExpression accept(Visitor v){
        return v.visit(this);
    }
}