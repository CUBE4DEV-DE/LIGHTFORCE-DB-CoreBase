#pragma once

//-----------------------------------------------------------------------------
// Author:  Anton Bukov
// Company: 1inch Network
// Date:    08.04.2022
// Class:   XLinq
// License: MIT
// GITHUB:  https://github.com/k06a/boolinq/blob/master/include/boolinq/boolinq.h
//----------------------------------------------------------------------------- 


// Refactored

#include <limits.h>

#include <functional>
#include <tuple>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <deque>
#include <list>
#include <QList>
#include <set>
#include <unordered_set>

namespace CoreBase {

    namespace priv {
        template<typename _Callable>
        struct resultOf;

        template<typename _Callable, typename... _Args>
        struct resultOf<_Callable(_Args...)> {
            typedef decltype(std::declval<_Callable>()(std::declval<_Args>()...)) type;
        };
    }

    //

    struct LinqEndException {};

    enum BytesDirection {
        BytesFirstToLast,
        BytesLastToFirst,
    };

    enum BitsDirection {
        BitsHighToLow,
        BitsLowToHigh,
    };

    //-----------------------------------------------------------------------------
    // Author:  Tobias Post
    // Company: CUBE4DEV GmbH
    // Date:    08.04.2022
    // Context: DefaultNamespace
    // Class:   XLinq
    // Method:  XLinq
    // Description: The XLinq class is part of the C++ library "boolinq" and
    // is used to create queryable collections of objects. It provides a
    // range of methods that allow for filtering, grouping, and aggregation
    // operations to be performed on data. These methods return new XLinq
    // objects with the appropriate transformations applied. The use of
    // lambda functions provides the flexibility to apply custom
    // functions to the data.
    //----------------------------------------------------------------------------- 
    
    template<typename S, typename T>
    class XLinq {
        std::function<T(S&)> nextFunc;
        S storage;

    public:
        typedef T value_type;

        XLinq();

        XLinq(S storage, std::function<T(S&)> nextFunc);

        T next();

        void forEachI(std::function<void(T, int)> apply) const;
        void forEach(std::function<void(T)> apply) const;
        XLinq<std::tuple<XLinq<S, T>, int>, T> whereI(std::function<bool(T, int)> filter) const;
        XLinq<std::tuple<XLinq<S, T>, int>, T> where(std::function<bool(T)> filter) const;
        XLinq<std::tuple<XLinq<S, T>, int>, T> take(int count) const;
        XLinq<std::tuple<XLinq<S, T>, int>, T> takeWhileI(std::function<bool(T, int)> predicate) const;
        XLinq<std::tuple<XLinq<S, T>, int>, T> takeWhile(std::function<bool(T)> predicate) const;
        XLinq<std::tuple<XLinq<S, T>, int>, T> skip(int count) const;
        XLinq<std::tuple<XLinq<S, T>, int, bool>, T> skipWhileI(std::function<bool(T, int)> predicate) const;
        XLinq<std::tuple<XLinq<S, T>, int, bool>, T> skipWhile(std::function<bool(T)> predicate) const;
        template<typename ... Types> XLinq<std::tuple<XLinq<S, T>, std::vector<T>, int>, T> append(Types ... newValues) const;
        template<typename ... Types> XLinq<std::tuple<XLinq<S, T>, std::vector<T>, int>, T> prepend(Types ... newValues) const;
        template<typename F, typename _TRet = typename priv::resultOf<F(T, int)>::type> XLinq<std::tuple<XLinq<S, T>, int>, _TRet> selectI(F apply) const;
        template<typename F, typename _TRet = typename priv::resultOf<F(T)>::type> XLinq<std::tuple<XLinq<S, T>, int>, _TRet> select(F apply) const;
        template<typename TRet> XLinq<std::tuple<XLinq<S, T>, int>, TRet> cast() const;
        template<typename S2, typename T2> XLinq<std::tuple<XLinq<S, T>, XLinq<S2, T2>, bool>, T> concat(const XLinq<S2, T2>& rhs) const;
        template<typename F,typename _TRet = typename priv::resultOf<F(T, int)>::type,typename _TRetVal = typename _TRet::value_type > XLinq<std::tuple<XLinq<S, T>, _TRet, int, bool>, _TRetVal> selectManyI(F apply) const;
        template<typename F,typename _TRet = typename priv::resultOf<F(T)>::type,typename _TRetVal = typename _TRet::value_type> XLinq<std::tuple<XLinq<S, T>, _TRet, int, bool>, _TRetVal> selectMany(F apply) const;
        // where(predicate)
        template<typename F,typename _TKey = typename priv::resultOf<F(T)>::type,typename _TValue = XLinq<std::tuple<XLinq<S, T>, int>, T> >   XLinq<std::tuple<XLinq<S, T>, XLinq<S, T>, std::unordered_set<_TKey> >, std::pair<_TKey, _TValue> > groupBy(F apply) const;
        template<typename F, typename _TRet = typename priv::resultOf<F(T)>::type> XLinq<std::tuple<XLinq<S, T>, std::unordered_set<_TRet> >, T> distinct(F transform) const;
        XLinq<std::tuple<XLinq<S, T>, std::unordered_set<T> >, T> distinct() const;
        template<typename F, typename _TIter = typename std::vector<T>::const_iterator> XLinq<std::tuple<std::vector<T>, _TIter, bool>, T> orderBy(F transform) const;
        XLinq<std::tuple<std::vector<T>, typename std::vector<T>::const_iterator, bool>, T> orderBy() const;
        template<typename _TIter = typename std::list<T>::const_reverse_iterator> XLinq<std::tuple<std::list<T>, _TIter, bool>, T> reverse() const;

