# C++ Primer

This page is not intended to teach you C++, but can give you some quick tips on things that may be unfamiliar (or gotchas to watch out specifically with HSEF)

## Important Keywords

Let us first go over some keywords you may not be familiar with. Below is not an exhaustive defintion
of each of these keywords, but provides some guidance in how it is used in HSEF.
<br/><br/>
`const`

This is a somewhat overloaded keyword that we primarily use in the two following ways.

When you see a method signature `int method(const Type &param)`, what this means is that a reference
to `param` is being given to `method`, however `method` is not allowed to `param` in any way. Passing
`param` as a reference is often more efficient since a full copy is not needed, so the `const` is necessary
to allow for this efficiency, while ensuring that `param` is not changed when this is not intended.

When you see a class method defined as `int method() const`, this ensures that when we make a call
like `a.method()` on object `a`, this method will not change `a` in any way. This is commonly used
for things like *getters*, which should just return something from `a` without changing it.
<br/><br/>
`using`

This keyword is primarily used to define type aliases for the purpose of readability.
For example, we define `using StringMap = std::map<std::string, std::string>` meaning that we will
use `StringMap` as a shorthand for a map (ie. hash map) whose keys are strings and whose values are
strings.

`using` will also be used to disambiguate which method we are calling, as we will see in the template section
<br/><br/>
`auto`

This keyword allows for auto type inference. In modern C++, the compiler can often figure out what type
a variable is when it is set as the output of a method, based on the methods signature.
For example, the compiler can easily figure out from `auto var = getValue()` that `var` should be set
to whatever type `getValue()` returns. This is handy when the types are very long or unwieldy.
<br/><br/>
`optional`

This keyword allows us to return a value or something like `null` (specifically `std::nullopt`). 
This can be handy for failure cases, for example, where a method should return an integer if it 
succeeds and `std::nullopt` otherwise. We can also use it to allow for a member variable to be 
initialized as `std::nullopt` prior to it being set.
<br/><br/>
`enum class`

This keyword is used for the definition of a finite set of constants. For example, it is used to define the type for moves in the sliding
tile puzzle in tile_puzzle_transitions.h since there is only a very limited number of such moves.


## Templates and Inheritance

HSEF is intended to allow for quickly implementing new domains and running existing algorithms on them.
To facilitate this, HSEF makes extensive use of templates. Classes that use templates begin their 
class definition with `template<class T>`.  This means that the defined code can be used for various 
different types. `T` is just a standin for whatever type you will use. For example, the vector class
is defined as `vector<T>` since it is a container that can hold just about anything. To create a
vector for `double` values, you would write `vector<double>`. Our class definitions would work similarly.

Of particular note, you may recall that in C one usually puts the function definition in a header 
(ie. a `.h` file) and the function implementation in another file (usually a `.c` file). Templated 
functions and classes don't allow for this (the reason why is technical), so the implmentations of 
these will be  in the header file for such templated cases.

The codebase also uses inheritance extensively to define interfaces. For example, in 
`id_engine.h`, you can see the following:

```c++
template<class State_t, class Action_t>
class IDEngine : public SingleStepSearchEngine<State_t, Action_t>
```

This says that the `IDEngine` class inherits from `SingleStepSearchEngine` using template types 
state_t and action_t. You will see functions designated as `virtual`, which means that the function 
can be overloaded in a child class, and when this is done, call the child's version of the function 
by default. If the function definition has `= 0` at the end, this means it is an abstract function 
in an abstract class.

Of note, mixing inheritance and templates can get a bit ugly at times. In particular, the compiler
will require you to resolve ambiguities where you wouldn't expect it. In particular, in other
languages like Java or Python, one can usually automatically access the protected or public member 
variables or functions from a child class. In C++ you need to be more specific.

For example, in `SingleStepSearchEngine`, there is a method called `isGoal(state)`, which calls the
goal test function on the given state and increments the count of the number of goal tests performed.
However, we cannot just call `isGoal(state)` from within `IDEngine`. Instead, we have to call the
following:

```c++
SingleStepSearchEngine<State_t, Action_t>::isGoal(state);
```
This disambiguates which `isGoal` we are calling for the compiler. Note that the reason for this
ambiguity is a bit technical, so just keep in mind it is necessary even if it is not obvious why.

Further, because the above call to `isGoal` is quite a mouthful, you will notice that at the top of
`IDEngine`, we instead have a statement like
```c++
using SE = SingleStepSearchEngine<State_t, Action_t>;
```
As describe above, this provides an alias for `SingleStepSearchEngine<State_t, Action_t>`, 
meaning we can now disambiguate the `isGoal`his allows us to instead call `SE::isGoal(state)`. 
Moreover, all the other public and protected methods and variables in `SingleStepSearchEngine` can
be accessed similarly.
