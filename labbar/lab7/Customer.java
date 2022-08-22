public class Customer{
    private int borntime;  
    private int groceries;

    public int getBorntime(){
        return this.borntime;
    }
    
    public Customer(int borntime, int groceries){
        if (borntime < 0) {
            this.borntime = 0;
        }
        
        else if (groceries < 0) {
            this.groceries = 0;
        }
        else {
        this.borntime = borntime;
        this.groceries = groceries;
        }
    }
    
    public String toString(){
        return String.valueOf(this.groceries);
    }


    public void serve(){
        this.groceries--;   
    }

    public Boolean isDone(){
        return (this.groceries == 0);
    }

    public static void main(String[] args) {
        Customer customer = new Customer(10, -10);

        while (!customer.isDone()) {
            System.out.println("Customer has " + customer.groceries + " left");
            customer.serve();
        }
        
        System.out.println("Customer is done!");
    }
}


