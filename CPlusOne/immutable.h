//
//  immutable.h
//  CPlusOne
//
//  Copyright (c) 2013 Sami Rosendahl. All rights reserved.
//

#ifndef CPlusOne_immutable_h
#define CPlusOne_immutable_h

template<typename S>
class MutatorBase;

template<typename S, typename T, typename U>
class Mutator;

template<typename S>
class Immutable {
    S s;
public:
    explicit Immutable(const S& s) : s(s) { }
    Immutable(const MutatorBase<S>& m);

    const S* operator->() const { return &s; }
    operator const S&() const { return s; }

    template<typename T, typename U>
    Mutator<S, T, U> set(T S::*field, const U& val);

private:
    // Assignment not supported to retain immutability
    Immutable<S>& operator=(const Immutable<S>&);
};

template<typename S>
class MutatorBase {
public:
    const S& s;
    MutatorBase(const S& s) : s(s) { }
    virtual void apply(S& to) const = 0;
protected:
    ~MutatorBase() { }
};

template<typename S, typename T, typename U>
class Mutator : public MutatorBase<S> {
    T S::*field;
    const U& val;
    MutatorBase<S>* prev;
public:
    explicit Mutator(const S& s, T S::*field, const U& val, MutatorBase<S>* prev)
        : MutatorBase<S>(s), field(field), val(val), prev(prev) { }

    template<typename T2, typename U2>
    Mutator<S, T2, U2> set(T2 S::*field, const U2& val) {
        return Mutator<S, T2, U2>(this->s, field, val, this);
    }

    virtual void apply(S& to) const override {
        to.*field = val;
        if (prev)
            prev->apply(to);
    }
};

template<typename S>
Immutable<S>::Immutable(const MutatorBase<S>& m) : Immutable(m.s) {
    m.apply(s);
}

template<typename S>
template<typename T, typename U>
Mutator<S, T, U> Immutable<S>::set(T S::*field, const U& val) {
    return Mutator<S, T, U>(s, field, val, nullptr);
}

template<typename S>
Immutable<S> make_immutable(const S& s) {
    return Immutable<S>(s);
}

#endif
