//Author: Igor Tryhub, 275235

// a.js
//var B = require('./zad4b');

class A {
    createB() {
        var B = require('./zad4b');
        return new B();
    }
}
    
module.exports = A;