        // Aggregators
        template<typename TRet> TRet aggregate(TRet start, std::function<TRet(TRet, T)> accumulate) const;
        template<typename F, typename _TRet = typename priv::resultOf<F(T)>::type> _TRet sum(F transform) const;
        template<typename TRet = T> TRet sum() const;
        template<typename F, typename _TRet = typename priv::resultOf<F(T)>::type> _TRet avg(F transform) const;
        template<typename TRet = T> TRet avg() const;
        int count() const;
        int count(std::function<bool(T)> predicate) const;
        int count(const T& item) const;

        // Bool aggregators
        bool any(std::function<bool(T)> predicate) const;
        bool any() const;
        bool all(std::function<bool(T)> predicate) const;
        bool all() const;
        bool contains(const T& item) const;

        // Election aggregators
        T elect(std::function<T(T, T)> accumulate) const;

        template<typename F> T max(F transform) const;
        T max() const;

        template<typename F> T min(F transform) const;

        T min() const;

        // Single object returners

        T elementAt(int index) const;

        T first(std::function<bool(T)> predicate) const
        {
            return where(predicate).next();
        }

        T first() const
        {
            return XLinq<S, T>(*this).next();
        }

        T firstOrDefault(std::function<bool(T)> predicate, T const& defaultValue = T()) const;

        T firstOrDefault(T const& defaultValue = T()) const;

        T last(std::function<bool(T)> predicate) const;

        T last() const;

        T lastOrDefault(std::function<bool(T)> predicate, T const& defaultValue = T()) const;

        T lastOrDefault(T const& defaultValue = T()) const;

        // Export to containers

        std::vector<T> toStdVector() const;

        std::list<T> toStdList() const;

        QList<T> toList() const;


        std::deque<T> toStdDeque() const
        {
            std::deque<T> items;
            forEach([&items](T value) {
                items.push_back(value);
                });
            return items;
        }

        std::set<T> toStdSet() const
        {
            std::set<T> items;
            forEach([&items](T value) {
                items.insert(value);
                });
            return items;
        }

        std::unordered_set<T> toStdUnorderedSet() const
        {
            std::unordered_set<T> items;
            forEach([&items](T value) {
                items.insert(value);
                });
            return items;
        }

        // Bits and bytes

        XLinq<std::tuple<XLinq<S, T>, BytesDirection, T, int>, int> bytes(BytesDirection direction = BytesFirstToLast) const
        {
            return XLinq<std::tuple<XLinq<S, T>, BytesDirection, T, int>, int>(
                std::make_tuple(*this, direction, T(), sizeof(T)),
                [](std::tuple<XLinq<S, T>, BytesDirection, T, int>& tuple) {
                    XLinq<S, T>& linq = std::get<0>(tuple);
                    BytesDirection& bytesDirection = std::get<1>(tuple);
                    T& value = std::get<2>(tuple);
                    int& index = std::get<3>(tuple);

                    if (index == sizeof(T)) {
                        value = linq.next();
                        index = 0;
                    }

                    unsigned char* ptr = reinterpret_cast<unsigned char*>(&value);

                    int byteIndex = index;
                    if (bytesDirection == BytesLastToFirst) {
                        byteIndex = sizeof(T) - 1 - byteIndex;
                    }

                    index++;
                    return ptr[byteIndex];
                }
            );
        }

