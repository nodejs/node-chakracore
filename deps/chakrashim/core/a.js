var a = 'a', b = 'b';
function foo() {
	function bar() {
		function baz() {
			return 'something';
		}
		return b + baz();
	}
	return a + bar();
}
function foo2() {
	function bar2() {
		var o = {
			m() { return 'm'; },
			a() { return a; },
			b() { return b; }
		};
		function baz2() {
			return o.a();
		}
		function baz3() {
			return o.b();
		}
		return o.m() + baz2() + baz3();
	}
	function bar3() {
		return a + b;
	}
	return bar2() + bar3() + a;
}
console.log(foo() + foo2());