function getLastProto(o) {
  var p = o;
  do {
    o = p;
    p = Object.getPrototypeOf(o);
  } while (p);

  return o;
}
var a = {i:8.99};
console.log(getLastProto(a));