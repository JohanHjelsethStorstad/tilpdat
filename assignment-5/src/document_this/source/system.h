/**
 * @file
*/

#ifndef SYSTEM_H
#define SYSTEM_H

/**
 * @brief All tuning methds implemented
 */
typedef enum {
    Classic_Ziegler_Nichols,
    Pessen_Integral_Rule
} Tuning;

/**
 * @brief A structure to represent a system and desired PID tuning method.
 */
typedef struct {
    double Ku;            /** < The gain margin for loop stability */
    double Tu;            /** < The period of the oscillations frequency at the stability limit */
    Tuning tuning_method; /** < The desired PID tuning method */
} System;


/**
 * @brief Creates a System struct allocated on the heap
 * @param[in] Ku Proportional gain for system
 * @param[in] Tu Time constant for System
 * @param[in] tuning_methode What tuning method the system uses of enum type Tuning. 
 * @return pointer to a new System
 */
System * system_create(double Ku, double Tu, Tuning tuning_method);

/**
 * @brief Deallocates syatem
 * @param[out] system_current
 */
void system_delete(System * system_current);

#endif
