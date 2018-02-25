//Author: Igor Tryhub, 275235

var Obj = function(name ='DefaultName') {
    this.name = name;
    Object.defineProperty(this, "name", { 
        get: function() { 
          return "My name is " + name;       
        },        
        set: function(newName) {            
          name = newName;        
        },               
        configurable: false     
     }); 
    this.sayHi = function(){
       return "Hello, " + name + "!";
    };
}

var x = new Obj();
console.log(x.name);
x.name = "Igor";
console.log(x.name);
console.log(x.sayHi());


// Obj.prototype.sayHi = function(){
//     return 'Hiiiiii! ' + this.name;
// }
// var z = new Obj('uuu');
// console.log(z.sayHi());

var y = new Object();
//console.log(y.name);

// y.name = 'foo';
// console.log(y.name); // my name is foo

Object.prototype.sayHi = function(){
    return 'Hiiiiii! ' + this.name;
}
console.log(y.sayHi());

Object.defineProperty(y, "name", { // jak zrobic zeby nie zapetlalo sie??????
    get: function() { 
      return "My name is " + y._name;       
    },        
    set: function(newName) {            
      y._name = newName;        
    },               
    configurable: false     
 }); 
 y.name = 'Alex';
 console.log(y.name);
//console.log(y.name);