"using strict";
window.onload = function() {
	var links = document.getElementsByTagName('a');
	for(var i=0; i<links.length; i++){
		//alert(i + " : " + links[i].innerHTML);
		//alert(links[i].id +'\r\n');
		links[i].onclick = drawSquare;
	}
}

function drawSquare(){
	document.getElementById('content').style.width = "100px";
	document.getElementById('content').style.height = "100px"; 
	document.getElementById('content').style.backgroundColor = this.id;  
	document.getElementById('sidebar').innerHTML += (this.id + "</br>");
}




