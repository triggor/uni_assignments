//Author: Igor Tryhub, 275235

"using strict";

var attempts = 15;
var guessed = 0;

class Card {
    backPath: string; //="back.png";
    public static className: string;
    public facePath: string;
    constructor(path) {
        this.backPath = "back.png";
        this.facePath = path;
        this.className = "card";
    }

}
let ccc = new Card("hahah");
console.log(ccc.backPath); // error

var cards = [];
var faces = [];
var pickedPair = [null, null];

window.onload = function() {
	for(var x=1; x<5; x++) { 
        for(var y=1; y<6; y++) {
            document.getElementById('gamePanel').innerHTML += '<div class="field"><span><img src="back.png" class="card"></span></div>';
        }
        document.getElementById('gamePanel').innerHTML += '</br>';
    }
	document.getElementById('attempts').innerHTML = String(attempts);
	document.getElementById('guessed').innerHTML = String(guessed);
	init();
	play();
}

function init(){
    cards = document.getElementsByClassName('card');
	for(var i=0; i<cards.length; i++){
		faces[i]=null;
	}
	for(var i=0; i<cards.length/2; i++){
		var paired = 0;
		while(paired < 2){
			var number = Math.floor(Math.random() * cards.length);
			if(faces[number] == null){
				faces[number] = i+1+'.jpg';
				paired++;
			}
		}
	}
}

function processClick(index){
	return function(){
		if(pickedPair[0] === null){
			pickedPair[0] = index;
			cards[index].src = faces[index];
		}
		else if(index !== pickedPair[0] && pickedPair[1] === null){
			pickedPair[1] = index;
			cards[index].src = faces[index];
			setTimeout(function(){
				if(faces[pickedPair[0]] === faces[pickedPair[1]]){
					cards[pickedPair[0]].parentNode.setAttribute("class", "guessed");
					cards[pickedPair[1]].parentNode.setAttribute("class", "guessed");
					document.getElementById('guessed').innerHTML = String(++guessed);
				}
				else{
					cards[pickedPair[0]].src = 'back.png';
					cards[pickedPair[1]].src = 'back.png';
					document.getElementById('attempts').innerHTML = String(--attempts);
				}
				if(attempts === 0 && guessed < 10) youLost();
				if(guessed === 10) youWon();
				pickedPair = [null, null];
			}, 1500);
		}
	}
}

function play(){
	for(var i=0; i<cards.length; i++){
		cards[i].onclick = processClick(i);
	}
}

function youWon(){
	setTimeout(function() { alert("Good job! You won in " + (15-attempts) + " attempts!"); }, 1000);
	setTimeout(function() { location.reload(); }, 2000);
}

function youLost(){
	for(var i=0; i<cards.length; i++){
		cards[i].src = faces[i];
	}
	setTimeout(function() { alert("Sorry, you lost... \nMaybe you'll get luckier next time!"); }, 1500);
	setTimeout(function() { location.reload(); }, 2000);
}
