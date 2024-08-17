/*
[notes]
3 - create a class (blueprint) for representing vectors (a direction with a magnitude, ordered list of numbers, etc)
***outside resources used***
Fundamentals of Computer Graphics (ch1) + adhoc googling
https://www.learncpp.com/cpp-tutorial/header-guards/

https://gamemath.com/book/vectors.html
linear algebra: vectors and their operations
    - negating (geometric: going the opposite direction an equal amount)
    - scalar multiplcation (stretching)
    - addition and subtraction (sequence of displacements)
    - magnitude (length)
    - multiplication (dot product, multiply corresponding components)

pointer types, reference types, and the related operators
https://www.learncpp.com/cpp-tutorial/introduction-to-pointers/
https://www.learncpp.com/cpp-tutorial/lvalue-references/
    int x{ 5 };      // create an integer with value 5 called x
    int& ref { x };  // get a reference to x, AKA manipulating this ref also manipulates x, an alias
    int* ptr { &x }; // get a pointer to x, recall pointer types MUST store memory addresses
                        // & on the RHS acts as an operator, providing the memory address of something

https://www.learncpp.com/cpp-tutorial/const-class-objects-and-const-member-functions/

overloading functions
https://www.learncpp.com/cpp-tutorial/introduction-to-overloading-the-i-o-operators/
https://www.learncpp.com/cpp-tutorial/overloading-the-subscript-operator/

https://www.learncpp.com/cpp-tutorial/inline-functions-and-variables/
*/

// use of float vs double up to preference and constraints

// two lines below are a header guard and the preprocessor (something before compilation) 
// checks if this class was defined already
// motivation: avoid defining something twice
#ifndef VEC3_H // if we haven't defined this vector class then go ahead and define it
#define VEC3_H // otherwise ignore the contents of the header

#include "rtweekend.h"

class vec3 {
public:
    // member variables
    double e[3];

    // constructors
    vec3() : e{ 0,0,0 } {}
    vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

    // getters: accessing the member variables
    // const means the member function CAN'T change any object or call other non-const member functions
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // negate a vector's components
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; } // read access, return a copy
    double& operator[](int i) { return e[i]; }      // write access, return a reference (great for large objects)
    /*
    * basically & and * change meaning depending on whether they're used as an operator or on a data type
    [aside: &, address-of operator and lvalue reference]
        suppose we have
        int x = 5;
        std::cout << x << '\n';
        this would print 5        
            - basically the compiler translates the name of the variable to the memory address for us

        std::cout << &x << '\n';
        this prints the memory address of x
        & (address-of operator)    
    
        if used like this, DECLARING A REFERENCE TYPE
        char& ref = x;
        then ref is directly referencing the memory address of x
        it's NOT a seperate variable with its own memory address holding its own 5
        so using ref is like using x itself

    [aside: pointers, * (dereference operator)]
        pointer: an object that holds a memory address
        what does it do as an operator:
            return the value at a given memory address

        if used like this
        int x = 5
        printing &x results in the memory address of x
        but if we dereference &x, like *(&x) then we would just get 5
        
        if used like this, POINTER TYPES
        int whatever = 9;
        int* x = &whatever, the pointer is initialized with the memory address of whatever, holding 9
        so to print 9 using x we would need to dereference x
        printing *x would get us 9

    [aside: overloading subscript operator ([])]
        motivation: we can't directly modify member variable arrays if we're practicing encapsulation
                    and creating getters/setters looks really awkward syntactically
                    so fuck it just overload the [] operator
        implementation:
        ex:
            int operator[](int i) {return array[i]}
    */
    
    // compound assignment operators
    // e = (0,0,0)
    // v = (1,2,3)
    // compound vector addition: 
    // e += v results in e becoming (1,2,3)?
    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }
    // compound scalar multiplication, /= representing scaling by the inverse of the scalar
    vec3& operator*=(double t) { 
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }
    vec3& operator/=(double t) {
        return *this *= 1 / t;
    }

    // magnitude (length)
    double length() const {
        return std::sqrt(length_squared());
    }
    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;


// Vector Utility Functions

/*
aside: inline functions, inline expansion
    the compiler can replace a function call with the code itself if it thinks there's a performance boost
    it's a form of premature optimization which could hurt the optimization overall 
    just add inline before the member method return type

    okay well who am i to argue with code written by literal gods of the field
*/

// helps print the vector's components, taking in print text and the vector w/o components to print
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// actual implementations of the vector operations to map the syntax to actual linear algebra
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
// * overloaded multiple times for dot product, scalar multiplication
inline vec3 operator*(const vec3& u, const vec3& v) { 
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
inline vec3 operator*(double t, const vec3& v) { 
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}
inline vec3 operator/(const vec3& v, double t) {
    return (1 / t) * v;
}

// use the overloaded * above that corresponds to the arguments
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

// motivation: what if we needed a vector perpendicular to u and v?
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// motivation: what if we needed to find a vector in the same direction as another, but length 1?
// constructor, completely shit myself that this existed
inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

#endif // end of header guard