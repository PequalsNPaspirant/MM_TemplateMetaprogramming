

/*

The using syntax has an advantage when used within templates. If you need the type abstraction, but also need to keep template parameter to be possible to be specified in future. You should write something like this.

template <typename T> struct whatever {};

template <typename T> struct rebind
{
typedef whatever<T> type; // to make it possible to substitue the whatever in future.
};

rebind<int>::type variable;

template <typename U> struct bar { typename rebind<U>::type _var_member; }
But using syntax simplifies this use case.

template <typename T> using my_type = whatever<T>;

my_type<int> variable;
template <typename U> struct baz { my_type<U> _var_member; }

*/