//
//  main.cpp
//  Effective C++ 1
//
//  Created by Chuanheng He on 2016-09-19.
//  Copyright © 2016 Chuanheng He. All rights reserved.
//

#include <iostream>
//#include <boost/type_index.hpp>

int main(int argc, const char * argv[]) {
    // insert code here...
    
    const int theAnswer = 42;
    
    auto x = theAnswer;
    auto y = &theAnswer;
    
    std::cout << typeid(x).name() << '\n'; // display types for
    std::cout << typeid(y).name() << '\n'; // x and y
    
    return 0;
}
