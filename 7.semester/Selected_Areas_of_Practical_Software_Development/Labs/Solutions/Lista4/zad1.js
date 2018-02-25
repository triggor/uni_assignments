//Author: Igor Tryhub, 275235

var Tree = function(value, left=undefined, right=undefined) {
    this.value = value;
    this.left = left;
    this.right = right;
}

var tree = new Tree(8, new Tree(6, new Tree(1), new Tree(7)), new Tree(10, new Tree(9), new Tree(12)));

console.log(tree)