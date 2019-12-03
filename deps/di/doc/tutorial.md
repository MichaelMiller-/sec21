###0. [Pre] Refactor towards DI

If you write a new application you can skip this step and go directly to [step 1](#1-basic-create-objects-tree).
However, if you have a lot code which is not using DI and you wonder what can it be refactored,
then you are in the right place.

Basically, there is a only one (big) step to get all benefits of Dependency Injection.
You have to separate creation logic from business logic, which means that your code
should be free of object creation inside other objects...

```cpp
class controller {
public:
  controller() : model_(new model{}) {}
  ~controller() { delete model_; }
  void run();

private:
  model* model_;
};

int main() {
  controller controller_;
  controller_.run();
}
```

Instead, DI approach would look like that...

```cpp
class controller {
public:
  explicit controller(model& m) : model_(m) {}
  void run();

private:
  model& model_;
};

int main() {
  model model_;
  controller controller_{model_};
  controller_.run();
}
```

So, what happened here? We just took the responsibility of creation `model` out from the `controller`. In other words,
we have split the creation logic and the business logic.

That's basically everything you have to remember in order to create applications using DI.
Nevertheless, please, be careful and don't 'carry out' your dependencies. What is meant by that,
is NOT to pass an object into constructor if it won't be stored ([Law of Demeter]).

```cpp
class app {
public:
  explicit app(model& m) : controller_(m) {} // BAD
  explicit app(controller& c) : controller_(c) {} // GOOD

private:
  controller controller_;
};
class controller {
public:
  explicit controller(model&);
};

int main() {
  model model_;
  app app_{model_};
}
```

###1. [Basic] Create objects tree

Before we will get into creating objects tree, let's first create a 'dummy' example.
In order to do so, firstly, we have to include (one and only) `boost/di.hpp` header and
declare a convenient `di` namespace alias.

```cpp
#include <boost/di.hpp>
namespace di = boost::di;
```

That is enough to try out `[Boost].DI`!

To have a first complete and working example we just have to add `main` function as usual.

```cpp
int main() {}
```

and compile our code using compiler supporting C++14 standard (Clang-3.4/GCC-5/MSVC-2015).

```
$CXX -std=c++14 example.cpp
```

Congrats, you are now ready to check out `[Boost].DI` features!

---

Let's move on to creating objects tree. Applications, usually, consists of a number of objects
which have to be instantiated. For example, let's consider a simplified Model View Controller code...

[![Create objects tree](images/tutorial_create_objects_tree.png)](images/tutorial_create_objects_tree.png)

The usual approach to create `app` would be following...

```cpp
renderer renderer_;
view view_{"", renderer_};
model model_;
controller controller_{model_, view_};
user user_;
app app_{controller_, user_};
```

Which is alright for a really small applications. However, it's really tedious to maintain.
Just imagine, that we have to change something here. For instance, `view` may need a new object `window`
or, even worse, we refactored the code and dependencies order has changed - yea **ORDER** of above is important!
**ANY** change in these classes constructors require developer input to maintain above boilerplate code!
Not fun, not fun at all :(

Right now imagine that your maintain effort will be minimized almost to none. How does it sound?
Well, that might be simply achieved with `[Boost].DI`!

The same result might be achieved with one liner. Doesn't matter how big the hierarchy will be.
We just have to create [injector] using [make_injector] and create the `app`.

```cpp
auto app_{make_injector().create<app>()};
```

Moreover, changes in the constructor of created objects will be handled automatically, so in our case
when we add a `window` to `view` or change `view&` to `std::shared_ptr<view>` required effort will be
exactly '0'. `[Boost].DI` will take care of everything for us!

| Type `T` | Is allowed? | Note |
| -------- | ----------- | ---- |
| `T` | ✔ | - |
| `T*` | ✔ | Ownership transfer! |
| `const T*` | ✔ | Ownership transfer! |
| `T&` | ✔ | - |
| `const T&` | ✔ | Reference with [singleton] / Temporary with [unique] |
| `T&&` | ✔ | - |
| `std::unique_ptr<T>` | ✔ | - |
| `std::shared_ptr<T>` | ✔ | - |
| `std::weak_ptr<T>` | ✔ | - |
| `boost_shared_ptr<T>` | ✔ | - |

Furthermore, there is no performance penalty for using `[Boost].DI` (see [Performance](overview.md#performance))!

<span class="fa fa-eye wy-text-neutral warning"> **Note**<br/><br/>
[Boost].DI can [inject] dependencies using direct initialization `T(...)` or uniform initialization `T{...}` for aggregate types.
</span>

Check it out yourself!
![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/tutorial/basic_create_objects_tree.cpp)

Check out also other examples. Please, notice that the diagram was also generated using `[Boost].DI` but we will get into that a bit later.

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)
![CPP(BTN)](Run_Automatic_Injection_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/automatic_injection.cpp)
![CPP(BTN)](Run_UML_Dumper_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/policies/uml_dumper.cpp)

<br /><hr />

###2. [Basic] First steps with bindings

But objects tree is not everything. A lot of classes uses interfaces or required a value to be passed.
`[Boost].DI` solution for this are [bindings].

For purpose of this tutorial, let's change `view` class into interface `iview` in order to support `text_view` and `gui_view`.

```cpp
class iview {
public:
  virtual ~iview() noexcept = default;
  virtual void update() =0;
};

class gui_view: public iview {
public:
  gui_view(std::string title, const renderer&) {}
  void update() override {}
};

class text_view: public iview {
public:
  void update() override {}
};
```

Please, notice that `text_view` doesn't require any constructor parameters, whilst `gui_view` does.

So, what will happen right now, when we try to create an `app`?

```cpp
auto app_{make_injector().create<app>()};
```

COMPILE error! (See also: [Error Messages](overview.md#error-messages))

```cpp
warning: 'create<app>' is deprecated: creatable constraint not satisfied
  injector.create<app>();
           ^
boost/di.hpp:870:2: error: 'boost::di::v1_0_0::concepts::abstract_type<iview>::is_not_bound::error'
  error(_ = "type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?");
```

<span class="fa fa-eye wy-text-neutral warning"> **Note**<br/><br/>
You can get more info about error by increasing [BOOST_DI_CFG_DIAGNOSTICS_LEVEL] [0-2] value (default=1).
</span>

Ah, okay, we haven't bound `iview` which means that `BOOST.DI` can't figure out whether we want `text_view` or `gui_view`?
Well, it's really simple to fix it, we just follow suggestion provided.

```cpp
auto injector = di::make_injector(
  di::bind<iview>.to<gui_view>()
);
```

Let's try again. Yay! It's compiling.

But what about values? `renderer` requires `device`, which, by default, was value initialized.

```cpp
struct renderer {
  int device;
};
```

<span class="fa fa-eye wy-text-neutral warning"> **Note**<br/><br/>
If you want change the default behaviour and be sure that all required dependencies are bound and not value initialized
take a look at [constructible] policy.
</span>

```cpp
auto injector = di::make_injector(
  di::bind<iview>.to<gui_view>()
, di::bind<int>.to(42) // renderer device | [Boost].DI can also deduce 'int' type for you -> 'di::bind<>.to(42)'
);
```

<span class="fa fa-eye wy-text-neutral warning"> **Note**<br/><br/>
[Boost].DI is a compile time beast which means that it guarantees that if your code compiles, all dependencies will be resolved
correctly. No runtime exceptions or runtime asserts, EVER!
</span>

Check it out yourself!
![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/tutorial/basic_first_steps_with_bindings.cpp)

Great, but my code is more dynamic than that! I mean that I want to choose `gui_view` or `text_view` at runtime.
`[Boost].DI` can handle that too!

```cpp
auto use_gui_view = ...;

auto injector = di::make_injector(
  di::bind<iview>.to([&](const auto& injector) -> iview& {
    if (use_gui_view)
      return injector.template create<gui_view&>();
    else
      return injector.template create<text_view&>();
  })
, di::bind<>.to(42) // renderer device
);
```

Notice, that [injector] was passed to lambda expression in order to create `gui_view` / `text_view`.
This way `[Boost].DI` can inject appropriate dependencies into chosen types. See [bindings] for more details.

Check it out yourself!
![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/tutorial/basic_first_steps_with_dynamic_bindings.cpp)

Okay, so what about the input. We have `user`, however, in the real life, we will have more clients.
`[Boost].DI` allows multiple bindings to the same type for `array/vector/set`. Let's do it then!

```cpp
class iclient {
 public:
   virtual ~iclient() noexcept = default;
   virtual void process() = 0;
};

class user : public iclient {
 public:
   void process() override {};
};

class timer : public iclient {
 public:
   void process() override {};
};

class app {
 public:
  app(controller&, std::vector<std::unique_ptr<iclient>>);
};
```

And our bindings...

```cpp
di::bind<iclient*[]>.to<user, client>()
```

Check it out yourself!
![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/tutorial/basic_first_steps_with_multiple_bindings.cpp)

The last but not least, sometimes, it's really useful to override some bindings. For example, for testing purposes.
With `[Boost].DI` you can easily do that with [override] specifier.

```cpp
auto injector = di::make_injector(
  di::bind<int>.to(42) // renderer device
, di::bind<int>.to(123) [di::override] // override renderer device
);
```

Without the `di::override` following compilation error will occur...

```cpp
boost/di.hpp:281:3: error: static_assert failed "constraint not satisfied"
boost/di.hpp:2683:80: type<int>::is_bound_more_than_once
  inline auto make_injector(TDeps... args) noexcept
```

And full example!
![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/tutorial/basic_first_steps_with_bindings_override.cpp)

Check out also.

![CPP(BTN)](Run_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/bindings.cpp)
![CPP(BTN)](Run_Forward_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/fwd_bindings.cpp)
![CPP(BTN)](Run_Dynamic_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/dynamic_bindings.cpp)
![CPP(BTN)](Run_Multiple_Bindings_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/multiple_bindings.cpp)
![CPP(BTN)](Run_Constructor_Bindings_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/bindings/constructor_bindings.cpp)

<br /><br /><br /><hr />

###3. [Basic] Decide the life times

So far so good but where these objects are stored?
Well, `[Boost].DI` supports [scopes] which are response for maintaining the life time of created objects.
By default there are 4 scopes

* [deduce] scope (default)
* [instance] scope (bind<>.to(value) where value is maintained by the user)
* [unique] scope (one instance per request)
* [singleton] scope (shared instance)

By default [deduce] scope is used which means that scope is deduced based on a constructor parameter.
For instance, reference, shared_ptr will be deduced as [singleton] scope and pointer, unique_ptr will be deduced as [unique] scope.

| Type | Scope |
|------|-------|
| T | [unique] |
| T& | [singleton] |
| const T& | [unique] (temporary) / [singleton] |
| T* | [unique] (ownership transfer) |
| const T* | [unique] (ownership transfer) |
| T&& | [unique] |
| std::unique_ptr<T> | [unique] |
| std::shared_ptr<T> | [singleton] |
| boost::shared_ptr<T> | [singleton] |
| std::weak_ptr<T> | [singleton] |

Coming back to our example, we got quite a lot `singletons` there as we just needed one instance per application life time.
Although scope deduction is very useful, it's not always what we need and therefore `[Boost].DI` allows changing the scope for given type.

```cpp
auto injector = di::make_injector(
  di::bind<iview>.to<gui_view>().in(di::singleton) // explicitly specify singleton scope
);
```

What if I want to change `gui_view` to be a different instance per each request. Let's change the scope to [unique] then.

```cpp
auto injector = di::make_injector(
  di::bind<iview>.to<gui_view>().in(di::unique)
);
```

It's NOT compiling!

```cpp
warning: 'create<app>' is deprecated: creatable constraint not satisfied
  injector.create<app>();
           ^
boost/di.hpp:897:2: error: 'scoped<scopes::unique, gui_view>::is_not_convertible_to<iview &>::error'
  error(_ = "scoped object is not convertible to the requested type, did you mistake the scope: 'di::bind<T>.in(scope)'?");
```

Ah, reference doesn't make much sense with [unique] scope because it would mean that it has to be stored somewhere.
It would be better to use `std::unique_ptr<iview>` instead.

| Type/Scope | [unique] | [singleton] | [instance] |
|------------|----------|-------------|------------|
| T | ✔ | - | ✔ |
| T& | - | ✔  | ✔ |
| const T& | ✔ (temporary) | ✔ | ✔ |
| T* (transfer ownership) | ✔ | - | - |
| const T* | ✔ | - | - |
| T&& | ✔ | - | ✔ |
| std::unique_ptr<T> | ✔ | - | - |
| std::shared_ptr<T> | ✔ | ✔ | ✔ |
| boost::shared_ptr<T> | ✔ | ✔ | - / ✔ converted to |
| std::weak_ptr<T> | - | ✔ |  - / ✔ converted to |

Hmm, let's try something else then. We have list of unique clients, we can share objects just by changing the list to
use `std::shared_ptr` instead.

```cpp
class app {
 public:
  app(controller&, std::vector<std::shared_ptr<iclient>>);
};
```

But, it would be better if `timer` was always created per request, although it's a `shared_ptr`.
To do so, we just need add scope when binding it, like this...

```cpp
auto injector = di::make_injector(
  di::bind<timer>.in(di::unique) // different per request
);
```

Check out full example here.
![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/tutorial/basic_decide_the_life_times.cpp)

See also.

![CPP(BTN)](Run_Deduce_Scope_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/deduce_scope.cpp)
![CPP(BTN)](Run_Scopes_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/scopes.cpp)
![CPP(BTN)](Run_Eager_Singletons_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/eager_singletons.cpp)

<br /><br /><hr />

###4. [Basic] Annotations to the rescue

Above example are fine and dandy, nonetheless, they don't cover one important thing.
How `[Boost].DI` knows which constructor to choose and what if they are ambiguous?

Well, algorithm is very simple. The longest (most parameters), unique constructor will be chosen.
Otherwise, `[Boost].DI` will give up with compile time error. However, which constructor should
be chosen is configurable by [BOOST_DI_INJECT].

To illustrate this, let modify `model` constructor.

```cpp
class model {
 public:
   model(int size, double precision) { }
   model(int rows, int cols) { }
};
```

Right now, as expected, we get a compile time error!

```cpp
warning: 'create<app>' is deprecated: creatable constraint not satisfied
  injector.create<app>();
           ^
boost/di.hpp:942:4: error: 'type<model>::has_ambiguous_number_of_constructor_parameters::error'
  error(_ = "verify BOOST_DI_INJECT_TRAITS or di::ctor_traits");
```

Let's fixed it using [BOOST_DI_INJECT] then!

```cpp
class model {
 public:
   model(int size, double precision) { }
   BOOST_DI_INJECT(model, int rows, int cols) { } // this constructor will be injected
};
```

<span class="fa fa-eye wy-text-neutral warning"> **Note**<br/><br/>
We can also write `model(int rows, int cols, ...)` to get the same result.
</span>

Okay, right now it compiles but, wait a minute, `123` (renderer device) was injected for both `rows` and `cols`!
Well, it wasn't even closed to what we would like to, but we can fix it easily using [named] annotations.

Firstly, we have to create names. That's easy as names are just unique objects.

```cpp
auto Rows = []{};
auto Cols = []{};
```

Secondly, we have to tell `model` constructor about it.

```cpp
class model {
 public:
   model(int size, double precision) { }
   BOOST_DI_INJECT(model, (named = Rows) int rows, (named = Cols) int cols); // this constructor will be injected
};

model::model(int rows, int cols) {}
```

Please, notice that we have separated `model` constructor definition and declaration to show that definition doesn't
require named annotations.

<span class="fa fa-eye wy-text-neutral warning"> **Note**<br/><br/>
If you happen to use clang/gcc compiler you can use string literals instead of creating objects,
for example `(named = "Rows"_s)`.
</span>

Finally, we have to bind our values.

```cpp
auto injector = di::make_injector(
  di::bind<int>.named(Rows).to(6)
, di::bind<int>.named(Cols).to(8)
);
```

That's all.

<span class="fa fa-eye wy-text-neutral warning"> **Note**<br/><br/>
The same result might be accomplished with having different types for rows and cols.
</span>

Full example here.
![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/tutorial/basic_annotations_to_the_rescue.cpp)

Check out also.

![CPP(BTN)](Run_Constructor_Injection_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/constructor_injection.cpp)
![CPP(BTN)](Run_Constructor_Signature_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/constructor_signature.cpp)
![CPP(BTN)](Run_Annotations_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/annotations.cpp)

<br /><br /><hr />

###5. [Basic] Split your configuration

But my project has hundreds of interfaces and I would like to split my bindings into separate components.
There is nothing simpler than that with `[Boost].DI` as [injector] might be extended by other [injector].

Let's split our configuration then and keep our `model` bindings separately from `app` bindings.

```cpp
auto model_module = [] {
  return di::make_injector(
    di::bind<int>.named(Rows).to(6)
  , di::bind<int>.named(Cols).to(8)
  );
};

auto app_module = [](const bool& use_gui_view) {
  return di::make_injector(
    di::bind<iview>.to([&](const auto& injector) -> iview& {
      if (use_gui_view)
        return injector.template create<gui_view&>();
      else
        return injector.template create<text_view&>();
    })
  , di::bind<timer>.in(di::unique) // different per request
  , di::bind<iclient*[]>().to<user, timer>() // bind many clients
  );
};
```

And glue them into one injector the same way...

```cpp
  auto injector = di::make_injector(
    model_module()
  , app_module(use_gui_view)
  );
```

Check it out yourself!
![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/tutorial/basic_split_your_configuration.cpp)

But I would like to have a module in `cpp` file, how can I do that?
Such design might be achieved with `[Boost].DI` using [injector] and exposing given types.
Let's refactor it then.

```cpp
di::injector<model&> model_module() {
  return di::make_injector(
    di::bind<int>.named(Rows).to(6)
  , di::bind<int>.named(Cols).to(8)
  );
}

di::injector<app> app_module(const bool& use_gui_view) {
  return di::make_injector(
    di::bind<iview>.to([&](const auto& injector) -> iview& {
      if (use_gui_view)
        return injector.template create<gui_view&>();
      else
        return injector.template create<text_view&>();
    })
  , di::bind<timer>.in(di::unique) // different per request
  , di::bind<iclient*[]>.to<user, timer>() // bind many clients
  , model_module()
  );
}
```

Right now you can easily separate definition and declaration between `hpp` and `cpp` files.

Check it out here!
![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/tutorial/basic_split_your_configuration_expose.cpp)

<span class="fa fa-eye wy-text-neutral warning"> **Note**<br/><br/>
You can also expose named parameters using `di::injector<BOOST_DI_EXPOSE((named = Rows) int)>`.
Different variations of the same type have to be exposed explicitly using `di::injector<model&, std::unique_ptr<model>>`.
Expose uses type erasure and therefore it has small performance overhead!
</span>

More examples.

![CPP(BTN)](Run_Modules_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/modules.cpp)
![CPP(BTN)](Run_Exposed_Annotated_Type_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/module_exposed_annotated_type.cpp)
![CPP(BTN)](Run_Exposed_Complex_Types_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/module_exposed_complex_types.cpp)

<br /><br />

Congrats! You have finished the basic part of the tutorial.
Hopefully, you have noticed potential of DI and `[Boost].DI` but if are still not convinced check out the [Advanced](#6-advanced-dumplimit-your-types) part.

###6. [Advanced] Dump/Limit your types

It's often a case that we would like to generate object diagram of our application in order to see code dependencies
more clear. Usually, it's a really hard task as creation of objects may happen anywhere in the code. However,
if the responsibility for creation  objects will be given to `[Boost].DI` we get such functionality for free.
The only thing we have to do is to implement how to dump our objects.

Let's dump our dependencies using [Plant UML](http://plantuml.com) format.

![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/policies/uml_dumper.cpp)
[![UML Dumper](images/uml_dumper.png)](images/uml_dumper.png)

See also.  <br /><br />
![CPP(BTN)](Run_Custom_Policy_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/custom_policy.cpp)
![CPP(BTN)](Run_Types_Dumper_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/policies/types_dumper.cpp)

<br />

On the other hand, it would be great to be able to limit types which might be constructed. For example, we just want to allow
smart pointers and disallow raw pointers too. We may want to have a `view` only with const parameters being passed, etc.
`[Boost].DI` allows you to do so by using [constructible] policy or writing a custom [policy].

![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/policies_constructible_global.cpp)

See also.

![CPP(BTN)](Run_Constructible_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/user_guide/policies_constructible_local.cpp)

<br /><hr />

###7. [Advanced] Customize it

`[Boost].DI` was design having extensibility in mind. You can easily customize

* [scopes] - to have custom life time of an object
* [providers] - to have custom way of creating objects, for example by using preallocated memory
* [policies] - to have custom way of dumping types at run-time or limiting them at compile-time

![CPP(BTN)](Run_Custom_Policy_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/custom_policy.cpp)
![CPP(BTN)](Run_Custom_Provider_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/custom_provider.cpp)
![CPP(BTN)](Run_Pool_Provider_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/pool_provider.cpp)

<br /><hr />

###8. [Advanced] Extend it

As mentioned before, `[Boost].DI` is quite easy to extend and therefore a lot of extensions exists already.
Please check them out and write your own!

* [Constructor Bindings](extensions.md#constructor-bindings)
* [Assisted Injection](extensions.md#assisted-injection)
* [Concepts](extensions.md#concepts)
* [Lazy](extensions.md#lazy)
* [Named Parameters](extensions.md#named-parameters)
* [XML Injection](extensions.md#xml-injection)
* [Types Dumper](extensions.md#types-dumper)
* [UML Dumper](extensions.md#uml-dumper)
* [Mocks Provider](extensions.md#mocks-provider)
* [Scoped Scope](extensions.md#scoped-scope)
* [Session Scope](extensions.md#session-scope)
* [Shared Scope](extensions.md#shared-scope)

[bindings]: user_guide.md#bindings
[injector]: user_guide.md#di_make_injector
[make_injector]: user_guide.md#make_injector
[BOOST_DI_INJECT]: user_guide.md#BOOST_DI_INJECT
[override]: user_guide.md#di_bind
[inject]: user_guide.md#di_automatic
[named]: user_guide.md#di_named
[scopes]: user_guide.md#scopes
[deduce]: user_guide.md#di_deduce
[instance]: user_guide.md#di_instance
[unique]: user_guide.md#di_unique
[singleton]: user_guide.md#di_singleton
[providers]: user_guide.md#providers
[policy]: user_guide.md#policies
[policies]: user_guide.md#policies
[constructible]: user_guide.md#di_constructible
[Law of Demeter]: https://en.wikipedia.org/wiki/Law_of_Demeter
[BOOST_DI_CFG_CTOR_LIMIT_SIZE]: overview.md#configuration
[BOOST_DI_CFG_DIAGNOSTICS_LEVEL]: overview.md#configuration
