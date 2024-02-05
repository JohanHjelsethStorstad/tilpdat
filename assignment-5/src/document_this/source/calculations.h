/**
 * @file
*/
#ifndef CALCULATIONS_H
#define CALCUATIONS_H
#include "system.h"

/**
    
 * @brief calculates P gain based on a system
 * @param[in] current_system
 * @return P gain
 */
double calculate_P_parameter(System * current_system);

/**
 * @brief calculates I gain based on a system
 * @param[in] current_system
 * @return I gain
 */
double calculate_I_parameter(System * current_system);

/**
 * @brief calculates D gain based on a system
 * @param[in] current_system
 * @return D gain
 */
double calculate_D_parameter(System * current_system);

#endif
