//Igor Tryhub, 275235
//Lista 1, zad. 2

res = [];

for (i=1; i<=100000; i++){
    if (divisible(i))
        res.push(i);
}

console.log(res);

function divisible(n){
    sum = 0;
    m = n;
    do {
        digit = n%10;
        if (n%digit != 0)
            return false;
        sum += digit;
        n = Math.floor(n/10);
    } while(n>0);

    if (m % sum != 0)
        return false;
    
    return true;
}