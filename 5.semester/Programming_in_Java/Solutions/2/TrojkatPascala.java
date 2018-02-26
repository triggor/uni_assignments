/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package newton;
import java.lang.IllegalArgumentException;
/**
 *
 * Author: Igor Tryhub, 275235
 */
public class TrojkatPascala {
    
        private final static int MAX = 10; // 10 to na pewno zbyt mało!
        private static long[][] trojkat = new long[MAX][];
        public static void TrojkatPascala(){
       // static {
       System.out.println("---");
            for (int n=1; n<=MAX; n++){// inicjalicacja tablicy trojkat
                for (int k=0; k<=n; k++){
                    if (k==0 || k==n) trojkat[n][k]=0;
                    else trojkat[n][k]=trojkat[n-1][k-1]+trojkat[n-1][k];
                    System.out.println(trojkat[n][k]);
                }
                System.out.println("---");
            }
        }

        public static long czytaj (int n, int k) throws IllegalArgumentException {
            if(0<=k && k<=n && n<=MAX) return trojkat[n][k];
            else throw new IllegalArgumentException();
        }

    }
//}
