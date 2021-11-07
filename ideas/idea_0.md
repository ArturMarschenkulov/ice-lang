# Stage 1: Make a "better" C

The first step should be to basically do better (looking) C.

```
fn add(x: i32, y: i32): i32 {
    x + y
}
fn square(n: i32): i32 {
    return n * n;
}


type S = struct {
    x: i32,
    y: f32,
}
type U = union {

}

fn main(): () {
    var x: i32 = 4;
    var y := 5;
}
```


## Const/immutable by default
All declared variables are immutable by default.
```
var x := 4;
x = 4; // Error. Can't mutate an immutable variable
```

To be able to mutate a value, use the keyword `mut` after `var`.

```
var mut x := 4;
x = 4;
```
## ? Maybe function overloading

## ? Maybe Delayed initialization
Maybe, add delayed initialization. This allows to declare a typeless placeholder variable, which will be initialized only later in the same block and also given a type.

```
var x;
x: i32 = 4;
```

One may also give that variable a type.
```
var x: i32 = ...;
x := 4;

var y: i32; // Initialized with default value. If default value does not exist, this is not possible.
y := 4; // Error. Can't initialize twice
y = 4; // Error. Can't mutate an immutable variable
```




# Stage 2: More Features
## Named function parameters
```

fn pow(base: i32, exp: i32) {
    ...
}

fn main(): () {

    var res_0 = pow(5, 2);
    var res_1 = pow(base: 5, exp: 2);
    var res_2 = pow(exp: 2, base: 5);
}
```
## Operator overloading and custom operators

```
fn + (i32 x, i32 y) -> i32 {
    __add(x, y);
}
```

```
fn <*> (i32 x, i32 y) -> i32 {

    
}
```

