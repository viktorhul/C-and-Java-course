import java.util.Scanner;
public class NameOrder {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Name1: ");
        String name1 = sc.nextLine();
        System.out.print("Name2: ");
        String name2 = sc.nextLine();

        int comp = name1.compareTo(name2);
        if (comp == 0) {
            
            System.out.println("They are the same!");

        } else if(comp < 0) {
            
            System.out.println(name1 + " is lexicographically less than " + name2);
            
        } else {

            System.out.println(name1 + " is lexicographically more than " + name2);

        }
    }
}
