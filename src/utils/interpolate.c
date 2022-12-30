#define _USE_MATH_DEFINES

#include "interpolate.h"
#include "math.h"

void SOSInit(SecondOrderSystem * system, float position, float velocity)
{
    system->target_position = position;
    system->target_velocity = velocity;

    system->apparent_position = position;
    system->apparent_velocity = velocity;
}

void SOSSetParameters(SecondOrderSystem * system, float frequency, float damping, float response)
{
    system->frequency = frequency;
    system->damping = damping;
    system->response = response;

    system->k1 = damping/(M_PI*frequency);

    system->k2 = (2.0f*M_PI*frequency);
    system->k2 = system->k2*system->k2;
    system->k2 = 1.0f/system->k2;

    system->k3 = (response*damping)/(2*M_PI*frequency);
}

void SOSSolve(SecondOrderSystem * system, float time_step)
{
    system->apparent_position += system->apparent_velocity * time_step;
    float acceleration =
        (
            (system->target_position + system->k3 * system->target_velocity)
            -
            (system->apparent_position + system->k1 * system->apparent_velocity)
        )
        / system->k2;
    system->apparent_velocity += acceleration*time_step;
}

void InterpolationInit(Interpolation * interpolation, SecondOrderSystem * system, float * in_position, float * in_velocity, float * out_position)
{
    interpolation->system = system;
    interpolation->in_position = in_position;
    interpolation->in_velocity = in_velocity;
    interpolation->out_position = out_position;
}

void InterpolationUpdate(Interpolation * interpolation, float time_step)
{
    if(!*interpolation->enabled)
        return;

    interpolation->system->target_position = *interpolation->in_position;
    interpolation->system->target_velocity = *interpolation->in_velocity;
    SOSSolve(&interpolation->system, time_step);
    *interpolation->out_position = interpolation->system->apparent_position;
}
