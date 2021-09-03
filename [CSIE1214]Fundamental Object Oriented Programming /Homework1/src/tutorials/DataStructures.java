package tutorials;

/**
 * Some basic data structure implementation in Java.
 * You can also learn Generic Type <T> here.
 */
class Stack<T> {
    private final int MAX_SIZE;
    private int top = -1;
    private final Object[] elements;

    Stack(int MAX_SIZE) {
        this.MAX_SIZE = MAX_SIZE;
        elements = new Object[MAX_SIZE];
    }

    public boolean isFull() {
        return top + 1 == MAX_SIZE;
    }

    public boolean isEmpty() {
        return top == -1;
    }

    public void push(T item) {
        if (isFull()) {
            throw new IllegalStateException("Full");
        }
        elements[++top] = item;
    }

    @SuppressWarnings("unchecked")
    public T pop() {
        if (isEmpty()) {
            throw new IllegalStateException("Empty");
        }
        return (T) elements[top--];
    }
}


class LinkedListNode<T> {
    public T value;
    public LinkedListNode<T> next;

    public LinkedListNode(T value) {
        this.value = value;
    }
}

class LinkedList<T> {
    private LinkedListNode<T> head;
    private LinkedListNode<T> tail;

    public void invert() {
        LinkedListNode<T> left, right = null, p = head;

        tail = head;
        while (p != null) {
            left = right;
            right = p;
            p = p.next;
            right.next = left;
        }
        head = right;
    }

    public void addHead(T item) {
        LinkedListNode<T> node = new LinkedListNode<>(item);
        if (head == null) {
            head = tail = node;
        }
        node.next = head;
        head = node;
    }

    public void addTail(T item) {
        LinkedListNode<T> node = new LinkedListNode<>(item);
        if (head == null) {
            head = tail = node;
        }
        tail.next = node;
        tail = node;
    }

    public T deleteHead() {
        T item = head.value;
        head = head.next;
        return item;
    }

    public T deleteTail() {
        T item = tail.value;
        LinkedListNode<T> previous = head;
        while (previous.next != tail) {
            previous = previous.next;
        }
        tail = previous;
        return item;
    }

}

public class DataStructures {
    public static void main(String[] args) {
        Stack<Integer> stack = new Stack<>(5000);
        stack.push(1);
        stack.push(2);
        stack.push(3);

        System.out.println(stack.pop());  // 3
        System.out.println(stack.pop());  // 2
        System.out.println(stack.pop());  // 1


        LinkedList<Integer> linkedList = new LinkedList<>();
        linkedList.addHead(0);
        linkedList.addHead(-1);
        linkedList.addTail(2);

        System.out.println(linkedList.deleteHead());  // -1
        System.out.println(linkedList.deleteTail());  // 2
        System.out.println(linkedList.deleteHead());  // 0
    }
}
