package calculator.parser;

import calculator.ast.*;
import java.io.StreamTokenizer;
import java.io.StringReader;
import java.io.IOException;
import java.util.*;
import java.util.concurrent.LinkedTransferQueue;

import javax.management.RuntimeErrorException;

/**
 * Represents the parsing of strings into valid expressions defined in the AST.
 */
public class CalculatorParser {

    private StreamTokenizer st;
    private Environment vars;
    private HashMap<String, FunctionDefinition> functions;

    private static char MULTIPLY = '*';
    private static char ADDITION = '+';
    private static char SUBTRACTION = '-';
    private static char DIVISION = '/';
    private static char NEGATION = '-';
    private static String NEG = "Neg";
    private static String SIN = "Sin";
    private static String COS = "Cos";
    private static String LOG = "Log";
    private static String EXP = "Exp";
    private static char ASSIGNMENT = '=';

    private static String IF = "If";
    private static String ELSE = "Else";
    private static String FUNCTION = "Function";

    // unallowerdVars is used to check if variable name that we
    // want to assign new meaning to is a valid name eg 3 = Quit
    // or 10 + x = L is not allowed
    private final ArrayList<String> unallowedVars = new ArrayList<String>(
            Arrays.asList(
                    "Quit",
                    "Vars",
                    "Clear",
                    "Sin",
                    "Cos",
                    "Log",
                    "Exp",
                    "Bigboy",
                    "If",
                    "Else",
                    "Function"));

    private final ArrayList<String> operators = new ArrayList<String>(
            Arrays.asList(
                    "==",
                    ">=",
                    "<=",
                    "<",
                    ">"));

    /**
     * Used to parse the inputted string by the Calculator program
     * 
     * @param inputString the string used to parse
     * @param vars        the Environment in which the variables exist
     * @return a SymbolicExpression to be evaluated
     * @throws IOException by nextToken() if it reads invalid input
     */
    public SymbolicExpression parse(String inputString, Environment vars, HashMap<String, FunctionDefinition> functions)
            throws IOException {
        this.st = new StreamTokenizer(new StringReader(inputString)); // reads from inputString via stringreader.
        this.vars = vars;
        this.functions = functions;
        this.st.ordinaryChar('-');
        this.st.ordinaryChar('/');
        this.st.eolIsSignificant(true);
        SymbolicExpression result = statement(); // calls to statement
        return result; // the final result
    }

    /**
     * Checks wether the token read is a command or an assignment
     * 
     * @return a SymbolicExpression to be evaluated
     * @throws IOException          by nextToken() if it reads invalid input
     * @throws SyntaxErrorException if the token parsed cannot be turned into a
     *                              valid expression
     */
    private SymbolicExpression statement() throws IOException {
        SymbolicExpression result;
        this.st.nextToken(); // kollar pÃ¥ nÃ¤sta token som ligger pÃ¥ strÃ¶mmen
        if (this.st.ttype == this.st.TT_EOF) {
            throw new SyntaxErrorException("Error: Expected an expression");
        }

        if (this.st.ttype == this.st.TT_WORD) { // vilken typ det senaste tecken vi lÃ¤ste in hade.
            if (this.st.sval.equals("Quit") || this.st.sval.equals("Vars") || this.st.sval.equals("Clear")) { // sval //
                                                                                                              // Variable
                result = command();
            } else if (this.st.sval.equals(FUNCTION)) {
                result = functionDefinition();

            } else if (this.functions.containsKey(this.st.sval)) {
                result = functionCall();

            } else {
                result = assignment(); // gÃ¥r vidare med uttrycket.

            }
        } else {
            result = assignment(); // om inte == word, gÃ¥ till assignment Ã¤ndÃ¥ (kan vara tt_number)
        }

        if (this.st.nextToken() != this.st.TT_EOF) { // token should be an end of stream token if we are done
            if (this.st.ttype == this.st.TT_WORD) {
                throw new SyntaxErrorException("Error: Unexpected '" + this.st.sval + "'");
            } else {
                throw new SyntaxErrorException("Error: Unexpected '" + String.valueOf((char) this.st.ttype) + "'");
            }
        }
        return result;
    }

    /**
     * Checks what kind of command that should be returned
     * 
     * @return an instance of Quit, Clear or Vars depending on the token parsed
     * @throws IOException by nextToken() if it reads invalid input
     */
    private SymbolicExpression command() throws IOException {
        if (this.st.sval.equals("Quit")) {
            return Quit.instance();
        } else if (this.st.sval.equals("Clear")) {
            return Clear.instance();
        } else {
            return Vars.instance();
        }
    }

