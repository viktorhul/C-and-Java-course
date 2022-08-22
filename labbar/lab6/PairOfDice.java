import java.util.Scanner;

public class PairOfDice {
    public Die dieOne;
    public Die dieTwo;

    public PairOfDice(int numberOfSides){
        this.dieOne = new Die(numberOfSides);
        this.dieTwo = new Die(numberOfSides);
    }

    public int rollBoth(){
        int rollOne = this.dieOne.roll();
        int rollTwo = this.dieTwo.roll();
        return rollOne + rollTwo;
    }

    public String toString(){
        return "Dice have values, " + this.dieOne.get() + " and  " + this.dieTwo.get() + ".";
    }

    public int readDie(Die die){
        return this.dieOne.get() + this.dieTwo.get();
    }

    public static void main(String [] args){
        Scanner sc = new Scanner(System.in);
        System.out.println("Number of sides: ");
        int numberOfSides = sc.nextInt();
        PairOfDice p = new PairOfDice(numberOfSides);

        System.out.println("The sum of your dices is: " + p.rollBoth());
        System.out.println(p);

        //System.out

    }
}