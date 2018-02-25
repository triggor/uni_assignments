//Author: Igor Tryhub, 275235

function genWrapper(n=10) {
    return function createGenerator() {
        var _state = 0;
        return {
            next : function() {
                return ({
                    value : _state,
                    done : _state++ >= n
                })
            }
        }
    } 
}  

// var foo = {
//     [Symbol.iterator] : createGenerator
// };

// for ( var f of foo )
//     console.log(f);

var foo1 = {
    [Symbol.iterator] : genWrapper(8)
};
for ( var f of foo1 )
    console.log(f);

var foo2 = {
    [Symbol.iterator] : genWrapper() 
};
for ( var f of foo2 )
    console.log(f);