//Igor Tryhub, 275235
//Lista 1, zad. 3

res = [];

for (i=2; i<=100000; i++){
    if (prime(i))
        res.push(i);
}

console.log(res);

function prime(n){
    for(j=2; j<=Math.sqrt(n); j++)
        if(n%j == 0)
            return false;
    return true;
}