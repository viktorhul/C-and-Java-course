package test;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Disabled;

import static org.junit.jupiter.api.Assertions.*;

import java.lang.Math.*;
import java.util.*;

import calculator.ast.*;
import calculator.Calculator;
import calculator.parser.CalculatorParser;

import test.*;

import visitor.EvaluationVisitor;

public class AllTests {
    private static StringBuilder output = new StringBuilder("");

    final testTemplates t = new testTemplates();
    final CalculatorParser parser = new CalculatorParser();

    final EvaluationVisitor vis = new EvaluationVisitor();
    final Environment env = new Environment();
    final HashMap<String, FunctionDefinition> functions = new HashMap<String, FunctionDefinition>();

    Quit quit = Quit.instance();
    Clear clear = Clear.instance();
    Vars vars = Vars.instance();

    Constant c0 = new Constant(0);
    Constant c1 = new Constant(1);
    Constant c2 = new Constant(2);
    Constant c3 = new Constant(3);
    Constant c4 = new Constant(4);
    Constant c5 = new Constant(5);
    Constant c10 = new Constant(10);

    Constant c20 = new Constant(20);
    Constant c42 = new Constant(42);
    Constant c4711 = new Constant(4711);

    Variable v1 = new Variable("x");
    Variable v2 = new Variable("y");

    Addition add = new Addition(c1, c2);
    Subtraction sub = new Subtraction(c2, c1);
    Multiplication mult = new Multiplication(c1, c2);
    Division div = new Division(c2, c1);

    Assignment assign = new Assignment(c1, v1);

    Exp exp = new Exp(c1);
    Cos cos = new Cos(c1);
    Log log = new Log(c1);
    Negation neg = new Negation(c1);
    Sin sin = new Sin(c1);

    Addition addTree;
    Subtraction subTree;
    Multiplication multTree;
    Division divTree;

    @BeforeEach
    public void init() {

        // -((-1) * 5) + 15 = 20
        addTree = new Addition(
                new Negation(
                        new Multiplication(
                                c1,
                                new Negation(c5))),
                new Constant(15));

        // // 0 - 0 = 0 wow bigboy brain
        subTree = new Subtraction(
                new Constant(0),
                new Constant(0));

        // // = = 1 * (4 + (-1)) =
        multTree = new Multiplication(
                c1,
                new Addition(
                        c1,
                        new Cos(new Constant(0))));

        // // 10 / 5 = 2
        divTree = new Division(
                new Constant(10),
                c5);
    }

    @Test
    @DisplayName("Unit | Addition")
    void test_Addition() {
        assertFalse(t.test_getValue(add, 7));
        assertTrue(t.test_isConstant(add, false));
        assertTrue(t.test_getName(add, "+"));
        assertTrue(t.test_isCommand(add, false));
        assertTrue(t.test_priority(add, 6));
    }

    @Test
    @DisplayName("Unit | Division ")
    void test_Division() {
        assertFalse(t.test_getValue(div, 7 / 3));
        assertTrue(t.test_isConstant(div, false));
        assertTrue(t.test_getName(div, "/"));
        assertTrue(t.test_isCommand(div, false));
        assertTrue(t.test_priority(div, 7));
    }

    @Test
    @DisplayName("Unit | Multiplication")
    void test_Multiplication() {
        assertFalse(t.test_getValue(mult, 7 * 3));
        assertTrue(t.test_isConstant(mult, false));
        assertTrue(t.test_getName(mult, "*"));
        assertTrue(t.test_isCommand(mult, false));
        assertTrue(t.test_priority(mult, 7));
    }

    @Test
    @DisplayName("Unit | Subtraction")
    void test_Subtraction() {
        assertFalse(t.test_getValue(sub, 1));
        assertTrue(t.test_isConstant(sub, false));
        assertTrue(t.test_getName(sub, "-"));
        assertTrue(t.test_isCommand(sub, false));
        assertTrue(t.test_priority(sub, 6));
    }

    @Test
    @DisplayName("Unit | Exp")
    void test_Exp() {
        assertFalse(t.test_getValue(exp, 1));
        assertTrue(t.test_isConstant(exp, false));
        assertTrue(t.test_getName(exp, "^"));
        assertTrue(t.test_isCommand(exp, false));
        assertTrue(t.test_priority(exp, 10));
    }

    @Test
    @DisplayName("Unit | Cos")
    void test_Cos() {
        assertFalse(t.test_getValue(cos, 1));
        assertTrue(t.test_isConstant(cos, false));
        assertTrue(t.test_getName(cos, "cos"));
        assertTrue(t.test_isCommand(cos, false));
        assertTrue(t.test_priority(cos, 10));
    }