    /**
     * Checks that the given expression is equivalent to the structure:
     * If identifier operator identifier truevalue falseValue
     * and returns the correct format
     * 
     * @return a SymbolicExpression
     * @throws IOException
     */
    private SymbolicExpression conditionals() throws IOException {
        this.st.nextToken();
        SymbolicExpression leftIdentifier;
        SymbolicExpression rightIdentifier;
        SymbolicExpression truthValue;
        SymbolicExpression falseValue;
        String operator = "";

        leftIdentifier = expression();

        this.st.nextToken();

        int i = 1;
        while (!(this.st.ttype == this.st.TT_WORD || this.st.ttype == this.st.TT_NUMBER
                || this.st.ttype == this.st.TT_EOL || this.st.ttype == this.st.TT_EOF) && i <= 2) {
           
            operator += (char) this.st.ttype;
            this.st.nextToken();
            i++;
        }
        
        if (this.st.ttype != '=') {
            this.st.pushBack();
        }
        
        if (!this.operators.contains(operator)) {
            throw new IOException("Unsupported operator '" + operator + "'");
        }
        
        rightIdentifier = expression();

        this.st.nextToken();

        truthValue = primary();

        this.st.nextToken();

        if (this.st.ttype == this.st.TT_WORD) {
            if (!this.st.sval.equals(ELSE))
                throw new IOException("Bad formatting on conditional (else)");
        }

        this.st.nextToken();

        falseValue = primary();

        return new Conditional(leftIdentifier, operator, rightIdentifier, truthValue, falseValue);
    }

    /**
     * Checks wether the token read is an assignment between 2 expression and
     * descend into the right hand side of '='
     * 
     * @return a SymbolicExpression to be evaluated
     * @throws IOException          by nextToken() if it reads invalid input
     * @throws SyntaxErrorException if the token parsed cannot be turned into a
     *                              valid expression,
     *                              the variable on rhs of '=' is a number or
     *                              invalid variable
     */
    private SymbolicExpression functionDefinition() throws IOException {
        this.st.nextToken();
        if (this.unallowedVars.contains(this.st.sval)) {
            throw new SyntaxErrorException("Error! Cannot redeclare function " + this.st.sval);
        }

        String identifier = this.st.sval;

        FunctionDefinition newFunction = new FunctionDefinition(identifier);

        while (this.st.nextToken() != ')') {
            if (this.st.ttype == this.st.TT_WORD) {
                newFunction.addParameter(this.st.sval);
            }
        }
        return newFunction;
    }

    /**
     * Checks wether the token read is an assignment between 2 expression and
     * descend into the right hand side of '='
     * 
     * @return a SymbolicExpression to be evaluated
     * @throws IOException          by nextToken() if it reads invalid input
     * @throws SyntaxErrorException if the token parsed cannot be turned into a
     *                              valid expression,
     *                              the variable on rhs of '=' is a number or
     *                              invalid variable
     */
    private SymbolicExpression functionCall() throws IOException {
        FunctionCall newCall = new FunctionCall(this.st.sval);

        // System.out.println("functionCall: " + this.st.sval);

        this.st.nextToken(); // gå förbi funktionsnamnet

        while (this.st.nextToken() != ')') { // gå förbi första parentesen + loopa
            if (this.st.ttype != ',') {
                newCall.addArgument(assignment()); // dyker det upp en ny parentes så behandlas det som (a+b) osv
            }
        }

        return newCall;
    }

    /**
     * Checks wether the token read is an assignment between 2 expression and
     * descend into the right hand side of '='
     * 
     * @return a SymbolicExpression to be evaluated
     * @throws IOException          by nextToken() if it reads invalid input
     * @throws SyntaxErrorException if the token parsed cannot be turned into a
     *                              valid expression,
     *                              the variable on rhs of '=' is a number or
     *                              invalid variable
     */
    private SymbolicExpression assignment() throws IOException {
        SymbolicExpression result = expression();
        this.st.nextToken();
        while (this.st.ttype == ASSIGNMENT) {
            this.st.nextToken();
            if (this.st.ttype == this.st.TT_NUMBER) {
                throw new SyntaxErrorException("Error: Numbers cannot be used as a variable name");
            } else if (this.st.ttype != this.st.TT_WORD) {
                throw new SyntaxErrorException("Error: Not a valid assignment of a variable");
            } else {
                if (this.st.sval.equals("ans")) {
                    throw new SyntaxErrorException("Error: ans cannot be redefined");
                }
                SymbolicExpression key = identifier();
                result = new Assignment(result, key);
            }
            this.st.nextToken();
        }
        this.st.pushBack();
        return result;
    }

