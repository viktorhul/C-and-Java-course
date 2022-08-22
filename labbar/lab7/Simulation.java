import java.util.Random;
import java.util.ArrayList;

public class Simulation {
    private Store store;
    private int time;
    private int intensity;
    private int maxGroceries;
    private int thresholdForNewRegister;

    //Statistics
    private int customersServed;
    private int maxWaitTime;
    private int avgWaitTime;
    
    private int totalCustomerTime; 

    Random random = new Random();


    public Simulation(int intensity, int maxGroceries, int thresholdForNewRegister) {
        int noRegisters = 5;
        if (maxGroceries < 1) {
          maxGroceries = 1;
        }
        this.store = new Store(noRegisters);
        this.intensity = intensity;
        this.maxGroceries = maxGroceries;
        this.thresholdForNewRegister = thresholdForNewRegister;
        this.time = 0;

        this.customersServed = 0;
        this.maxWaitTime = 0;
        this.avgWaitTime = 0;
        this.totalCustomerTime = 0;
    }

    public String toString(){
      String res = "SIMULATION step: " + this.time + "\n";
      res += this.store.toString();
      res += "\nNumber of customers served: " + String.valueOf(this.customersServed);
      res += "\nMax waiting time: " + String.valueOf(this.maxWaitTime);
      res += "\nAverage waiting time: " + String.valueOf(this.avgWaitTime);
      return res;
    }



    public void step() {
      
      int inten = this.random.nextInt(100); 
      
      if (inten < this.intensity) {
        int noGroceries = this.random.nextInt(this.maxGroceries) + 1;        
        Customer c = new Customer(this.time,noGroceries);
        
        this.store.newCustomer(c);
      }
      
      if(this.store.getAverageQueueLength() > this.thresholdForNewRegister){
          this.store.openNewRegister();
      }


      this.store.step();
      this.time++;
      
      ArrayList <Customer> finishedCustomers = this.store.getDoneCustomers();

      for(Customer c : finishedCustomers){
        
        int customerTime = this.time - c.getBorntime();
        
        this.totalCustomerTime += customerTime;
        
        this.customersServed++;
        
        this.maxWaitTime = Math.max(customerTime, this.maxWaitTime);

      }
      if (this.customersServed == 0) {
        this.avgWaitTime = 0;
      } else {
        this.avgWaitTime = this.totalCustomerTime/this.customersServed;
      }

    }

    public static void main(String[] args){

    }
}
