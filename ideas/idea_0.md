- [Stage 0: Big picture](#stage-0-big-picture)
- [First steps: Making a "better" C](#first-steps-making-a-better-c)
  - [Const/immutable by default](#constimmutable-by-default)
  - [? Maybe function overloading](#-maybe-function-overloading)
  - [? Maybe Delayed initialization](#-maybe-delayed-initialization)
- [Features](#features)
  - [Named function parameters](#named-function-parameters)
  - [Operator overloading and custom operators](#operator-overloading-and-custom-operators)
  - [Dynamic numerical base](#dynamic-numerical-base)
  - [Switch statements](#switch-statements)
  - [Complex Types](#complex-types)
    - [Struct](#struct)
    - [Union](#union)
    - [C-style Union](#c-style-union)
    - [Enum](#enum)
    - [Bitmask](#bitmask)
    - [Trait](#trait)
      - [Names for traits in other languages](#names-for-traits-in-other-languages)
  - [Polymorphism](#polymorphism)
    - [Parametric Polymorphism](#parametric-polymorphism)
- [Code Examples](#code-examples)
  - [Stack](#stack)
  - [Hash](#hash)

# Stage 0: Big picture

Ice is a hobby project, where a clean C++-like language is created, which in addition to that draws many ideas from various languages like Swift, Rust, Raku, Haskell, Ocaml, etc.


# First steps: Making a "better" C

The first step should be to basically do better (looking) C. In this stage I keep everything very minimal and basically create a in my eyes better C.

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




# Features
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
postfix
infix
suffix
circumfix?

in swift

precedence group

11 _ BitwiseShiftPrecedence
10 l MultiplicationPrecedence 
09 l AdditionPrecedence
08 _ RangeFormationPrecedence
07 l CastingPrecedence
06 r NilCoalescingPrecedence
05 _ ComparisonPrecedence
04 l LogicalConjunctionPrecedence 
03 l LogicalDisjunctionPrecedence 
02 r TernaryPrecedence 
01 r AssignmentPrecedence 

fn<*> postfix (i32 x, i32 y) -> i32 {


}
```

## Dynamic numerical base

```
var x := 10b3;
```


## Switch statements

```
fn foo(x: i32) -> i32 {

    var x := given x {
        _: 32
    }
    
}
```
## Complex Types
### Struct

```
type Person := struct {
    age: u32,
};

var person = struct {
    age: u32,
} = { 
    .age = 30,
};
```
### Union
### C-style Union

### Enum
```
type Color = enum u32 {
    Red,
    Blue,
    Yellow,
};
```
### Bitmask
### Trait
#### Names for traits in other languages
Rust: ``trait``
Swift: ``protocol`` ``extension``
C++: ``concept``
Perl: ``role``
Kotlin: ``interface``
Haskell: ``typeclass``




```

```

## Polymorphism
### Parametric Polymorphism

```
type Stack = struct {

    
}
```



# Code Examples
## Stack


```
type Stack<T> = struct {
    items: [T] = [],
}

impl Stack<T> {
    fn push(self, item: T): () {
        self.items.append(item);
    }

}
```

## Hash

```
fn djb2(data: []u8): u32 {
    var hash: u32 = 5381;
    for var i := 0; i < data.size(); i++ {
        hash = (hash << 5) + hash + data[i] as u32;
    }
    hash
}
```
