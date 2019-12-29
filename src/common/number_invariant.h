/**
 * @file Header file containing interface for number invariant
 */
#ifndef APP_NUMBER_INVARIANT_H_
#define APP_NUMBER_INVARIANT_H_

/**
 * Enumeration to specify how to interpret invariant number type
 */
enum num_invariant_type {
    NUMBER_INVARIANT_INT, //!< Invariant number is integer
    NUMBER_INVARIANT_FLOAT//!< Invariant number is floating point number
};

/**
 * Structure that contains either float or integer number
 */
typedef struct number_invariant_struct {
    enum num_invariant_type type;
    union {
        float float_repr;
        int16_t int_repr;
    };
} number_invariant_t;

#endif /* APP_NUMBER_INVARIANT_H_ */