    @Test
    @DisplayName("Unit | Log")
    void test_Log() {
        assertFalse(t.test_getValue(log, 1));
        assertTrue(t.test_isConstant(log, false));
        assertTrue(t.test_getName(log, "log"));
        assertTrue(t.test_isCommand(log, false));
        assertTrue(t.test_priority(log, 10));

    }

    @Test
    @DisplayName("Unit | Negation")
    void test_Negation() {
        assertFalse(t.test_getValue(neg, 1));
        assertTrue(t.test_isConstant(neg, false));
        assertTrue(t.test_getName(neg, "-"));
        assertTrue(t.test_isCommand(neg, false));
        assertTrue(t.test_priority(neg, 5));

    }

    @Test
    @DisplayName("Unit | Sin")
    void test_Sin() {
        assertFalse(t.test_getValue(sin, 1));
        assertTrue(t.test_isConstant(sin, false));
        assertTrue(t.test_getName(sin, "sin"));
        assertTrue(t.test_isCommand(sin, false));
        assertTrue(t.test_priority(sin, 10));

    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Addition")
    public void additionTest() { // c1 + c2 = c3
        assertTrue(t.test_visitor_eval(vis, add, c3, env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Clear")
    public void clearTest() {
        assertFalse(t.test_visitor_eval(vis, clear, clear, env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Constant")
    public void constantTest() { // c2 / c1
        assertTrue(t.test_visitor_eval(vis, cos, new Constant(Math.cos(1)), env));

    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Cos")
    public void cosTest() { // Cos c1 = cos(1)
        assertTrue(t.test_visitor_eval(vis, cos, new Constant(Math.cos(1)), env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Division")
    public void divisionTest() { // c2 / c1 = c2
        assertTrue(t.test_visitor_eval(vis, div, c2, env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Exp")
    public void expTest() { // Exp c1 = e^1
        assertTrue(t.test_visitor_eval(vis, exp, new Constant(Math.exp(1)), env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Log")
    public void logTest() { // Log c1 = ln(1)
        assertTrue(t.test_visitor_eval(vis, log, new Constant(Math.log(1)), env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Multiplication")
    public void multiplicationTest() { // c1 * c2 = c2
        assertTrue(t.test_visitor_eval(vis, mult, c2, env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Negation")
    public void negationTest() { // Neg c1 = Constant(-1)
        assertTrue(t.test_visitor_eval(vis, neg, new Constant(-1), env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Quit")
    public void quitTest() {
        assertFalse(t.test_visitor_eval(vis, quit, quit, env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Sin")
    public void sinTest() { // Sin c1 = sin(1)
        assertTrue(t.test_visitor_eval(vis, sin, new Constant(Math.sin(1)), env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Subtraction")
    public void subtractionTest() { // c2 - c1 = c1
        assertTrue(t.test_visitor_eval(vis, sub, c1, env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Assignment")
    public void variableAndVariableTest() { // 1 = x, x = 1
        assertTrue(t.test_visitor_eval(vis, assign, c1, env));
        assertTrue(t.test_visitor_eval(vis, v1, c1, env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Vars")
    public void varsTest() {
        assertFalse(t.test_visitor_eval(vis, vars, vars, env));
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ««« INTEGRATION TESTS
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // SymbolicExpression parsedExpr = parser.parse(addStr,env);

    @Test
    @DisplayName("Integration parse-eval | Scope, {1 = x} + {1 = x} ")
    public void scope1() {

        String str = "{1 = x} + {1 = x}";
        SymbolicExpression exp = c2;
        assertTrue(t.test_visitor_eval_parse_scope(vis, parser, str, env, exp));
    }

    @Test
    @DisplayName("Integration parse-eval | Scope, {1 = x} + {x}")
    public void scope2() {
        String str = "{1 = x} + {x}";

        SymbolicExpression exp = new Addition(
                new Constant(1),
                new Variable("x"));
        assertTrue(t.test_visitor_eval_parse_scope(vis, parser, str, env, exp));
    }

    @Test
    @DisplayName("Integration parse-eval | Scope, {{1 = x} = x}")
    public void scope3() {
        String str = "{{1 = x} = x}";

        SymbolicExpression exp = c1;
        assertTrue(t.test_visitor_eval_parse_scope(vis, parser, str, env, exp));
    }

    @Test
    @DisplayName("Integration parse-eval | Scope, {(2 = x) + {1 = x}}")
    public void scope4() {
        String str = "{(2 = x) + {1 = x}}";

        SymbolicExpression exp = c3;
        assertTrue(t.test_visitor_eval_parse_scope(vis, parser, str, env, exp));
    }

    @Test
    @DisplayName("Integration parse-eval | Scope, (1 = x) + {(2 + x = x) + {3 + x = x}}")
    public void scope5() {
        String str = "(1 = x) + {(2 + x = x) + {3 + x = x}}";

        SymbolicExpression exp = c10;
        assertTrue(t.test_visitor_eval_parse_scope(vis, parser, str, env, exp));
    }

    @Test
    @DisplayName("Integration parse-eval | Scope, {{1 = x} = x} = y")
    public void scope6() {
        String str = "{{1 = x} = x} = y";

        assertTrue(t.test_visitor_eval_parse_scope(vis, parser, str, env, c1));
        assertTrue(t.test_visitor_eval_parse_scope(vis, parser, "x", env, v1));
        assertTrue(t.test_visitor_eval_parse_scope(vis, parser, "y", env, c1));
    }

    // '@Test
    // @DisplayName("Integration | AST-Parser")
    // void test_AST_Parser_Integration() {

    // String addStr = addTree.toString();
    // String subStr = subTree.toString();
    // String multStr = multTree.toString();
    // String divStr = divTree.toString();

    // SymbolicExpression parsedAddExpr = null;
    // SymbolicExpression parsedSubExpr = null;
    // SymbolicExpression parsedMultExpr = null;
    // SymbolicExpression parsedDivExpr = null;

    // try {
    // parsedAddExpr = parser.parse(addStr, env, functions);
    // parsedSubExpr = parser.parse(subStr, env, functions);
    // parsedMultExpr = parser.parse(multStr, env, functions);
    // parsedDivExpr = parser.parse(divStr, env, functions);
    // } catch (Exception e) {
    // System.out.println(e.getMessage());
    // }

    // assertTrue(add.equals(parsedAddExpr));
    // assertTrue(sub.equals(parsedSubExpr));
    // assertTrue(mult.equals(parsedMultExpr));
    // assertTrue(div.equals(parsedDivExpr));
    // }'

    @Test
    @DisplayName("Unit | evaluationVisitor, Addition tree")
    public void addTreeTest() {
        assertTrue(t.test_visitor_eval(vis, addTree, c20, env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Subtraction tree")
    public void subTreeTest() {
        assertTrue(t.test_visitor_eval(vis, subTree, c0, env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Multiplication tree")
    public void multTreeTest() {
        assertTrue(t.test_visitor_eval(vis, multTree, c2, env));
    }

    @Test
    @DisplayName("Unit | evaluationVisitor, Division tree")
    public void divTreeTest() {
        assertTrue(t.test_visitor_eval(vis, divTree, c2, env));
    }

    @Test
    @DisplayName("Integration test conditional-evaluate | leq, true")
    public void conditional1() {

        String str = "if c3 <= c4 {42} else {4711}";

        SymbolicExpression leftIdentifier = c3;
        String operation = "<=";
        SymbolicExpression rightIdentifier = c4;
        Scope trueValue = new Scope(c42);
        Scope falseValue = new Scope(c4711);
        SymbolicExpression biggie = new Conditional(leftIdentifier, operation, rightIdentifier, trueValue, falseValue);

        SymbolicExpression exp = c42;

        assertTrue(vis.evaluate(biggie, env, functions).equals(exp));
    }

    @Test
    @DisplayName("Integration test conditional-evaluate | geq, true")
    public void conditional2() {

        SymbolicExpression leftIdentifier = c3;
        String operation = ">=";
        SymbolicExpression rightIdentifier = c4;
        Scope trueValue = new Scope(c42);
        Scope falseValue = new Scope(c4711);
        SymbolicExpression biggie = new Conditional(leftIdentifier, operation, rightIdentifier, trueValue, falseValue);

        SymbolicExpression exp = c4711;

        assertTrue(vis.evaluate(biggie, env, functions).equals(exp));
    }

    @Test
    @DisplayName("Integration conditional-evaluate | eq, true")
    public void conditional3() {

        SymbolicExpression leftIdentifier = c3;
        String operation = "<";
        SymbolicExpression rightIdentifier = c4;
        Scope trueValue = new Scope(c42);
        Scope falseValue = new Scope(c4711);

        SymbolicExpression exp = c42;

        SymbolicExpression biggie = new Conditional(leftIdentifier, operation, rightIdentifier, trueValue, falseValue);

        assertTrue(vis.evaluate(biggie, env, functions).equals(exp));
    }

    @Test
    @DisplayName("Integration conditional-evaluate | gt, true")
    public void conditional4() {
        SymbolicExpression leftIdentifier = c3;
        String operation = ">"; // bigboy easter egg
        SymbolicExpression rightIdentifier = c4;
        Scope trueValue = new Scope(c42);
        Scope falseValue = new Scope(c4711);

        SymbolicExpression exp = c4711;

        SymbolicExpression biggie = new Conditional(leftIdentifier, operation, rightIdentifier, trueValue, falseValue);

        assertTrue(vis.evaluate(biggie, env, functions).equals(exp));
    }

    @Test
    @DisplayName("Integration conditional-evaluate | eq, true")
    public void conditional5() {

        SymbolicExpression leftIdentifier = c3;
        String operation = "==";
        SymbolicExpression rightIdentifier = c4;
        Scope trueValue = new Scope(c42);
        Scope falseValue = new Scope(c4711);

        SymbolicExpression exp = c4711;

        SymbolicExpression biggie = new Conditional(leftIdentifier, operation, rightIdentifier, trueValue, falseValue);

        assertTrue(vis.evaluate(biggie, env, functions).equals(exp));
    }

    @Test
    @DisplayName("Integration conditional-evaluate | nested, true")
    public void conditional6() {

        SymbolicExpression leftIdentifier = c4;
        String operation = "==";
        SymbolicExpression rightIdentifier = c5;
        Scope trueValue = new Scope(c1);
        Scope falseValue = new Scope(c2);

        SymbolicExpression biggie = new Conditional(leftIdentifier, operation, rightIdentifier, trueValue, falseValue);

        String operation1 = "<";
        SymbolicExpression leftIdentifier1 = c3;
        SymbolicExpression rightIdentifier1 = c4;
        Scope trueValue1 = new Scope(biggie);
        Scope falseValue1 = new Scope(c4711);
        SymbolicExpression biggie1 = new Conditional(leftIdentifier1, operation1, rightIdentifier1, trueValue1,
                falseValue1);

        SymbolicExpression exp = c2;

        assertTrue(vis.evaluate(biggie1, env, functions).equals(exp));

    }

    @Test
    @DisplayName("Integration parser-conditional-visitorEvaluate | leq")
    public void test_conditional1a() {
        String str = "If 3 <= 2 {1} Else {0}";
        assertTrue(t.test_parse_evaluate_conditional(vis, parser, str, env, c0));
    }

    @Test
    @DisplayName("Integration parser-conditional-visitorEvaluate | leq")
    public void test_conditional1b() {
        String str = "If 2 <= 2 {1} Else {0}";
        assertTrue(t.test_parse_evaluate_conditional(vis, parser, str, env, c1));
    }

    @Test
    @DisplayName("Integration parser-conditional-visitorEvaluate | geq, false")
    public void test_conditional2() {
        String str = "If 2 >= 3 {1} Else {0}";
        assertTrue(t.test_parse_evaluate_conditional(vis, parser, str, env, c0));
    }

    @Test
    @DisplayName("Integration parser-conditional-visitorEvaluate | geq, true")
    public void test_conditional2b() {
        String str = "If 2 >= 2 {1} Else {0}";
        assertTrue(t.test_parse_evaluate_conditional(vis, parser, str, env, c1));
    }

    @Test
    @DisplayName("Integration parser-conditional-visitorEvaluate | lt, true")
    public void test_conditional3a() {
        String str = "If 2 < 3 {1} Else {0}";
        assertTrue(t.test_parse_evaluate_conditional(vis, parser, str, env, c1));
    }

    @Test
    @DisplayName("Integration parser-conditional-visitorEvaluate | lt, false")
    public void test_conditional3b() {
        String str = "If 3 < 3 {1} Else {0}";
        assertTrue(t.test_parse_evaluate_conditional(vis, parser, str, env, c0));
    }

    @Test
    @DisplayName("Integration parser-conditional-visitorEvaluate | gt, true")
    public void test_conditional4a() {
        String str = "If 3 > 2 {1} Else {0}";
        assertTrue(t.test_parse_evaluate_conditional(vis, parser, str, env, c1));
    }

    @Test
    @DisplayName("Integration parser-conditional-visitorEvaluate | gt, false")
    public void test_conditional4b() {
        String str = "If 2 > 2 {1} Else {0}";
        assertTrue(t.test_parse_evaluate_conditional(vis, parser, str, env, c0));
    }

    @Test
    @DisplayName("Integration parser-conditional-visitorEvaluate | eq, true")
    public void test_conditional5a() {
        String str = "If 2 == 2 {1} Else {0}";
        assertTrue(t.test_parse_evaluate_conditional(vis, parser, str, env, c1));
    }

    @Test
    @DisplayName("Integration parser-conditional-visitorEvaluate | eq, false")
    public void test_conditional5b() {
        String str = "If 3 == 2 {1} Else {0}";
        assertTrue(t.test_parse_evaluate_conditional(vis, parser, str, env, c0));
    }

    @Test
    @DisplayName("Integration parser-conditional-visitorEvaluate | nestlé")
    public void test_conditional6() {
        String str = "If 2 == 2 {{If 5 > 4 {15} Else {5}} + 5} Else {0}";
        assertTrue(t.test_parse_evaluate_conditional(vis, parser, str, env, c20));
    }

}