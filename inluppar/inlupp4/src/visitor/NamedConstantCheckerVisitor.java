package visitor;

import calculator.ast.*;
//import calculator.ast.functions.*;
import visitor.exceptions.*;

import java.util.HashMap;

public class NamedConstantCheckerVisitor implements Visitor {
    private HashMap<SymbolicExpression, SymbolicExpression> redefinedNamedConstants;

    public NamedConstantCheckerVisitor(){
        this.redefinedNamedConstants = new HashMap <SymbolicExpression, SymbolicExpression>();
    }

    public void reset(){
        this.redefinedNamedConstants.clear();
    }

    public String ToString(){
        String res = "";
        for (SymbolicExpression val: this.redefinedNamedConstants.keySet()) {
            String key = val.toString();
            String value = this.redefinedNamedConstants.get(val).toString();
            res += value + " = " + key + "\n";
        }
        return res;
    }

    public void check(SymbolicExpression n) throws IllegalNamedConstantException {
        this.reset();
                
        n.accept(this);    
        
        if (!this.redefinedNamedConstants.isEmpty()) {
            throw new IllegalNamedConstantException("Error: cannot redefine named constant");
        }
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
        SymbolicExpression rhs = a.getrhs().accept(this); //.accept(this);// .accept(this);
        SymbolicExpression lhs = a.getlhs(); 

        if (rhs.isNamedConstant()) {
            this.redefinedNamedConstants.put(rhs,lhs);
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
        throw new IllegalEvaluationException("Cannot visit 'Vars'");
    }
    public SymbolicExpression visit(Quit n) {
        throw new IllegalEvaluationException("Cannot visit 'Quit'");
    }
    public SymbolicExpression visit(Clear n) {
        throw new IllegalEvaluationException("Cannot visit 'Clear'");
    }
    
   

    public SymbolicExpression visit(Constant n) {
        return n; 
    }
    public SymbolicExpression visit(Variable n) {
        return n; 
    }



    

    public SymbolicExpression visit(Scope n) {  
        return n.getExpr().accept(this);
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
