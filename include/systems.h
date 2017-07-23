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
 * This is a function that draws things to the screen. It will not perform any
 * action with an Entity with flag_hidden set to true. The funciton system_draw
 * orders everything into a priority queue, confirmDraw will actually draw the
 * things.
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
void confirmDraw();

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
 * - Callum Nicholsons (hamsterjam)
 *
 */
void system_kinematics(ECS& ecs, unsigned int deltaT);

#endif
