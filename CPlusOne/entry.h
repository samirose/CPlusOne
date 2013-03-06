//
//  entry.h
//  CPlusOne
//
//  Copyright (c) 2013 Sami Rosendahl. All rights reserved.
//

#ifndef CPlusOne_entry_h
#define CPlusOne_entry_h

#include <string>

struct Address {
    std::string street;
    std::string postalCode;
    std::string city;
    std::string country;
};

struct Entry {
    Entry() : numCallsFrom(0), numCallsTo(0) {}
    std::string givenName;
    std::string familyName;
    std::string phone;
    int numCallsTo;
    int numCallsFrom;
    Address address;
};

#endif
