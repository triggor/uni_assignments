//Author: Igor Tryhub, 275235

var fs = require('fs');
// const util = require('util');

// function printAsync(path) {
//     fs.readFile(path, 'utf8', (err, data) => {
//         if(err) throw err;
//         console.log(data);
//     });
// }
// printAsync('logs.txt');

// function fspromise(path) {
//     return new Promise( function(res, rej) {
//         fs.readFile(path, function(err, data) {
//             if(err)
//                 rej(err);
//             res( data.toString() );
//         });
//     } );
// }
// fspromise('test.txt')
//     .then(function(data) {
//         console.log(data);
//     });

// const readpromise = util.promisify(fs.readFile);

// readpromise('logs.txt').then((data) => {
//     console.log(data.toString('utf8'));
// }).catch((err) => {
//     throw err;
// });

// async function callRead() {
//   const data = await readpromise('test.txt');
//   console.log(data.toString('utf8'));
// }
// callRead();

// fspromise('test.txt')
//     .then(function(data) {
//         fspromise('logs.txt')
//         .then(function(data2) {
//             console.log(data2);				
//             console.log(data);				
//         });
//     });

// fspromise('logs.txt')
//     .then(function(data) {
//         console.log(data);				
//         return fspromise('test.txt')
//         .then(function(data2) {
//             console.log(data2);		
//             })
//     });

// var f1 = fspromise('logs.txt');
// var f2 = fspromise('test.txt');
// var f3 = Promise.all([f2,f1])
//     .then(function([data, data2]) {
//         console.log( data );
//         console.log( data2 );
//     } );				

// var p1 = fspromise('logs.txt');
// var p2 = fspromise('test.txt');
// function promiseall(f1, f2) {
//     return p1.then(function(p1r) {
//         return p2.then(function(p2r) {
//             return new Promise( function(res, rej) {
//                 res([p1r,p2r]);
//             });
//         });
//     });
// }	
// var p3 = promiseall(p1, p2);
// p3.then(function([data, data2]) {
//     console.log( data );
//     console.log( data2 );
// } );

// var http = require("http");
// // funkcja oznakowana jako async
// // zwraca Promise
// function promisedGet(url) {
//     return new Promise(function (resolve, reject) {
//         var client = http.get(url, function (res) {
//             var buffer = '';
//             res
//                 .on('data', function (data) {
//                     buffer += data.toString();
//                 })
//                 .on('end', function () {
//                     resolve(buffer);
//                 });
//         });
//     });
// }
// // funkcja oznakowana jako async, 
// // wolno w niej wywoływać inną funkcję zwracającą Promise przez await
// (async function() {
//     // await ___ ...
//     var result = await promisedGet('http://www.google.pl');
//     console.log( result );
//     fs.writeFile("google.html", result, function(err) {
//         if(err) {
//             return console.log(err);
//         }
//         console.log("The file was saved!");
//     }); 
//     // jest równoważne dzisiejszemu
//     //
//     // promisedGet('http://www.google.pl')
//     //   .then( function(result) { console.log( result );} );
//     // 
//     // a umieszczeniem całego kodu napisanego "pod" await w kontynuacji 
//     // doczepionej do Promise przez then zajmuje się już sam kompilator
// })();

var util = require('util'); //?????????????????????????????????????????????????????????????????????????????
// var readFileAsync = util.promisify(fs.readFile);
// console.log((async function() {

//     var data1 = await readFileAsync('logs.txt');
//     var data2 = await readFileAsync('test.txt');
//     return data1;
                
// })());

function fetchX(cb) {
    var result = 1;
    cb(1);
}

function fetchY(cb) {
    setTimeout( () => {
        cb(2);
    }, 1000);
}

function add(getX,getY,cb) {//????????????????????????????????????????????????????????????????????????????
	var x, y;
	getX( function(xVal){
		x = xVal;
		// both are ready?
		if (y != undefined) {
			cb( x + y );	// send along sum
		}
	} );
	getY( function(yVal){
		y = yVal;
		// both are ready?
		if (x != undefined) {
			cb( x + y );	// send along sum
		}
	} );
}
// `fetchX()` and `fetchY()` are sync or async
// functions
add( fetchX, fetchY, function(sum){
	console.log( sum ); // that was easy, huh?
} );

