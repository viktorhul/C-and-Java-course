import java.util.Scanner;

public class MyDieTest {

    public static void main(String [] args) {
        Scanner sc = new Scanner(System.in);
        
        System.out.print("Number of sides: ");
        
        int sides = sc.nextInt();
        Die myDie = new Die(sides);

        System.out.println(myDie);
        
        int dieResult;
        int sum = 0;
        System.out.println("Throwing ten dice");
        
        for(int i = 1; i <= 10; i++)
        {
            dieResult = myDie.roll();
            sum += dieResult;
        }
        
        System.out.println("Their sum was " + sum);

        Die die1 = new Die(6);
        Die die2 = new Die(6);

        die1.cheat(1);
        die2.cheat(1);

        System.out.println(die1.get());
        System.out.println(die2.get());

        System.out.println(die1.equals(die2));
        
        
    }
}