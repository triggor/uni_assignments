//Author: Igor Tryhub, 275235

function* take(it, top) {
    var counter = 0;
    while (true) {
        if (counter < top) {
            yield it.next().value;
            counter++;
        }
        //else break; //nic nie wnosi???????????????????????????????
    }
}

// zwróć dokładnie 10 wartości z potencjalnie
// "nieskończonego" iteratora/generatora
for (let num of take( fib(), 10 ) ) {
    console.log(num);
}

function* fib() {
    var fn1 = 0;
    var fn2 = 1;
    while (true) {  
        var current = fn1;
        fn1 = fn2;
        fn2 = current + fn1;
        var reset = yield current;
        if (reset) {
            fn1 = 0;
            fn2 = 1;
        }
    }
}