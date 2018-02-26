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
final class Sinus extends Operator1Arg{
    Sinus(Wyrazenie w){
        argument1 = w;
    }
    @Override public double oblicz(){
        return Math.sin(argument1.oblicz());
    }
    @Override public String toString(){
        return "(sin(" + argument1.toString() + "))";
    }
    @Override public boolean equals(Object o) {
        if (o instanceof Wyrazenie){
            return Math.sin(argument1.oblicz())==((Wyrazenie)o).oblicz();
        }
        else return false;
    }
}
