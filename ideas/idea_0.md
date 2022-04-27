- [Stage 0: Big picture](#stage-0-big-picture)
- [First steps: Making a "better" C](#first-steps-making-a-better-c)
  - [Const/immutable by default](#constimmutable-by-default)
  - [? Maybe function overloading](#-maybe-function-overloading)
  - [? Maybe Delayed initialization](#-maybe-delayed-initialization)
- [Features](#features)
  - [Functions](#functions)
  - [Named function parameters](#named-function-parameters)
  - [Operator overloading and custom operators](#operator-overloading-and-custom-operators)
  - [Dynamic numerical base](#dynamic-numerical-base)
  - [Use keyword](#use-keyword)
  - [Import keyword](#import-keyword)
  - [Switch statements](#switch-statements)
  - [Complex Types](#complex-types)
    - [Unit Types](#unit-types)
    - [Product Types](#product-types)
      - [Tuples](#tuples)
      - [Struct](#struct)
    - [Sum Types](#sum-types)
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
  - [Sorting](#sorting)
    - [Quicksort](#quicksort)

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
```
var x: mut = 4;
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
## Functions
Functions are declared with the keyword `fn`.

Functions parameters are by default copied by value and immutable.

```
fn foo(x: i32): () {
    x + 1; // Error. Can't mutate an immutable variable
}
```


The below does not create an error, but there is no outside effect, because the parameters is a copy of the original value.
```
fn foo(x: mut i32): () {
    x + 1; 
}
```

To actually create the desired effect.

```
fn foo(x: ref mut i32): () {
    x + 1; 
}
```
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

## Use keyword

```
type Person = struct {
    name: String,
    age: i32,
}

fn foo(p: ref Person) {
    use p;
    return name;
}
```

## Import keyword
```
import math;

fn main(): () {
    var x: i32 = math::sqrt(4);
}
```
```
import math as *;

fn main(): () {
    var x: i32 = sqrt(4);
}
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
The keyword `type` is used to declare a new type.

### Unit Types
One can also create custom unit types, which are not simply the type `()` or `Void`. Those types don't have any fields.
```
type Unit;
```
### Product Types
#### Tuples

Tuples are a special kind of product type. They are a collection of values, which are accessed by index.

nameless and fieldless
```
var person := (1, "Mark");
```
nameless and fieldful
```
var person := (id := 1, name := "Mark");
var person := (
    id := 1, 
    name := "Mark"
);
```
nameful and fieldless
```
var x := Point(1, 2.0);
```
nameful and fieldful
```
var x := (1, 2.0);
```
#### Struct

Tuple struct
```rust
type Pair = struct {i32, i32};
```
This is basically another way to declare a named tuple. Or more like a named tuple is the shorthand notation for this kind of struct.
Maybe do not include this into the language as it is superfluous.

Field struct
```rust
type Point := struct {
    x: i32,
    y: i32,
}
```

```
type Person := struct {
    id: u32,
    age: u32,
    height: f32,
};

var person = struct {
    age: u32,
} = { 
    .age = 30,
};
```

```
type Person := struct{u32, u32, f32};
```
### Sum Types
#### Union
```rust
type Animal = union {
    Dog,
    Cat,
    Fish,
}
```
From a more theoretical point of view, this union consists of three unit types. In pseudo-code, one could thus write:
```rust
type Animal := union {
    type Dog,
    type Cat,
    type Fish,
}
```

```rust
type Option<T> := union {
    Some<T>,
    None,
}
```
#### C-style Union

#### Enum
Enums function more or less exactly like unions, only that they are more like "named integers" and thus by default have a value. If not specified, the first member is 0 from where it is then incremented.
```
type Color = enum u32 {
    Red,
    Blue,
    Yellow,
};
```
#### Bitmask
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

## Sorting
### Quicksort