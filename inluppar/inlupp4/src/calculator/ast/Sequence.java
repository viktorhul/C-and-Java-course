package calculator.ast;

import calculator.ast.*;

import java.util.*;

import visitor.*;
public class Sequence extends SymbolicExpression {
    private ArrayList <SymbolicExpression> expressions;

    public Sequence() {
        this.expressions = new ArrayList <SymbolicExpression>();
    }

    public void addExpr(SymbolicExpression expr) {
        this.expressions.add(expr);
    }

    public ArrayList<SymbolicExpression> getExpr(){
        return this.expressions;
    }

    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
