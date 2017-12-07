/*
 * HARP
 * systems.h
 *
 * These are the forward declares for functions that act as Systems for the
 * entity component system.
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

#ifndef HARP_SYSTEM_DRAW_H
#define HARP_SYSTEM_DRAW_H

class ECS;

/*
 * Passes keyboard input stuff to a function that decides on how it wants to
 * to ahndle the movement. If acceptingInput is set to true, it will read
 * keyboard input, otherwise it will act as if no keys are pressed.
 *
 * Operates on any Enitty with:
 *
 *      comp_inputFunction
 *
 *
 * - Callum Nicholson (hamsterjam)
 *
 */
void system_input(ECS& ecs, bool acceptingInput);

/*
 * This is a function that draws things to the screen. It will not perform any
 * action with an Entity with flag_hidden set to true.
 *
 * Operates on any Entity with:
 *
 *     comp_position
 *     comp_visual
 *
 *
 * - Callum Nicholson (hamsterjam)
 *
 */
void system_draw(ECS& ecs);

/*
 * One half of a physics simulation (the other being kinematics). The purpose
 * of this function is essentially to calculate and apply forces.
 *
 * Operates on any Entity with:
 *
 *      comp_acceleration
 *
 *
 * - Callum Nicholson (hamsterjam)
 *
 */
void system_dynamics(ECS& ecs);

/*
 * This is a function that performs very simple kinematics, that is, it moves
 * each Entity around based on its velocity and acceleration.
 *
 * Operates on any Entity with:
 *
 *     comp_position
 *     comp_velocity
 *
 * and any Entity with:
 *
 *     comp_velocity
 *     comp_acceleration
 *
 *
 * - Callum Nicholson (hamsterjam)
 *
 */
void system_kinematics(ECS& ecs, unsigned int deltaT, bool partialStep);

/*
 * Detects and rectifies collisions, for now this just means that it stops
 * everything from going off the bottom of the screen, I'll do something more
 * elaborate later
 *
 * - Callum Nicholson (hamsterjam)
 *
 */
void system_collision(ECS& ecs);

/*
 * Fudges components to give them exact values if it's something that should be
 * able to be exact but wont be for precision reasons
 *
 * Operates on any Entity with
 *
 *      comp_velocity
 *
 *
 * - Callum Nicholson (hamsterjam)
 *
 */
void system_fudge(ECS& ecs);

#endif
