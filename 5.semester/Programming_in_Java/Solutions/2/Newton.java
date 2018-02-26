/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package newton;

/**
 *
 * @Author: Igor Tryhub, 275235
 */
public class Newton {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        //TrojkatPascala triangle = new TrojkatPascala();
        //System.out.println(triangle.czytaj(5,3));// TODO code application logic here
    readInput(args);
    System.out.println(writeOutput(args));
    }
    
    
            
    private final static int MAX = 67; // 10 to na pewno zbyt mało!
    private static long[][] trojkat = new long[MAX][MAX];
    
    static {
        for (int n=1; n<MAX; n++){// inicjalicacja tablicy trojkat
            for (int k=0; k<=n; k++){
                if (k==0 || k==n) trojkat[n][k]=1;
                else trojkat[n][k]=trojkat[n-1][k-1]+trojkat[n-1][k];
            }
        }
    }

    public static long czytaj (int n, int k) throws IllegalArgumentException {
        if(0<=k && k<=n && n<=MAX) return trojkat[n][k];
        else throw new IllegalArgumentException();
    }
    private static String sign;
    private static String first;
    private static String second;
    private static void readInput(String[] args){
        int p = args[0].indexOf('+');
        if (p==-1) p = args[0].indexOf('-');
        sign = args[0].substring(p,p+1);
        first = args[0].substring(0,p);
        second = args[0].substring(p+1);
        //throw Exc;
    }
    private static String writeOutput(String[] args){
        int n = new Integer (args[1]);
        String res = "("+first+sign+second+")^"+n+" = ";
        for (int k=0; k<=n; k++){
            String nextSign;
            if ((n-k)%2==0) nextSign = "+";
            else nextSign = sign;
            if (k==0) res += first + "^" + n + " " + nextSign + " ";
            else if (k==n) res += second + "^" + n;
            else if (k==1) res += trojkat[n][k] + first + "^" + (n-k) + "*" + second + " " + nextSign + " ";
            else if (n-k==1) res += trojkat[n][k] + first + "*" + second + "^" + k + " " + nextSign + " ";
            else res += trojkat[n][k] + first + "^" + (n-k) + "*" + second + "^" + k + " " + nextSign + " ";
        }
        return res;
    }
    
}