    /**
     * Check if valid identifier for variable and return that if so
     * 
     * @return a SymbolicExpression that is either a named constant or a new
     *         variable
     * @throws IOException                by nextToken() if it reads invalid input
     * @throws IllegalExpressionException if you try to redefine a string that isn't
     *                                    allowed
     */
    private SymbolicExpression identifier() throws IOException {
        SymbolicExpression result;

        if (this.unallowedVars.contains(this.st.sval)) {
            throw new IllegalExpressionException("Error: cannot redefine " + this.st.sval);
        }

        if (Constants.namedConstants.containsKey(this.st.sval)) {
            result = new NamedConstant(st.sval, Constants.namedConstants.get(st.sval));
        } else {
            result = new Variable(this.st.sval);
        }
        return result;
    }
    

    /**
     * Checks wether the token read is an addition or subtraction
     * and then continue on with the right hand side of operator
     * 
     * @return a SymbolicExpression to be evaluated
     * @throws IOException by nextToken() if it reads invalid input
     */
    private SymbolicExpression expression() throws IOException {
        SymbolicExpression result = term();
        this.st.nextToken();

        while (this.st.ttype == ADDITION || this.st.ttype == SUBTRACTION) {
            int operator = st.ttype;
            this.st.nextToken();
            if (operator == ADDITION) {
                result = new Addition(result, term());
            } else {
                result = new Subtraction(result, term());
            }
            this.st.nextToken();
        }
        this.st.pushBack();
        return result;
    }

    /**
     * Checks wether the token read is an Multiplication or
     * Division and then continue on with the right hand side of
     * operator
     * 
     * @return a SymbolicExpression to be evaluated
     * @throws IOException by nextToken() if it reads invalid input
     */
    private SymbolicExpression term() throws IOException {
        SymbolicExpression result = primary();
        this.st.nextToken();
        while (this.st.ttype == MULTIPLY || this.st.ttype == DIVISION) {
            int operator = st.ttype;
            this.st.nextToken();

            if (operator == MULTIPLY) {
                result = new Multiplication(result, primary());
            } else {
                result = new Division(result, primary());
            }
            this.st.nextToken();
        }
        this.st.pushBack();
        return result;
    }

    private SymbolicExpression primary() throws IOException {
        SymbolicExpression result;
        if (this.st.ttype == '(') {
            this.st.nextToken();
            result = assignment();
            /// This captures unbalanced parentheses!
            if (this.st.nextToken() != ')') {
                throw new SyntaxErrorException("expected ')'");
            }
        } else if (this.st.ttype == '{') {
            this.st.nextToken();
            result = new Scope(assignment());
            // this.st.nextToken();
            // System.out.println((char) this.st.nval);
            if (this.st.nextToken() != '}') {
                throw new SyntaxErrorException("expected '}'");
            }
        } else if (this.st.ttype == NEGATION) {
            result = unary();
        } else if ((this.functions.containsKey(this.st.sval))) {
            result = functionCall();
        } else if (this.st.ttype == this.st.TT_WORD) {
            if (st.sval.equals(SIN) ||
                    st.sval.equals(COS) ||
                    st.sval.equals(EXP) ||
                    st.sval.equals(NEG) ||
                    st.sval.equals(LOG)) {

                result = unary();
            } else if (this.st.sval.equals(FUNCTION)) {
                result = functionDefinition(); // TODO: Används nog inte
            } else if (this.st.sval.equals(IF)) {
                result = conditionals();
            } else if (this.st.sval.equals(ELSE)) { // TODO: is it needed
                this.st.nextToken();
                result = primary();
            } else {
                result = identifier();
            }
        } else {
            this.st.pushBack();
            result = number();
        }
        return result;
    }

    /**
     * Checks what type of Unary operator the token read is and
     * then continues with the expression that the operator holds
     * 
     * @return a SymbolicExpression to be evaluated
     * @throws IOException by nextToken() if it reads invalid input
     */
    private SymbolicExpression unary() throws IOException {
        SymbolicExpression result;
        int operatorNeg = st.ttype;
        String operator = st.sval;
        this.st.nextToken();
        if (operatorNeg == NEGATION || operator.equals(NEG)) {
            result = new Negation(primary());
        } else if (operator.equals(SIN)) {
            result = new Sin(primary());
        } else if (operator.equals(COS)) {
            result = new Cos(primary());
        } else if (operator.equals(LOG)) {
            result = new Log(primary());
        } else {
            result = new Exp(primary());
        }
        return result;
    }

    /**
     * Checks if the token read is a number - should always be a number in this
     * method
     * 
     * @return a SymbolicExpression to be evaluated
     * @throws IOException          by nextToken() if it reads invalid input
     * @throws SyntaxErrorException if the token parsed cannot be turned into a
     *                              valid expression,
     *                              expected a number which is not present
     */
    private SymbolicExpression number() throws IOException {
        this.st.nextToken();

        if (this.st.ttype == this.st.TT_NUMBER) {
            return new Constant(this.st.nval);
        } else {
            throw new SyntaxErrorException("Error: Expected number but got '" + this.st.ttype + "'");
        }
    }
}