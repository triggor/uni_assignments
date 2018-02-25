//Author: Igor Tryhub, 275235

var fs = require('fs');
var readline = require('readline');

var rl = readline.createInterface({
  input: fs.createReadStream('./logs.txt'),
});

function read1Stream()
{
    var buf = {};
    
    rl.on('line', function(line) { 
        var data = line.split(' ');
        if ( data.length > 1) {
            if ( data[1] in buf ) {
                buf[data[1]]++;
            }
            else buf[data[1]] = 1;
        }
    });
    rl.on('close', function() {
        var top = 3;
        for (var key in buf) {
            if (top--) {
                var value = buf[key];
                console.log(key, value);
            }
        }
    });
}

read1Stream();