        template<typename TRet>
        XLinq<std::tuple<XLinq<S, T>, BytesDirection, int>, TRet> unbytes(BytesDirection direction = BytesFirstToLast) const
        {
            return XLinq<std::tuple<XLinq<S, T>, BytesDirection, int>, TRet>(
                std::make_tuple(*this, direction, 0),
                [](std::tuple<XLinq<S, T>, BytesDirection, int>& tuple) {
                    XLinq<S, T>& linq = std::get<0>(tuple);
                    BytesDirection& bytesDirection = std::get<1>(tuple);
                    // int &index = std::get<2>(tuple);

                    TRet value;
                    unsigned char* ptr = reinterpret_cast<unsigned char*>(&value);

                    for (int i = 0; i < sizeof(TRet); i++) {
                        int byteIndex = i;
                        if (bytesDirection == BytesLastToFirst) {
                            byteIndex = sizeof(TRet) - 1 - byteIndex;
                        }

                        ptr[byteIndex] = linq.next();
                    }

                    return value;
                }
            );
        }

        XLinq<std::tuple<XLinq<S, T>, BytesDirection, BitsDirection, T, int>, int> bits(BitsDirection bitsDir = BitsHighToLow, BytesDirection bytesDir = BytesFirstToLast) const;

        template<typename TRet = unsigned char>
        XLinq<std::tuple<XLinq<S, T>, BytesDirection, BitsDirection, int>, TRet> unbits(BitsDirection bitsDir = BitsHighToLow, BytesDirection bytesDir = BytesFirstToLast) const;
    };

    template <typename S, typename T>
    XLinq<S, T>::XLinq(): nextFunc(), storage()
    {
    }

    template <typename S, typename T>
    XLinq<S, T>::XLinq(S storage, std::function<T(S&)> nextFunc): nextFunc(nextFunc), storage(storage)
    {
    }

    template <typename S, typename T>
    T XLinq<S, T>::next()
    {
	    return nextFunc(storage);
    }

    template <typename S, typename T>
    void XLinq<S, T>::forEachI(std::function<void(T, int)> apply) const
    {
	    XLinq<S, T> linq = *this;
	    try {
		    for (int i = 0; ; i++) {
			    apply(linq.next(), i);
		    }
	    }
	    catch (LinqEndException&) {}
    }

    template <typename S, typename T>
    void XLinq<S, T>::forEach(std::function<void(T)> apply) const
    {
	    return forEachI([apply](T value, int) { return apply(value); });
    }

    template <typename S, typename T>
    XLinq<std::tuple<XLinq<S, T>, int>, T> XLinq<S, T>::whereI(std::function<bool(T, int)> filter) const
    {
	    return XLinq<std::tuple<XLinq<S, T>, int>, T>(
		    std::make_tuple(*this, 0),
		    [filter](std::tuple<XLinq<S, T>, int>& tuple) {
			    XLinq<S, T>& linq = std::get<0>(tuple);
			    int& index = std::get<1>(tuple);

			    while (true) {
				    T ret = linq.next();
				    if (filter(ret, index++)) {
					    return ret;
				    }
			    }
		    }
	    );
    }

    template <typename S, typename T>
    XLinq<std::tuple<XLinq<S, T>, int>, T> XLinq<S, T>::where(std::function<bool(T)> filter) const
    {
	    return whereI([filter](T value, int) { return filter(value); });
    }

    template <typename S, typename T>
    XLinq<std::tuple<XLinq<S, T>, int>, T> XLinq<S, T>::take(int count) const
    {
	    return whereI([count](T /*value*/, int i) {
		    if (i == count) {
			    throw LinqEndException();
		    }
		    return true;
	    });
    }

    template <typename S, typename T>
    XLinq<std::tuple<XLinq<S, T>, int>, T> XLinq<S, T>::takeWhileI(std::function<bool(T, int)> predicate) const
    {
	    return whereI([predicate](T value, int i) {
		    if (!predicate(value, i)) {
			    throw LinqEndException();
		    }
		    return true;
	    });
    }

    template <typename S, typename T>
    XLinq<std::tuple<XLinq<S, T>, int>, T> XLinq<S, T>::takeWhile(std::function<bool(T)> predicate) const
    {
	    return takeWhileI([predicate](T value, int /*i*/) { return predicate(value); });
    }

    template <typename S, typename T>
    XLinq<std::tuple<XLinq<S, T>, int>, T> XLinq<S, T>::skip(int count) const
    {
	    return whereI([count](T /*value*/, int i) { return i >= count; });
    }

    template <typename S, typename T>
    XLinq<std::tuple<XLinq<S, T>, int, bool>, T> XLinq<S, T>::skipWhileI(std::function<bool(T, int)> predicate) const
    {
	    return XLinq<std::tuple<XLinq<S, T>, int, bool>, T>(
		    std::make_tuple(*this, 0, false),
		    [predicate](std::tuple<XLinq<S, T>, int, bool>& tuple) {
			    XLinq<S, T>& linq = std::get<0>(tuple);
			    int& index = std::get<1>(tuple);
			    bool& flag = std::get<2>(tuple);

			    if (flag) {
				    return linq.next();
			    }
			    while (true) {
				    T ret = linq.next();
				    if (!predicate(ret, index++)) {
					    flag = true;
					    return ret;
				    }
			    }
		    }
	    );
    }

