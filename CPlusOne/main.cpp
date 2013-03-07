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
    auto ie = make_immutable(e);
    assert(ie->givenName == "John");
    assert(ie->familyName == "Doe");
}

void test_field_mutation() {
    Entry e;
    e.givenName = "John";
    e.familyName = "Doe";
    auto ie1 = make_immutable(e);
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
    auto ie1 = make_immutable(e);
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
    auto ie1 = make_immutable(e);
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

struct CopyCounted {
    int val1, val2;
    static int copyCount;
    CopyCounted() : val1(0), val2(0) { }
    CopyCounted(const CopyCounted& src)
    : val1(src.val1), val2(src.val2) {
        ++copyCount;
    }
};

int CopyCounted::copyCount = 0;

void test_make_immutable_does_not_increase_copy_count() {
    CopyCounted::copyCount = 0;
    CopyCounted cc;
    Immutable<CopyCounted> icc = make_immutable(cc);
    assert(icc->val1 == 0);
    // Wrapping struct to immutable creates one copy
    assert(CopyCounted::copyCount == 1);
}

void test_single_copy_taken_per_assignment_chain() {
    CopyCounted::copyCount = 0;
    CopyCounted cc;
    Immutable<CopyCounted> icc1(cc);
    // Wrapping struct to immutable creates one copy
    assert(CopyCounted::copyCount == 1);
    Immutable<CopyCounted> icc2 = icc1.set(&CopyCounted::val1, 42)
                                      .set(&CopyCounted::val2, 53);
    assert(icc2->val1 == 42);
    assert(icc2->val2 == 53);
    // Although multiple fields were set, only one copy of struct is made
    assert(CopyCounted::copyCount == 2);
}

int main(int argc, const char * argv[])
{
    test_field_access_operator();
    test_field_mutation();
    test_substructure_mutation();
    test_mixed_field_types();
    test_conversion();
    test_make_immutable_does_not_increase_copy_count();
    test_single_copy_taken_per_assignment_chain();
    return 0;
}
