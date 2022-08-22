package test;

import calculator.ast.*;
import calculator.ast.SymbolicExpression;
import visitor.*;
import visitor.exceptions.*;
import visitor.exceptions.IllegalEvaluationException;

import java.util.*;

import calculator.parser.*;
public class testTemplates {
    private HashMap <String,FunctionDefinition> functions = new HashMap <String,FunctionDefinition> ();

    
    public boolean test_getValue(SymbolicExpression evaluatedExpression, int expected){
        try{
            return evaluatedExpression.getValue() == expected;   
        } catch(RuntimeException e) {
            return false;
        }
    }

    public boolean test_isConstant(SymbolicExpression evaluatedExpression, boolean expected){
      return evaluatedExpression.isConstant() == expected;
    }
    

    public boolean test_getName(SymbolicExpression evaluatedExpression, String expected){
        try {
            return evaluatedExpression.getName().equals(expected);
        } catch (RuntimeException e) {
            return false;
        }
    }
    

    public boolean test_isCommand(SymbolicExpression evaluatedExpression, boolean expected){
        return evaluatedExpression.isCommand() == expected;
    }
    

    public boolean test_priority(SymbolicExpression evaluatedExpression, int expected){
        return evaluatedExpression.getPriority() == expected;
    }
    

    public boolean test_toString(SymbolicExpression evaluatedExpression, String expected){
        return evaluatedExpression.toString().equals(expected);
    } 
    

    public boolean test_equals(SymbolicExpression evaluatedExpression, SymbolicExpression expectedExpression){
        return evaluatedExpression.equals(expectedExpression);
    } 


    public boolean test_visitor_eval(EvaluationVisitor vis, SymbolicExpression toEvaluate,SymbolicExpression expected, Environment env){
        try {
            return expected.equals(vis.evaluate(toEvaluate,env,functions));
        } catch (Exception e) {
        // } catch (IllegalEvaluationException e) {
            return false;
        }

    }

    public boolean test_visitor_eval_parse_scope(EvaluationVisitor vis, CalculatorParser parser, String toParse,Environment environment,SymbolicExpression expected){
        try {
            return vis.evaluate(parser.parse(toParse, environment, functions), environment, functions).equals(expected);
            
        } catch (Exception e) {
            System.out.println("Biggie boi");
            return false;
        }

    }

    public boolean test_parse_evaluate_conditional(EvaluationVisitor vis, CalculatorParser parser, String toParse, Environment env, SymbolicExpression expected){
        try {
            return vis.evaluate(parser.parse(toParse, env, functions), env, functions).equals(expected);   
        } catch (Exception e) {
            System.out.println(e.getMessage());
            return false;
        } 
    } 
}