    template <typename S, typename T>
    XLinq<std::tuple<XLinq<S, T>, int, bool>, T> XLinq<S, T>::skipWhile(std::function<bool(T)> predicate) const
    {
	    return skipWhileI([predicate](T value, int /*i*/) { return predicate(value); });
    }

    template <typename S, typename T>
    template <typename ... Types>
    XLinq<std::tuple<XLinq<S, T>, std::vector<T>, int>, T> XLinq<S, T>::append(Types... newValues) const
    {
	    return XLinq<std::tuple<XLinq<S, T>, std::vector<T>, int>, T>(
		    std::make_tuple(*this, std::vector<T>{ newValues... }, -1),
		    [](std::tuple<XLinq<S, T>, std::vector<T>, int>& tuple) {
			    XLinq<S, T>& linq = std::get<0>(tuple);
			    std::vector<T>& values = std::get<1>(tuple);
			    int& index = std::get<2>(tuple);

			    if (index == -1) {
				    try {
					    return linq.next();
				    }
				    catch (LinqEndException&) {
					    index = 0;
				    }
			    }

			    if (index < values.size()) {
				    return values[index++];
			    }

			    throw LinqEndException();
		    }
	    );
    }

    template <typename S, typename T>
    template <typename ... Types>
    XLinq<std::tuple<XLinq<S, T>, std::vector<T>, int>, T> XLinq<S, T>::prepend(Types... newValues) const
    {
	    return XLinq<std::tuple<XLinq<S, T>, std::vector<T>, int>, T>(
		    std::make_tuple(*this, std::vector<T>{ newValues... }, 0),
		    [](std::tuple<XLinq<S, T>, std::vector<T>, int>& tuple) {
			    XLinq<S, T>& linq = std::get<0>(tuple);
			    std::vector<T>& values = std::get<1>(tuple);
			    int& index = std::get<2>(tuple);

			    if (index < values.size()) {
				    return values[index++];
			    }
			    return linq.next();
		    }
	    );
    }

    template <typename S, typename T>
    template <typename F, typename _TRet>
    XLinq<std::tuple<XLinq<S, T>, int>, _TRet> XLinq<S, T>::selectI(F apply) const
    {
	    return XLinq<std::tuple<XLinq<S, T>, int>, _TRet>(
		    std::make_tuple(*this, 0),
		    [apply](std::tuple<XLinq<S, T>, int>& tuple) {
			    XLinq<S, T>& linq = std::get<0>(tuple);
			    int& index = std::get<1>(tuple);

			    return apply(linq.next(), index++);
		    }
	    );
    }

    template <typename S, typename T>
    template <typename F, typename _TRet>
    XLinq<std::tuple<XLinq<S, T>, int>, _TRet> XLinq<S, T>::select(F apply) const
    {
	    return selectI([apply](T value, int /*index*/) { return apply(value); });
    }

    template <typename S, typename T>
    template <typename TRet>
    XLinq<std::tuple<XLinq<S, T>, int>, TRet> XLinq<S, T>::cast() const
    {
	    return selectI([](T value, int /*i*/) { return TRet(value); });
    }

    template <typename S, typename T>
    template <typename S2, typename T2>
    XLinq<std::tuple<XLinq<S, T>, XLinq<S2, T2>, bool>, T> XLinq<S, T>::concat(const XLinq<S2, T2>& rhs) const
    {
	    return XLinq<std::tuple<XLinq<S, T>, XLinq<S2, T2>, bool>, T>(
		    std::make_tuple(*this, rhs, false),
		    [](std::tuple<XLinq<S, T>, XLinq<S2, T2>, bool>& tuple) {
			    XLinq<S, T>& first = std::get<0>(tuple);
			    XLinq<S2, T2>& second = std::get<1>(tuple);
			    bool& flag = std::get<2>(tuple);

			    if (!flag) {
				    try {
					    return first.next();
				    }
				    catch (LinqEndException&) {}
			    }
			    return second.next();
		    }
	    );
    }

