//Author: Igor Tryhub, 275235

var fs = require('fs');

function read1Stream()
{
    var buf ='';
    var s = fs.createReadStream( './test.txt', { encoding :'utf-8' } );
    s.on( 'data', function(data) { //error handling???????????????????????????
        buf += data.toString();
    });
    s.on('error', err => {

    });
    s.on('end', function() {
        console.log( buf );
    });
}

read1Stream();

fs.readFile('log.txt', 'utf-8', (err, data) => {

});

