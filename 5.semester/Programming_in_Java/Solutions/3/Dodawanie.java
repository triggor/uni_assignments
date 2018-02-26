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
final class Dodawanie extends Operator2Arg{
    Dodawanie(Wyrazenie w1, Wyrazenie w2){
        argument1 = w1;
        argument2 = w2;
    }
    @Override public double oblicz(){
        return argument1.oblicz() + argument2.oblicz();
    }
    @Override public String toString(){
        return "(" + argument1.toString() + "+" + argument2.toString() + ")";
    }
    @Override public boolean equals(Object o) {
        if (o instanceof Wyrazenie){
            return (argument1.oblicz() + argument2.oblicz())==((Wyrazenie)o).oblicz();
        }
        else return false;
    }
}