    template <typename S, typename T>
    template <typename F, typename _TRet, typename _TRetVal>
    XLinq<std::tuple<XLinq<S, T>, _TRet, int, bool>, _TRetVal> XLinq<S, T>::selectManyI(F apply) const
    {
	    return XLinq<std::tuple<XLinq<S, T>, _TRet, int, bool>, _TRetVal>(
		    std::make_tuple(*this, _TRet(), 0, true),
		    [apply](std::tuple<XLinq<S, T>, _TRet, int, bool>& tuple) {
			    XLinq<S, T>& linq = std::get<0>(tuple);
			    _TRet& current = std::get<1>(tuple);
			    int& index = std::get<2>(tuple);
			    bool& finished = std::get<3>(tuple);

			    while (true) {
				    if (finished) {
					    current = apply(linq.next(), index++);
					    finished = false;
				    }
				    try {
					    return current.next();
				    }
				    catch (LinqEndException&) {
					    finished = true;
				    }
			    }
		    }
	    );
    }

    template <typename S, typename T>
    template <typename F, typename _TRet, typename _TRetVal>
    XLinq<std::tuple<XLinq<S, T>, _TRet, int, bool>, _TRetVal> XLinq<S, T>::selectMany(F apply) const
    {
	    return selectManyI([apply](T value, int /*index*/) { return apply(value); });
    }

    template <typename S, typename T>
    template <typename F, typename _TKey, typename _TValue>
    XLinq<std::tuple<XLinq<S, T>, XLinq<S, T>, std::unordered_set<_TKey>>, std::pair<_TKey, _TValue>> XLinq<S, T>::
    groupBy(F apply) const
    {
	    return XLinq<std::tuple<XLinq<S, T>, XLinq<S, T>, std::unordered_set<_TKey> >, std::pair<_TKey, _TValue> >(
		    std::make_tuple(*this, *this, std::unordered_set<_TKey>()),
		    [apply](std::tuple<XLinq<S, T>, XLinq<S, T>, std::unordered_set<_TKey> >& tuple) {
			    XLinq<S, T>& linq = std::get<0>(tuple);
			    XLinq<S, T>& linqCopy = std::get<1>(tuple);
			    std::unordered_set<_TKey>& set = std::get<2>(tuple);

			    while (true) {
				    _TKey key = apply(linq.next());
				    if (set.insert(key).second) {
					    return std::make_pair(key, linqCopy.where([apply, key](T v) {
						    return apply(v) == key;
					    }));
				    }
			    }
		    }
	    );
    }

    template <typename S, typename T>
    template <typename F, typename _TRet>
    XLinq<std::tuple<XLinq<S, T>, std::unordered_set<_TRet>>, T> XLinq<S, T>::distinct(F transform) const
    {
	    return XLinq<std::tuple<XLinq<S, T>, std::unordered_set<_TRet> >, T>(
		    std::make_tuple(*this, std::unordered_set<_TRet>()),
		    [transform](std::tuple<XLinq<S, T>, std::unordered_set<_TRet> >& tuple) {
			    XLinq<S, T>& linq = std::get<0>(tuple);
			    std::unordered_set<_TRet>& set = std::get<1>(tuple);

			    while (true) {
				    T value = linq.next();
				    if (set.insert(transform(value)).second) {
					    return value;
				    }
			    }
		    }
	    );
    }

    template <typename S, typename T>
    XLinq<std::tuple<XLinq<S, T>, std::unordered_set<T>>, T> XLinq<S, T>::distinct() const
    {
	    return distinct([](T value) { return value; });
    }

    template <typename S, typename T>
    template <typename F, typename _TIter>
    XLinq<std::tuple<std::vector<T>, _TIter, bool>, T> XLinq<S, T>::orderBy(F transform) const
    {
	    std::vector<T> items = toStdVector();
	    std::sort(items.begin(), items.end(), [transform](const T& a, const T& b) {
		    return transform(a) < transform(b);
	    });

	    return XLinq<std::tuple<std::vector<T>, _TIter, bool>, T>(
		    std::make_tuple(items, _TIter(), false),
		    [](std::tuple<std::vector<T>, _TIter, bool>& tuple) {
			    std::vector<T>& vec = std::get<0>(tuple);
			    _TIter& it = std::get<1>(tuple);
			    bool& flag = std::get<2>(tuple);

			    if (!flag) {
				    flag = true;
				    it = vec.cbegin();
			    }
			    if (it == vec.cend()) {
				    throw LinqEndException();
			    }
			    return *(it++);
		    }
	    );
    }

    template <typename S, typename T>
    XLinq<std::tuple<std::vector<T>, typename std::vector<T>::const_iterator, bool>, T> XLinq<S, T>::orderBy() const
    {
	    return orderBy([](T value) { return value; });
    }

    template <typename S, typename T>
    template <typename _TIter>
    XLinq<std::tuple<std::list<T>, _TIter, bool>, T> XLinq<S, T>::reverse() const
    {
	    return XLinq<std::tuple<std::list<T>, _TIter, bool>, T>(
		    std::make_tuple(toStdList(), _TIter(), false),
		    [](std::tuple<std::list<T>, _TIter, bool>& tuple) {
			    std::list<T>& list = std::get<0>(tuple);
			    _TIter& it = std::get<1>(tuple);
			    bool& flag = std::get<2>(tuple);

			    if (!flag) {
				    flag = true;
				    it = list.crbegin();
			    }
			    if (it == list.crend()) {
				    throw LinqEndException();
			    }
			    return *(it++);
		    }
	    );
    }

