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
class InitialMutator;

template<typename S>
class Immutable {
    S s;
public:
    explicit Immutable(const S& s) : s(s) { }
    Immutable(const MutatorBase<S>& m);
    // Assignment not supported to retain immutability
    Immutable<S>& operator=(const Immutable<S>&) = delete;

    const S* operator->() const { return &s; }
    operator const S&() const { return s; }

    template<typename T, typename U>
    InitialMutator<S, T, U> set(T S::*field, const U& val);
};

template<typename S>
class MutatorBase {
public:
    virtual void apply(S& to) const = 0;
    virtual const S& initial() const = 0;
protected:
    ~MutatorBase() { }
};

template<typename S, typename T, typename U>
class Mutator : public MutatorBase<S> {
    T S::*field;
    const U& val;
    MutatorBase<S>* prev;
public:
    explicit Mutator(T S::*field, const U& val, MutatorBase<S>* prev)
        : field(field), val(val), prev(prev) { }

    template<typename T2, typename U2>
    Mutator<S, T2, U2> set(T2 S::*field, const U2& val) {
        return Mutator<S, T2, U2>(field, val, this);
    }

    void apply(S& to) const override {
        if (prev)
            prev->apply(to);
        to.*field = val;
    }

    const S& initial() const override {
        return prev->initial();
    }
};

template<typename S, typename T, typename U>
class InitialMutator : public Mutator<S, T, U> {
    const S& s;
public:
    explicit InitialMutator(const S& s, T S::*field, const U& val)
        : Mutator<S, T, U>(field, val, nullptr), s(s) { }

    const S& initial() const override {
        return s;
    }
};

template<typename S>
Immutable<S>::Immutable(const MutatorBase<S>& m)
    : s(m.initial()) {
    m.apply(s);
}

template<typename S>
template<typename T, typename U>
InitialMutator<S, T, U> Immutable<S>::set(T S::*field, const U& val) {
    return InitialMutator<S, T, U>(this->s, field, val);
}

template<typename S>
Immutable<S> make_immutable(const S& s) {
    return Immutable<S>(s);
}

#endif
