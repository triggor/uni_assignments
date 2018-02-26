"using strict";

window.onload = function() {
	var form = document.forms.myform;
	var inputs, i;
    validateForm();
	form.onsubmit = function(){
		if(validateAccount(form.account) && validatePesel(form.pesel) && validateDob(form.dob) && validateEmail(form.email)){
			return true;
		}
		else{
			alert("Please input correct data and try again!");
			document.getElementById('submitAlert').style.color = 'red';
			document.getElementById('submitAlert').innerHTML = 'Form validation failed. Please try again.';
			return false;
		}
	}
}
    
function validateForm(){
	inputs = document.getElementsByClassName('validable');
	for (i = 0; i < inputs.length; i++) {
		if(inputs[i].name == "account"){
			inputs[i].onchange = function() { 
				return validateAccount(this);
			};
		}
		if(inputs[i].name == "pesel"){
			inputs[i].onchange = function() { 
				return validatePesel(this);
			};
		}
		if(inputs[i].name == "dob"){
			inputs[i].onchange = function() { 
				return validateDob(this);
			};
		}
		if(inputs[i].name == "email"){
			inputs[i].onchange = function() { 
				return validateEmail(this);
			};
		}
	}
}

function validateAccount(x){
	var regexp = /^\s*[0-9]{2}\s?[0-9]{4}\s?[0-9]{4}\s?[0-9]{4}\s?[0-9]{4}\s?[0-9]{4}\s?[0-9]{4}\s*$/;
	var text;
	if(x.value.match(regexp)){
		document.getElementById('accountAlert').style.color = 'green';
		document.getElementById('accountAlert').innerHTML = 'OK';
		return true;
	}
    else{
		document.getElementById('accountAlert').style.color = 'red';
		document.getElementById('accountAlert').innerHTML = 'NOT OK';
		x.focus();
		return false;
	}
}

function validatePesel(x){
    var regexp = /^\s*[0-9]{11}\s*$/;
	var text;
	if(x.value.match(regexp)){
		document.getElementById('peselAlert').style.color = 'green';
		document.getElementById('peselAlert').innerHTML = 'OK';
		return true;
	}
	else{
		document.getElementById('peselAlert').style.color = 'red';
		document.getElementById('peselAlert').innerHTML = 'NOT OK';
		x.focus();
		return false;
	}
}

function validateDob(x){
    var regexp = /^(0[1-9]|[12][0-9]|3[01])([- /.])(0[1-9]|1[012])\2(19|20)\d\d$/; 
	var text;
	if(x.value.match(regexp)){
		document.getElementById('dobAlert').style.color = 'green';
		document.getElementById('dobAlert').innerHTML = 'OK';
		return true;
	}
	else{
		document.getElementById('dobAlert').style.color = 'red';
		document.getElementById('dobAlert').innerHTML = 'NOT OK';
		x.focus();
		return false;
	}
}

function validateEmail(x){
    var regexp = /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/; 
	var text;
	if(x.value.match(regexp)){
		document.getElementById('emailAlert').style.color = 'green';
		document.getElementById('emailAlert').innerHTML = 'OK';
		return true;
	}
	else{
		document.getElementById('emailAlert').style.color = 'red';
		document.getElementById('emailAlert').innerHTML = 'NOT OK';
		x.focus();
		return false;
	}
}