#pragma once

/// NullType:
struct NullType {};


/// TypeList:
template <typename... Args>
struct TypeList {};

template <typename H, typename... T>
struct TypeList<H, T...> {
    using Head = H;
    using Tail = TypeList<T...>;
};

template <typename H>
struct TypeList<H> {
    using Head = H;
    using Tail = NullType;
};

template <>
struct TypeList<> {
    using Head = NullType;
    using Tail = NullType;
};


/// Length:
template <typename... T>
struct Length {};

template <typename... T>
struct Length<TypeList<T...>> {
    static constexpr size_t value = 1 + Length<typename TypeList<T...>::Tail>::value;
};

template <>
struct Length<TypeList<>> {
    static constexpr size_t value = 0;
};

template <>
struct Length<NullType> {
    static constexpr size_t value = 0;
};


/// Concat:
template <typename... T>
struct Concat {};

template <typename T, typename... U>
struct Concat<T, TypeList<U...>> {
    using type = TypeList<T, U...>;
};

template <typename T>
struct Concat<T, TypeList<>> {
    using type = TypeList<T>;
};

template <typename T>
struct Concat<T, NullType> {
    using type = TypeList<T>;
};

template <>
struct Concat<NullType, NullType> {
    using type = TypeList<>;
};


/// TypeAt
template <size_t, typename...>
struct TypeAt {};

template <size_t i, typename... T>
struct TypeAt<i, TypeList<T...>> {
    static_assert(i < sizeof...(T));
    using type = typename TypeAt<i - 1, typename TypeList<T...>::Tail>::type;
};

template <typename... T>
struct TypeAt<0, TypeList<T...>> {
    using type = typename TypeList<T...>::Head;
};


template <typename TList, class U>
struct Erase {
    using type = typename Concat<typename TList::Head, typename Erase<typename TList::Tail, U>::type>::type;
};

template <typename Head, typename... T>
struct Erase<TypeList<Head, T...>, Head> {
    using type = TypeList<T...>;
};

template <class U>
struct Erase<NullType, U> {
    using type = NullType;
};