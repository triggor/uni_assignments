//Author: Igor Tryhub, 275235

function foo() {
    
}

function foo()
{
    
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

function fib() {
    var fn1 = 0;
    var fn2 = 1;
    return {
        next : function() {
            var current = fn1;
            fn1 = fn2;
            fn2 = current + fn1;

            return {
                value : current,
                done : false
            }
        }
    }
}
    
var _it = fib(); //automatycznie bierze definicje generatora??????????
for ( var _result; _result = _it.next(), !_result.done; ) {
    console.log( _result.value );
}

for ( var i of fib() ) {
    console.log( i );
}
    