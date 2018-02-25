//Author: Igor Tryhub, 275235

function sum(...args) {
    var sum = 0;
    for (i of args) {
        sum += i;
    }
    console.log(sum);
}
sum(1,2,3);
// 6
sum(1,2,3,4,5);
// 15