n = 33;

function fib_rec(n){
    if(n==1) return 0;
    if(n==2) return 1;
    return fib_rec(n-1) + fib_rec(n-2);
}

function fib_iter(n){
    a = 0;
    b = 1;
    if(n==1) return a;
    if(n==2) return b;
    while(n>2){
        t = a;
        a = b;
        b = t + a;
        n--;
    }
    return b;
}


// for(num=10; num<=n; num++){
//     console.time("iter" + num);
//     for(r=0;r<100;r++)
//         fib_iter(num);
//     console.timeEnd("iter" + num);

//     console.time("rec" + num);
//     for(r=0;r<100;r++)
//         fib_rec(num);
//     console.timeEnd("rec" + num);
// }

for(num=10; num<=n; num++){
    var start = Date.now(); //jaka różnica między var i bez var???
    for(r=0;r<10;r++)
        fib_iter(num);
    var end = Date.now();
    var time = end-start; //jak uzyskać większą precyzję?
    console.log("iter" + num + ": " + time);

    var start = new Date().getTime();
    for(r=0;r<10;r++)
        fib_rec(num);
    var end = new Date().getTime();
    //var time = end-start;
    console.log("rec" + num + ": " + (end-start); //czemu `end-start` nie działa???
}


