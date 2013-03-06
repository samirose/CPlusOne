//
//  main.cpp
//  CPlusOne
//
//  Copyright (c) 2013 Sami Rosendahl. All rights reserved.
//

#include <cassert>
#include "entry.h"
#include "immutable.h"

void test_conversion(const Entry& e) {
    assert(e.familyName == "Brown");
}

int main(int argc, const char * argv[])
{
    Entry e;
    e.givenName = "John";
    e.familyName = "Doe";

    Immutable<Entry> ie1(e);
    assert(ie1->givenName == "John");
    assert(ie1->familyName == "Doe");

    Immutable<Entry> ie2 = ie1.set(&Entry::givenName, "Jackie")
                              .set(&Entry::familyName, std::string("Brown"));
    assert(e.givenName == "John");
    assert(e.familyName == "Doe");
    assert(ie1->givenName == "John");
    assert(ie1->familyName == "Doe");
    assert(ie2->givenName == "Jackie");
    assert(ie2->familyName == "Brown");

    Immutable<Entry> ie3 = ie2.set(&Entry::address, Address { "Penny Lane", "12345", "London", "GB" });
    assert(ie2->address.city == "");
    assert(ie3->address.city == "London");
    assert(ie2->givenName == "Jackie");
    assert(ie2->familyName == "Brown");
    assert(ie3->givenName == "Jackie");
    assert(ie3->familyName == "Brown");

    Immutable<Entry> ie4 = ie3.set(&Entry::numCallsTo, 42)
                              .set(&Entry::givenName, "Charlie");
    assert(ie3->numCallsTo == 0);
    assert(ie3->givenName == "Jackie");
    assert(ie4->numCallsTo == 42);
    assert(ie4->givenName == "Charlie");
    assert(ie4->familyName == "Brown");
    test_conversion(ie4);
    return 0;
}
