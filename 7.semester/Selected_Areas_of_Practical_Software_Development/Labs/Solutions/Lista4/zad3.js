//Author: Igor Tryhub, 275235

function Foo() {
    function Qux() {
        console.log('private');
    }
    return {
        Bar : function() {
            console.log('public');
            Qux();
        }
    }
}

var obj = new Foo();
obj.Bar();
obj.Qux(); //TypeError: obj.Qux is not a function