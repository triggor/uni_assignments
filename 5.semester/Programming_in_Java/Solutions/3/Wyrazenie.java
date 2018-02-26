/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package exptrees;

/**
 *
 * @author Igor Tryhub, 275235
 */
abstract class Wyrazenie {
    abstract public double oblicz();
    /** metoda sumująca wyrażenia */
    public static double sumuj (Wyrazenie... wyr) {
        double result = 0;
        for (Wyrazenie wyr1 : wyr) {
            result += wyr1.oblicz();
        }
        return result;
    }
    /** metoda mnożąca wyrażenia */
    public static double pomnoz (Wyrazenie... wyr) {
        if (wyr.length == 0) return 0;
        double result = 1;
        for (Wyrazenie wyr1 : wyr) {
            result *= wyr1.oblicz();
        }
        return result;
    }

}
