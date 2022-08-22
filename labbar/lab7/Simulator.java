public class Simulator{
   
    public static void main(String[] args) throws InterruptedException{
        int steps = 100;
        
        int intensity = 100;
        int maxGroceries = 7;
        int thresholdForNewRegister = 5;

        Simulation s = new Simulation(intensity,maxGroceries,thresholdForNewRegister); 

        for(int i = 0; i < steps; i++){
            System.out.println("\nStep" + i);
            System.out.print("\033[2J\033[;H");
            s.step();
            System.out.println(s);
            Thread.sleep(500);
        }
        System.out.println("");
        System.out.println("Simulation finished!");
    }
}