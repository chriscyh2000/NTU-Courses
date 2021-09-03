package utils;

import java.util.Scanner;

/**
 * I recommend to use the same Scanner instance throughout the program to avoid some I/O buffering issue.
 * (e.g., `java.util.NoSuchElementException` thrown during `Scanner`'s `nextInt()` or `nextLine()`)
 */
public final class Inputs {
    public final static Scanner in = new Scanner(System.in);
}
