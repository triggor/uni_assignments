//Author: Igor Tryhub, 275235

// b.js
//var A = require('./zad4a');

class B {
    createA() {
        var A = require('./zad4a');
        return new A();
    }    
}

module.exports = B;