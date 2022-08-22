public class Queue{
    private Node first;
    private  Node last;
    private int length;


    private class Node {
        public Customer element;
        public Node next;
    }

    public Queue(){
        this.first = null;
        this.last = null;
        this.length = 0;
    }

    public String toString(){
        if(this.getLength() <= 1){
            return "";
        }
        return "@".repeat(this.length-1);   
    }
    
    public int getLength(){
        return this.length;
    }

    public void enqueue(Customer newCustomer){
        Node newNode = new Node();
        newNode.element = newCustomer;
        
        if (this.getLength() == 0) {
            this.first = newNode;
            this.last = newNode;
        } else {
            this.last.next = newNode;
        }

        this.last = newNode;
        
        this.length++;        
    }


    public Customer dequeue(){
    
        if(this.length == 0){
            throw new EmptyQueueException();
        }
    
        Node current = this.first;
        this.first = current.next;
        this.length--;
        return current.element;
    }


    public Customer firstCustomer(){
        if(this.length == 0){
            throw new EmptyQueueException();
        }
        return this.first.element;
    }

    public static void main(String args[]) {
        Queue q = new Queue();

        Customer c1 = new Customer(0, 3);
        q.enqueue(c1);
        Customer c2 = new Customer(0, 3);
        q.enqueue(c2);
        Customer c3 = new Customer(0, 3);
        q.enqueue(c3);

        System.out.println("3 == length: " + (3 == q.getLength()));
        System.out.println("c1 same: " + (c1 == q.dequeue()));
        System.out.println("c2 same: " + (c2 == q.dequeue()));
        System.out.println("1 == length: " + (1 == q.getLength()));
        System.out.println("c2 same: " + (c3 == q.dequeue()));
        System.out.println("0 == length: " + (0 == q.getLength()));
    }
}


