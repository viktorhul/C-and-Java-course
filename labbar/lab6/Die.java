import java.util.Scanner;

import javax.lang.model.util.ElementScanner6;

public class Die {
  private int numberOfSides;
  private int value;

  public Die() {
      this.numberOfSides = 6;
      this.roll();      
  }

  public Die(int numberOfSides) {
     if (numberOfSides > 0) {
       this.numberOfSides = numberOfSides;
  
     } else {
       throw new IllegalArgumentException("Illegal nuber of sides!");
     }
  }


  public String toString(){
    return "Die(" + this.value + ")";
  }


  public void cheat(int val){
    this.value = val;
  }


  boolean equals(Die otherDie){
    return this.get() == otherDie.get();
  }


  public int roll() {
    this.value = (int) (Math.random() * numberOfSides) + 1;
    return this.get();
  }


  public int get() {
      return this.value;   
  }

  
  public static void main(String [] args) {
      Scanner sc = new Scanner(System.in);
      System.out.print("Number of sides: ");
      int sides = sc.nextInt();
      Die d = new Die(sides);
      System.out.println("Alea iacta est: " + d.roll());
  }
}