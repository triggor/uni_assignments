/*window.onload = function() {
	var elem = document.getElementById('par');
	elem.onclick = runE;
}
function runE(){
	document.getElementById('par').innerHTML = 'Fred Flinstone';
}
*/
//"using strict";
var Vehicle = function(Xcoord, Ycoord, consumption, tank){
	this.currentPosition = {
		X: Xcoord,
		Y: Ycoord
	},
	this.fuelConsumption = consumption,
	this.tankStatus = tank,
	this.move = function(x,y){
		distance = Math.sqrt(x*x + y*y);
		if(distance*this.fuelConsumption <= this.tankStatus){
			this.currentPosition.X += x;
			this.currentPosition.Y += y;
			this.tankStatus -= distance*this.fuelConsumption;
			alert("Moved " + distance + " km");
			alert("New position: " + this.currentPosition.X + ", " + this.currentPosition.Y);
		}
		else alert("Not enough fuel!");
	},
	this.loadFuel = function(fuelAmount){
		this.tankStatus += fuelAmount;
		alert("Loaded fuel (" + fuelAmount + "L)!");
		alert("Now tank volume is " + this.tankStatus + "L");
	}
	this.getTankStatus = function(){
		return this.tankStatus;
	}
};

var Car = new Vehicle(2,3,0.1,10);
var Truck = new Vehicle(5,5,0.3,20);

Car.move(4,3);
Truck.move(40,30);
Truck.move(40,30);
alert("Truck tank: " + Truck.getTankStatus());
Truck.loadFuel(50);
alert("Truck tank: " + Truck.getTankStatus());
Truck.move(40,30);
Truck.move(40,30);
Truck.move(40,30);
alert("Car tank: " + Car.getTankStatus());
alert("Truck tank: " + Truck.getTankStatus());
Truck.move(40,30);



