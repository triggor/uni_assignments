/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package exptrees;

/**
 *
 * @author igor
 */
final class Stala extends Wyrazenie{
    private final double value;
    public Stala (double x){
        value = x;
    }
    @Override public double oblicz(){
        return value;
    }
    @Override public String toString(){
        return "("+ value + ")";
    }
    @Override public boolean equals(Object o) {
        if (o instanceof Wyrazenie){
            return value ==((Wyrazenie)o).oblicz();
        }
        else return false;
    }
}
