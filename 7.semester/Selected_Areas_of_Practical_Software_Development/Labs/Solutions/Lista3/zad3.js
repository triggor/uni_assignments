//Author: Igor Tryhub, 275235

function forEach( a, f ) {
    for(var i=0; i<a.length; i++){
        f(a[i])
    }
}
function map( a, f ) {
    for(var i=0; i<a.length; i++){
        a[i] = f(a[i])
    }
    return a;
}
function filter( a, f ) {
    b=[];
    for(var i=0; i<a.length; i++){
        if(f(a[i])){
            b.push(a[i]);
        }
    }
    return b;
}
var a = [1,2,3,4];
forEach( a, _ => { console.log( _ ); } );
// [1,2,3,4]
b = filter( a, _ => _ < 3 );
console.log(b);
// [1,2]
c = map( a, _ => _ * 2 );
console.log(c);
// [2,4,6,8]
    