    template <typename S, typename T>
    template <typename TRet>
    TRet XLinq<S, T>::aggregate(TRet start, std::function<TRet(TRet, T)> accumulate) const
    {
	    XLinq<S, T> linq = *this;
	    try {
		    while (true) {
			    start = accumulate(start, linq.next());
		    }
	    }
	    catch (LinqEndException&) {}
	    return start;
    }

    template <typename S, typename T>
    template <typename F, typename _TRet>
    _TRet XLinq<S, T>::sum(F transform) const
    {
	    return aggregate<_TRet>(_TRet(), [transform](_TRet accumulator, T value) {
		    return accumulator + transform(value);
	    });
    }

    template <typename S, typename T>
    template <typename TRet>
    TRet XLinq<S, T>::sum() const
    {
	    return sum([](T value) { return TRet(value); });
    }

    template <typename S, typename T>
    template <typename F, typename _TRet>
    _TRet XLinq<S, T>::avg(F transform) const
    {
	    int count = 0;
	    _TRet res = sum([transform, &count](T value) {
		    count++;
		    return transform(value);
	    });
	    return res / count;
    }

    template <typename S, typename T>
    template <typename TRet>
    TRet XLinq<S, T>::avg() const
    {
	    return avg([](T value) { return TRet(value); });
    }

    template <typename S, typename T>
    int XLinq<S, T>::count() const
    {
	    int index = 0;
	    forEach([&index](T /*a*/) { index++; });
	    return index;
    }

    template <typename S, typename T>
    int XLinq<S, T>::count(std::function<bool(T)> predicate) const
    {
	    return where(predicate).count();
    }

    template <typename S, typename T>
    int XLinq<S, T>::count(const T& item) const
    {
	    return count([item](T value) { return item == value; });
    }

    template <typename S, typename T>
    bool XLinq<S, T>::any(std::function<bool(T)> predicate) const
    {
	    XLinq<S, T> linq = *this;
	    try {
		    while (true) {
			    if (predicate(linq.next())) {
				    return true;
			    }
		    }
	    }
	    catch (LinqEndException&) {}
	    return false;
    }

    template <typename S, typename T>
    bool XLinq<S, T>::any() const
    {
	    return any([](T value) { return static_cast<bool>(value); });
    }

    template <typename S, typename T>
    bool XLinq<S, T>::all(std::function<bool(T)> predicate) const
    {
	    return !any([predicate](T value) { return !predicate(value); });
    }

    template <typename S, typename T>
    bool XLinq<S, T>::all() const
    {
	    return all([](T value) { return static_cast<bool>(value); });
    }

    template <typename S, typename T>
    bool XLinq<S, T>::contains(const T& item) const
    {
	    return any([&item](T value) { return value == item; });
    }

    template <typename S, typename T>
    T XLinq<S, T>::elect(std::function<T(T, T)> accumulate) const
    {
	    T result;
	    forEachI([accumulate, &result](T value, int i) {
		    if (i == 0) {
			    result = value;
		    }
		    else {
			    result = accumulate(result, value);
		    }
	    });
	    return result;
    }

    template <typename S, typename T>
    template <typename F>
    T XLinq<S, T>::max(F transform) const
    {
	    return elect([transform](const T& a, const T& b) {
		    return (transform(a) < transform(b)) ? b : a;
	    });
    }

    template <typename S, typename T>
    T XLinq<S, T>::max() const
    {
	    return max([](T value) { return value; });
    }

    template <typename S, typename T>
    template <typename F>
    T XLinq<S, T>::min(F transform) const
    {
	    return elect([transform](const T& a, const T& b) {
		    return (transform(a) < transform(b)) ? a : b;
	    });
    }

    template <typename S, typename T>
    T XLinq<S, T>::min() const
    {
	    return min([](T value) { return value; });
    }

    template <typename S, typename T>
    T XLinq<S, T>::elementAt(int index) const
    {
	    return skip(index).next();
    }

    template <typename S, typename T>
    T XLinq<S, T>::firstOrDefault(std::function<bool(T)> predicate, T const& defaultValue) const
    {
	    try {
		    return where(predicate).next();
	    }
	    catch (LinqEndException&) {}
	    return defaultValue;
    }

    template <typename S, typename T>
    T XLinq<S, T>::firstOrDefault(T const& defaultValue) const
    {
	    try {
		    return XLinq<S, T>(*this).next();
	    }
	    catch (LinqEndException&) {}
	    return defaultValue;
    }

