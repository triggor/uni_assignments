//Author: Igor Tryhub, 275235

var readline = require('readline');
var rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
  terminal: false
});

// rl.on('line', function(line){
//     console.log(line);
// })

rl.question('What is your name? \n', (answer) => {
    console.log(`Hello ${answer}!`);
    rl.close();
});