- [1. Stage 0: Big picture](#1-stage-0-big-picture)
- [2. Stage 1: Make a "better" C](#2-stage-1-make-a-better-c)
  - [2.1. Const/immutable by default](#21-constimmutable-by-default)
  - [2.2. ? Maybe function overloading](#22--maybe-function-overloading)
  - [2.3. ? Maybe Delayed initialization](#23--maybe-delayed-initialization)
- [3. Stage 2: More Features](#3-stage-2-more-features)
  - [3.1. Named function parameters](#31-named-function-parameters)
  - [3.2. Operator overloading and custom operators](#32-operator-overloading-and-custom-operators)
  - [3.3. Dynamic numerical base](#33-dynamic-numerical-base)
  - [3.4. Switch statements](#34-switch-statements)
  - [Structs](#structs)
  - [3.5. Traits](#35-traits)
    - [3.5.1. Names for traits in other languages](#351-names-for-traits-in-other-languages)

# 1. Stage 0: Big picture

Ice is a hobby project, where a clean C++-like language is created, which in addition to that draws many ideas from various languages like Swift, Rust, Raku, Haskell, Ocaml, etc.


# 2. Stage 1: Make a "better" C

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


## 2.1. Const/immutable by default
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
## 2.2. ? Maybe function overloading

## 2.3. ? Maybe Delayed initialization
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




# 3. Stage 2: More Features
## 3.1. Named function parameters
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
## 3.2. Operator overloading and custom operators

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

## 3.3. Dynamic numerical base

```
var x := 10b3;
```


## 3.4. Switch statements

```
fn foo(x: i32) -> i32 {

    var x := given x {
        _: 32
    }
    
}
```
## Structs

```
type Person: struct {
    age: u32,
};

var person : struct {
    age: u32,
} = { 
    .age = 30,
};
```

## 3.5. Traits
### 3.5.1. Names for traits in other languages
Rust: ``trait``
Swift: ``protocol`` ``extension``
C++: ``concept``
Perl: ``role``
Kotlin: ``interface``
Haskell: ``typeclass``




```

```