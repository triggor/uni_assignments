//Author: Igor Tryhub, 275235

var Tree = function(value, left=undefined, right=undefined) {
    this.value = value;
    this.left = left;
    this.right = right;
}

var root = new Tree(8, new Tree(6, new Tree(1), new Tree(7)), new Tree(10, new Tree(9), new Tree(12)));

Tree.prototype[Symbol.iterator] = function* () {
    /*var stack = [this];
    while (stack.length > 0) {
        var node =  stack.pop();
        yield node.value;
        if(node.right !== undefined) {
            stack.push(node.right)
        }
        if(node.left !== undefined) {
            stack.push(node.left)
        }
    } 
    */
    if (this.left)
        yield* this.left;
    yield this.value;
    if( this.right)
        yield* this.right;
};

// enumeracja wartości z węzłów
for ( var e of root ){
    console.log( e );
}






