//Author: Igor Tryhub, 275235

n = 35;

function fib_rec(n){
    if(n==1) return 0;
    if(n==2) return 1;
    return fib_rec(n-1) + fib_rec(n-2);
}

var memo = {};

function fib_mem(n){
    if(n in memo) return memo[n];
    if(n==1) return 0;
    if(n==2) return 1;
    memo[n] = fib_rec(n-1) + fib_rec(n-2);
    return memo[n];
}

for(num=10; num<=n; num++){
    var start = new Date().getTime();
    for(r=0;r<10;r++)
        fib_rec(num);
    var end = new Date().getTime();
    console.log("rec" + num + ": " + (end-start));

    start = new Date().getTime();
    for(r=0;r<10;r++)
        fib_mem(num);
    end = new Date().getTime();
    console.log("mem" + num + ": " + (end-start)); 
}



