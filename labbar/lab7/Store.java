import java.util.ArrayList;

public class Store {
    private Register[] registers;
    private int noRegisters;

    public Store(int noRegisters){
        this.noRegisters = noRegisters;
        this.registers = new Register[noRegisters];

        for(int i = 0; i < noRegisters; i++){
          this.registers[i] = new Register(); 
        }
        
        this.registers[0].open();
    }
    

    public String toString(){
      String res = "\n";
        for(Register r : this.registers){
          res += r.toString() + "\n";
        }
        return res;
    }


    public int getAverageQueueLength() {
        int sum = 0;
        int openRegisters = 0;
        for(Register r : this.registers){
          
          if(r.isOpen()){
            openRegisters++;
          }

          sum += r.queue.getLength();
        }
        System.out.println("Number of people in queque: " + sum);
        System.out.println("Open registers: " + openRegisters);
        System.out.println("Current load: " + sum / openRegisters);

        return sum / openRegisters;
    }


    public void newCustomer(Customer c) {
        Register shortestQueue = getShortestQueue();
        shortestQueue.addToQueue(c);
    }  
    

    private Register getShortestQueue() {
       Register register = this.registers[0];

        for (Register r : this.registers) {
          if(r.isOpen()){
            if (register.getQueueLength() > r.getQueueLength())
                register = r; 
          }
        }

        return register;
    }


    public void step() {
      for (Register r : this.registers) {       
        r.step();
      }
    }
    

    public void openNewRegister() {
        for(Register r : this.registers){
            if (!r.isOpen()) {
                r.open();
                break;
            }
        }
    }


    public ArrayList <Customer> getDoneCustomers() {
      ArrayList<Customer> doneCustomers = new ArrayList<Customer>();

      for (Register r : this.registers) {
        if(r.queue.getLength() > 0){
          if (r.currentCustomerIsDone()) {
              doneCustomers.add(r.removeCurrentCustomer());
          }
        }
      }
      return doneCustomers;
    }

    public static void main(String[] args) {
      System.out.println("@".repeat(0));
      System.out.println("@".repeat(1));
      System.out.println("@".repeat(2));
    }
}







