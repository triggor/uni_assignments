/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package exptrees;

import java.util.Objects;

/**
 *
 * @author Igor Tryhub, 275235
 */
public class Para {
    public final String key;
    private double value;
    public Para(String key, double value){
        this.key = key;
        this.value = value;
    }
    public double getKey(){
        return value;
    }
    public void setKey(double val){
        value = val;
    }
    @Override public String toString(){
        return "{" + key + ", " + value + "}";
    }
    /*???????????????????????????????????????????????????????????????????????????????????????*/
    @Override public int hashCode(){ int hash = 7;
        hash = 97 * hash + Objects.hashCode(this.key);
        return hash;
    }
    @Override public boolean equals(Object o) {
        //generate missing hashCode() ?????
        //Implementation of .equals methods not checking the type of the input parameter ????
        if (o instanceof Para){
            return key.equals(((Para)o).key);
        }
        else return false;
    }


}
