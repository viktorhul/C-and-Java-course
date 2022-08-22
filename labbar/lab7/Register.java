public class Register {
    private Boolean open;
    Queue queue;

    public Register(){
        this.open = false;
        this.queue = new Queue();
    }

    public String toString(){
        if(this.open){
            return (this.queue.getLength() == 0) ? "  [ ]" : "  [" + this.queue.firstCustomer() + "] " + this.queue;  
        }
        else{
            return "X [ ]";
        }
    }

    public void open(){
        this.open = true;
    }

    public void close(){
        this.open = false;
    }

    public Boolean isOpen(){
        return this.open ;
    }

    public void step(){
        if (this.isOpen() && (0 < this.queue.getLength())) {
            Customer currentCustomer = this.queue.firstCustomer();
            
            currentCustomer.serve();
        }
    }


    public Boolean hasCustomer(){
        return this.queue.getLength() > 0;
    }

    public Boolean currentCustomerIsDone(){
        return this.queue.firstCustomer().isDone();
    }

    public void addToQueue(Customer c){
        this.queue.enqueue(c);
    }

    public int getQueueLength(){
        return this.queue.getLength();
    }


    public Customer removeCurrentCustomer(){
        return this.queue.dequeue();
    }

    public static void main(String[] args) {
        Register reg = new Register();
        for (int index = 0; index < 5; index++) {
            Customer newCustomer = new Customer(0, 1);
            reg.addToQueue(newCustomer);
        }
            
            System.out.println("queue.length == 5: " + (reg.queue.getLength() == 5));
            reg.step();
            System.out.println("queue.length == 4: " + (reg.queue.getLength() == 4));
            reg.step();
            System.out.println("queue.length == 3: " + (reg.queue.getLength() == 3));
            reg.step();
            System.out.println("queue.length == 2: " + (reg.queue.getLength() == 2));
            reg.step();
            System.out.println("queue.length == 1: " + (reg.queue.getLength() == 1));
            reg.step();
            System.out.println("queue.length == 0: " + (reg.queue.getLength() == 0));
            reg.step();
            
            //reg.step();
            //System.out.println("Customer #" + index + "")
        
    }
}
