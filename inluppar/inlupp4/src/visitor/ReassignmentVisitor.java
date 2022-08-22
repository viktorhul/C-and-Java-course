package visitor;

import visitor.exceptions.*;

import calculator.ast.*;
//import calculator.ast.functions.*;
import visitor.*;

import java.util.*;

public class ReassignmentVisitor implements Visitor {
    public Environment calculatorEnvironment;
    private HashMap<SymbolicExpression, SymbolicExpression> reassignedVariables;

    private Stack<Environment> stackEnv = new Stack<Environment>();

    public ReassignmentVisitor(Environment env) {
        this.calculatorEnvironment = env;
        this.reassignedVariables = new HashMap<SymbolicExpression, SymbolicExpression>();

        this.stackEnv.push(env);
    }

    public String ToString() {
        String res = "";
        for (SymbolicExpression val : this.reassignedVariables.keySet()) {
            String key = val.toString();
            String value = this.reassignedVariables.get(val).toString();
            res += value + " = " + key + "\n";
        }
        return res;
    }

    public void check(SymbolicExpression n) throws IllegalReassignmentException {
        this.reset();
        n.accept(this);

        if (!this.reassignedVariables.isEmpty()) {
            throw new IllegalReassignmentException("Error: cannot reassign constants: ");
        }
    }

    public void reset() {
        this.reassignedVariables.clear();
    }

    public SymbolicExpression visit(Addition a) {
        a.getlhs().accept(this);
        a.getrhs().accept(this);
        return a;
    }

    public SymbolicExpression visit(Subtraction a) {
        a.getlhs().accept(this);
        a.getrhs().accept(this);
        return a;
    }

    public SymbolicExpression visit(Multiplication a) {
        a.getlhs().accept(this);
        a.getrhs().accept(this);
        return a;
    }

    public SymbolicExpression visit(Division a) {
        a.getlhs().accept(this);
        a.getrhs().accept(this);
        return a;
    }

    public SymbolicExpression visit(Assignment a) {
        SymbolicExpression rhs = a.getrhs();
        SymbolicExpression lhs = a.getlhs(); // (1 = x) + {(2 + x = x) + {3 + x = x}}

        if (rhs.isVariable()) {

            if (this.stackEnv.peek().containsKey(rhs)) {
                // Variable already assigned in current scope
                this.reassignedVariables.put(new Variable(rhs.toString()), lhs);
                // EXCEPTION eller nåt idk
            } else {
                // Lägg in variable
                // System.out.println("LETS PUT THIS INTO DA stackEnv: " + lhs.accept(this));
                this.stackEnv.peek().put(new Variable(rhs.toString()), lhs.accept(this));
            }

        }

        return lhs.accept(this);
    }

    public SymbolicExpression visit(Cos a) {
        a.getExpr().accept(this);
        return a;
    }

    public SymbolicExpression visit(Sin a) {
        a.getExpr().accept(this);
        return a;
    }

    public SymbolicExpression visit(Log a) {
        a.getExpr().accept(this);
        return a;
    }

    public SymbolicExpression visit(Exp a) {
        a.getExpr().accept(this);
        return a;
    }

    public SymbolicExpression visit(Negation a) {
        a.getExpr().accept(this);
        return a;
    }

    public SymbolicExpression visit(NamedConstant a) {
        return a;
    }

    public SymbolicExpression visit(Vars n) {
        throw new IllegalVisitorException("Cannot visit 'Vars");
    }

    public SymbolicExpression visit(Quit n) {
        throw new IllegalVisitorException("Cannot visit 'Quit");
    }

    public SymbolicExpression visit(Clear n) {
        throw new IllegalVisitorException("Cannot visit 'Clear");
    }

    public SymbolicExpression visit(Constant n) {
        return n;
    }

    public SymbolicExpression visit(Variable n) {
        return n;
    }

    public SymbolicExpression visit(Scope n) {
        this.stackEnv.push(new Environment());
        n.getExpr().accept(this);
        this.reset();
        this.stackEnv.pop();
        return n;
    }

    public SymbolicExpression visit(Conditional n){
        return n;
    }

    public SymbolicExpression visit(FunctionDefinition n){
        for (SymbolicExpression expr : n.getSequence().getExpr()) {
            expr.accept(this);
        }
        return n;

    }

    public SymbolicExpression visit(FunctionCall n){
        return n;
    }

    public SymbolicExpression visit(Sequence n){
        return n;
    }



}