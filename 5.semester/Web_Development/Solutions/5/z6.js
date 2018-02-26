//chrome://flags/#enable-javascript-harmony 
'using strict;'

window.onload = function(){
	for (var prime of take(15, primes())) {
		console.log(prime);
	}

	function* primes() {
		var seq = numbers(2);
		var prime;

		while (true) {
			prime = seq.next().value;
			yield prime;
			seq = filter(seq, prime);
		}
	}

	function* numbers(start) {
		while (true) {
			yield start++;
		}
	}

	function* take(count, seq) {
		for (var i = 0; i < count; i++) {
			yield seq.next().value;
		}
	}

	function* filter(seq, prime) {
		for (var num of seq) {
			if (num % prime !== 0) {
				yield num;
			}
		}
	}
}