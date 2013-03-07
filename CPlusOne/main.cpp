//
//  main.cpp
//  CPlusOne
//
//  Copyright (c) 2013 Sami Rosendahl. All rights reserved.
//

#include <cassert>
#include "entry.h"
#include "immutable.h"

void test_field_access_operator() {
    Entry e;
    e.givenName = "John";
    e.familyName = "Doe";
    Immutable<Entry> ie(e);
    assert(ie->givenName == "John");
    assert(ie->familyName == "Doe");
}

void test_field_mutation() {
    Entry e;
    e.givenName = "John";
    e.familyName = "Doe";
    Immutable<Entry> ie1(e);
    Immutable<Entry> ie2 = ie1.set(&Entry::givenName, "Jackie")  // Conversion const char* -> std::string should be applied
                              .set(&Entry::familyName, std::string("Brown"));
    assert(e.givenName == "John");
    assert(e.familyName == "Doe");
    assert(ie1->givenName == "John");
    assert(ie1->familyName == "Doe");
    assert(ie2->givenName == "Jackie");
    assert(ie2->familyName == "Brown");
}

void test_substructure_mutation() {
    Entry e;
    e.givenName = "Jackie";
    e.familyName = "Brown";
    Immutable<Entry> ie1(e);
    Immutable<Entry> ie2 = ie1.set(&Entry::address, Address { "Penny Lane", "12345", "London", "GB" });
    assert(ie1->address.city == "");
    assert(ie2->address.city == "London");
    assert(ie1->givenName == "Jackie");
    assert(ie1->familyName == "Brown");
    assert(ie2->givenName == "Jackie");
    assert(ie2->familyName == "Brown");
}

void test_mixed_field_types() {
    Entry e;
    e.givenName = "Jackie";
    e.familyName = "Brown";
    Immutable<Entry> ie1(e);
    Immutable<Entry> ie2 = ie1.set(&Entry::numCallsTo, 42)
                              .set(&Entry::givenName, "Charlie");
    assert(ie1->numCallsTo == 0);
    assert(ie1->givenName == "Jackie");
    assert(ie2->numCallsTo == 42);
    assert(ie2->givenName == "Charlie");
    assert(ie2->familyName == "Brown");
}

void test_conversion(const Entry& e) {
    assert(e.familyName == "Brown");
}

void test_conversion() {
    Entry e;
    e.givenName = "Jackie";
    e.familyName = "Brown";
    Immutable<Entry> ie(e);
    test_conversion(ie);
}

int main(int argc, const char * argv[])
{
    test_field_access_operator();
    test_field_mutation();
    test_substructure_mutation();
    test_mixed_field_types();
    test_conversion();
    return 0;
}