    template <typename S, typename T>
    T XLinq<S, T>::last(std::function<bool(T)> predicate) const
    {
	    T res;
	    int index = -1;
	    where(predicate).forEachI([&res, &index](T value, int i) {
		    res = value;
		    index = i;
	    });

	    if (index == -1) {
		    throw LinqEndException();
	    }
	    return res;
    }

    template <typename S, typename T>
    T XLinq<S, T>::last() const
    {
	    return last([](T /*value*/) { return true; });
    }

    template <typename S, typename T>
    T XLinq<S, T>::lastOrDefault(std::function<bool(T)> predicate, T const& defaultValue) const
    {
	    T res = defaultValue;
	    where(predicate).forEach([&res](T value) {
		    res = value;
	    });
	    return res;
    }

    template <typename S, typename T>
    T XLinq<S, T>::lastOrDefault(T const& defaultValue) const
    {
	    return lastOrDefault([](T  /*value*/) { return true; }, defaultValue);
    }

    template <typename S, typename T>
    std::vector<T> XLinq<S, T>::toStdVector() const
    {
	    std::vector<T> items;
	    forEach([&items](T value) {
		    items.push_back(value);
	    });
	    return items;
    }

    template <typename S, typename T>
    std::list<T> XLinq<S, T>::toStdList() const
    {
	    std::list<T> items;
	    forEach([&items](T value) {
		    items.push_back(value);
	    });
	    return items;
    }

    template <typename S, typename T>
    QList<T> XLinq<S, T>::toList() const
    {
        QList<T> items;
        forEach([&items](T value) {
            items.append(value);
            });
        return items;
    }

    template <typename S, typename T>
    XLinq<std::tuple<XLinq<S, T>, BytesDirection, BitsDirection, T, int>, int> XLinq<S, T>::bits(BitsDirection bitsDir,
	    BytesDirection bytesDir) const
    {
	    return XLinq<std::tuple<XLinq<S, T>, BytesDirection, BitsDirection, T, int>, int>(
		    std::make_tuple(*this, bytesDir, bitsDir, T(), sizeof(T) * CHAR_BIT),
		    [](std::tuple<XLinq<S, T>, BytesDirection, BitsDirection, T, int>& tuple) {
			    XLinq<S, T>& linq = std::get<0>(tuple);
			    BytesDirection& bytesDirection = std::get<1>(tuple);
			    BitsDirection& bitsDirection = std::get<2>(tuple);
			    T& value = std::get<3>(tuple);
			    int& index = std::get<4>(tuple);

			    if (index == sizeof(T) * CHAR_BIT) {
				    value = linq.next();
				    index = 0;
			    }

			    unsigned char* ptr = reinterpret_cast<unsigned char*>(&value);

			    int byteIndex = index / CHAR_BIT;
			    if (bytesDirection == BytesLastToFirst) {
				    byteIndex = sizeof(T) - 1 - byteIndex;
			    }

			    int bitIndex = index % CHAR_BIT;
			    if (bitsDirection == BitsHighToLow) {
				    bitIndex = CHAR_BIT - 1 - bitIndex;
			    }

			    index++;
			    return (ptr[byteIndex] >> bitIndex) & 1;
		    }
	    );
    }

    template <typename S, typename T>
    template <typename TRet>
    XLinq<std::tuple<XLinq<S, T>, BytesDirection, BitsDirection, int>, TRet> XLinq<S, T>::unbits(BitsDirection bitsDir,
	    BytesDirection bytesDir) const
    {
	    return XLinq<std::tuple<XLinq<S, T>, BytesDirection, BitsDirection, int>, TRet>(
		    std::make_tuple(*this, bytesDir, bitsDir, 0),
		    [](std::tuple<XLinq<S, T>, BytesDirection, BitsDirection, int>& tuple) {
			    XLinq<S, T>& linq = std::get<0>(tuple);
			    BytesDirection& bytesDirection = std::get<1>(tuple);
			    BitsDirection& bitsDirection = std::get<2>(tuple);
			    // int &index = std::get<3>(tuple);

			    TRet value = TRet();
			    unsigned char* ptr = reinterpret_cast<unsigned char*>(&value);

			    for (int i = 0; i < sizeof(TRet) * CHAR_BIT; i++) {
				    int byteIndex = i / CHAR_BIT;
				    if (bytesDirection == BytesLastToFirst) {
					    byteIndex = sizeof(TRet) - 1 - byteIndex;
				    }

				    int bitIndex = i % CHAR_BIT;
				    if (bitsDirection == BitsHighToLow) {
					    bitIndex = CHAR_BIT - 1 - bitIndex;
				    }

				    ptr[byteIndex] &= ~(1 << bitIndex);
				    ptr[byteIndex] |= bool(linq.next()) << bitIndex;
			    }

			    return value;
		    }
	    );
    }


