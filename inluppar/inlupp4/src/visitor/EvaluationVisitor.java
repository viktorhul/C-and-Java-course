package visitor;

import javax.swing.plaf.synth.SynthComboBoxUI;

import calculator.ast.*;

import visitor.*;
import visitor.exceptions.*;

import java.util.*;

public class EvaluationVisitor implements Visitor {
    private Stack<Environment> envStack = new Stack<Environment>();

    private HashMap<String, FunctionDefinition> functions;

    public SymbolicExpression evaluate(SymbolicExpression topLevel, Environment env,
            HashMap<String, FunctionDefinition> functions) {
        this.envStack.push(env);
        this.functions = functions;

        return topLevel.accept(this);
    }

    @Override
    public SymbolicExpression visit(Addition n) {

        SymbolicExpression left = n.getlhs().accept(this);
        SymbolicExpression right = n.getrhs().accept(this);

        if (left.isConstant() && right.isConstant()) {
            return new Constant(left.getValue() + right.getValue());
        } else {
            return new Addition(left, right);
        }
    }

    public SymbolicExpression visit(Subtraction n) {

        SymbolicExpression left = n.getlhs().accept(this);
        SymbolicExpression right = n.getrhs().accept(this);

        if (left.isConstant() && right.isConstant()) {
            return new Constant(left.getValue() - right.getValue());
        } else {
            return new Subtraction(left, right);
        }
    }

    public SymbolicExpression visit(Multiplication n) {

        SymbolicExpression left = n.getlhs().accept(this);
        SymbolicExpression right = n.getrhs().accept(this);

        if (left.isConstant() && right.isConstant()) {
            return new Constant(left.getValue() * right.getValue());
        } else {
            return new Multiplication(left, right);
        }
    }

    public SymbolicExpression visit(Division n) {

        SymbolicExpression left = n.getlhs().accept(this);
        SymbolicExpression right = n.getrhs().accept(this);

        if (left.isConstant() && right.isConstant()) {
            return new Constant(left.getValue() / right.getValue());
        } else {
            return new Division(left, right);
        }
    }

    public SymbolicExpression visit(Constant n) {
        return n;
    }

    public SymbolicExpression visit(Cos n) {

        SymbolicExpression expr = n.getExpr().accept(this);

        if (expr.isConstant()) {
            return new Constant(Math.cos(expr.getValue()));
        } else {
            return new Cos(expr);
        }
    }

    public SymbolicExpression visit(Sin n) {
        SymbolicExpression expr = n.getExpr().accept(this);

        if (expr.isConstant()) {
            return new Constant(Math.sin(expr.getValue()));
        } else {
            return new Sin(expr);
        }
    }

    public SymbolicExpression visit(Log n) {
        SymbolicExpression expr = n.getExpr().accept(this);

        if (expr.isConstant()) {
            return new Constant(Math.log(expr.getValue()));
        } else {
            return new Log(expr);
        }
    }

    public SymbolicExpression visit(Exp n) {

        SymbolicExpression expr = n.getExpr().accept(this);

        if (expr.isConstant()) {
            return new Constant(Math.exp(expr.getValue()));
        } else {
            return new Exp(expr);
        }
    }

    public SymbolicExpression visit(Negation n) {

        SymbolicExpression expr = n.getExpr().accept(this);

        if (expr.isConstant()) {
            return new Constant(-expr.getValue());
        } else {
            return new Negation(expr);
        }
    }

    public SymbolicExpression visit(Variable n) {
        SymbolicExpression res = null;
        for (Environment env : this.envStack) {
            if (env.containsKey(n))
                res = env.get(n);
        }
        if (res == null)
            res = n;
        return res;
    }

    public SymbolicExpression visit(NamedConstant n) {
        Double result = Constants.namedConstants.get(n.toString());
            if (result == null) {
            throw new IllegalExpressionException(n + " is not a named constant");
        }
        return new Constant(result);
    }

    public SymbolicExpression visit(Assignment n) {

        SymbolicExpression lhs = n.getlhs();
        SymbolicExpression rhs = n.getrhs();

        Environment current = this.envStack.peek();

        if (rhs.toString() == "ans" && this.envStack.peek().containsKey(rhs)) {
            this.envStack.peek().remove(rhs);
        }

        Variable var = (Variable) rhs;
        SymbolicExpression reducedLhs = lhs.accept(this);
        this.envStack.peek().put(var, reducedLhs);
        return reducedLhs;
    }

    public SymbolicExpression visit(Vars n) {
        throw new IllegalEvaluationException("Cannot visit 'Vars'");
    }

    public SymbolicExpression visit(Quit n) {
        throw new IllegalEvaluationException("Cannot visit 'Quit'");
    }

    public SymbolicExpression visit(Clear n) {
        throw new IllegalEvaluationException("Cannot visit 'Clear'");
    }

    public SymbolicExpression visit(Scope n) {

        this.envStack.push(new Environment());

        SymbolicExpression reducedexpr = n.getExpr().accept(this);

        this.envStack.pop();

        return reducedexpr;
    }

    public SymbolicExpression visit(Conditional n) {

        SymbolicExpression lhs = n.getlhs().accept(this);
        SymbolicExpression rhs = n.getrhs().accept(this);
        String op = n.getOp();

        Scope trueValue = n.getTrueValue();
        Scope falseValue = n.getFalseValue();

        switch (op) {
            case "==":
                return (((Constant) lhs).equals((Constant) rhs)) ? trueValue.accept(this) : falseValue.accept(this);

            case "<=":
                return (((Constant) lhs).leq((Constant) rhs)) ? trueValue.accept(this) : falseValue.accept(this);

            case ">=":
                return (((Constant) lhs).geq((Constant) rhs)) ? trueValue.accept(this) : falseValue.accept(this);

            case "<":
                return (((Constant) lhs).lt((Constant) rhs)) ? trueValue.accept(this) : falseValue.accept(this);

            case ">":
                return (((Constant) lhs).gt((Constant) rhs)) ? trueValue.accept(this) : falseValue.accept(this);

            default:
                throw new IllegalExpressionException("Error: " + op + " is not an accepted operator!");

        }
    }

    public SymbolicExpression visit(FunctionDefinition n) {
        this.functions.put(n.getName(), n);
        return n;
    }

    public SymbolicExpression visit(FunctionCall n) {
        String identifier = n.getName();
        FunctionDefinition fun = this.functions.get(identifier);

        ArrayList<SymbolicExpression> callArguments = n.getArguments();

        ArrayList<String> functParameters = fun.getParameters();

        if (callArguments.size() != functParameters.size()) {
            throw new RuntimeException("Wrong amount of arguments. Expected " + functParameters.size() + " but got "
                    + callArguments.size());
        }

        Sequence functionSequence = new Sequence();

        Iterator<SymbolicExpression> it1 = callArguments.iterator();
        Iterator<String> it2 = functParameters.iterator();

        while (it1.hasNext() && it2.hasNext()) {
            functionSequence.addExpr(new Assignment(it1.next(), new Variable(it2.next())));
        }

        for (SymbolicExpression expr : fun.getSequence().getExpr()) {
            functionSequence.addExpr(expr);
        }

        Scope call = new Scope(functionSequence);

        return call.accept(this);
    }

    public SymbolicExpression visit(Sequence n) {
        SymbolicExpression result = null;

        for (SymbolicExpression expr : n.getExpr()) {

            result = expr.accept(this);
        }

        return result;
    }

}