    template<typename S, typename T>
    std::ostream& operator<<(std::ostream& stream, XLinq<S, T> linq)
    {
        try {
            while (true) {
                stream << linq.next() << ' ';
            }
        }
        catch (LinqEndException&) {}
        return stream;
    }

    ////////////////////////////////////////////////////////////////
    // XLinq Creators
    ////////////////////////////////////////////////////////////////

    template<typename T>
    XLinq<std::pair<T, T>, typename std::iterator_traits<T>::value_type> from(const T& begin, const T& end)
    {
        return XLinq<std::pair<T, T>, typename std::iterator_traits<T>::value_type>(
            std::make_pair(begin, end),
            [](std::pair<T, T>& pair) {
                if (pair.first == pair.second) {
                    throw LinqEndException();
                }
                return *(pair.first++);
            }
        );
    }

    template<typename T>
    XLinq<std::pair<T, T>, typename std::iterator_traits<T>::value_type> from(const T& it, int n)
    {
        return from(it, it + n);
    }

    template<typename T, int N>
    XLinq<std::pair<const T*, const T*>, T> from(T(&array)[N])
    {
        return from((const T*)(&array), (const T*)(&array) + N);
    }

    template<template<class> class TV, typename TT>
    auto from(const TV<TT>& container)
        -> decltype(from(container.cbegin(), container.cend()))
    {
        return from(container.cbegin(), container.cend());
    }

    // std::list, std::vector, std::dequeue
    template<template<class, class> class TV, typename TT, typename TU>
    auto from(const TV<TT, TU>& container)
        -> decltype(from(container.cbegin(), container.cend()))
    {
        return from(container.cbegin(), container.cend());
    }

    // std::set
    template<template<class, class, class> class TV, typename TT, typename TS, typename TU>
    auto from(const TV<TT, TS, TU>& container)
        -> decltype(from(container.cbegin(), container.cend()))
    {
        return from(container.cbegin(), container.cend());
    }

    // std::map
    template<template<class, class, class, class> class TV, typename TK, typename TT, typename TS, typename TU>
    auto from(const TV<TK, TT, TS, TU>& container)
        -> decltype(from(container.cbegin(), container.cend()))
    {
        return from(container.cbegin(), container.cend());
    }

    //-----------------------------------------------------------------------------
    // Author:  Tobias Post
    // Company: CUBE4DEV GmbH
    // Date:    08.04.2022
    // Context: DefaultNamespace
    // Class:   XLinq
    // Method:  from
    // Params:  const TV<TT, TL>& container
    //----------------------------------------------------------------------------- 
    
    // std::array
    template<template<class, size_t> class TV, typename TT, size_t TL>
    auto from(const TV<TT, TL>& container)
        -> decltype(from(container.cbegin(), container.cend()))
    {
        return from(container.cbegin(), container.cend());
    }

    //-----------------------------------------------------------------------------
    // Author:  Tobias Post
    // Company: CUBE4DEV GmbH
    // Date:    08.04.2022
    // Context: DefaultNamespace
    // Class:   XLinq
    // Method:  repeat
    // Params:  const T& value
    // Params:  int count
    //----------------------------------------------------------------------------- 
    template<typename T>
    XLinq<std::pair<T, int>, T> repeat(const T& value, int count) {
        return XLinq<std::pair<T, int>, T>(
            std::make_pair(value, count),
            [](std::pair<T, int>& pair) {
                if (pair.second > 0) {
                    pair.second--;
                    return pair.first;
                }
                throw LinqEndException();
            }
        );
    }

    //-----------------------------------------------------------------------------
    // Author:  Tobias Post
    // Company: CUBE4DEV GmbH
    // Date:    08.04.2022
    // Context: DefaultNamespace
    // Class:   XLinq
    // Method:  range
    // Params:  const T& start
    // Params:  const T& end
    // Params:  const T& step
    //----------------------------------------------------------------------------- 
    
    template<typename T>
    XLinq<std::tuple<T, T, T>, T> range(const T& start, const T& end, const T& step) {
        return XLinq<std::tuple<T, T, T>, T>(
            std::make_tuple(start, end, step),
            [](std::tuple<T, T, T>& tuple) {
                T& start = std::get<0>(tuple);
                T& end = std::get<1>(tuple);
                T& step = std::get<2>(tuple);

                T value = start;
                if (value < end) {
                    start += step;
                    return value;
                }
                throw LinqEndException();
            }
        );
    }
}

using namespace CoreBase;
