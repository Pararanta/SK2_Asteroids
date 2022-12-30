#include <stdint.h>
#include <stdio.h>
#include <interpolate.h>

#define EPSILON_SQUARED EPSILON*EPSILON

int exit_code = 0;

int assert(int assertion, const char * error)
{
    
    printf("Assertion %s: %s\n", assertion ? "Succeeded" : "Failed", error);

    if(assertion)
        return 0;

    exit_code = 1;
    return 1;
}

void testConverges()
{
    float target_position = 0.0f;
    float target_velocity = 0.0f;
    float apparent_position = 2.0f;
    SecondOrderSystem test_object_a = {0};
    Interpolation test_object_b = {0};

    SOSInit(&test_object_a, target_position, target_velocity);
    SOSSetParameters(&test_object_a, 1.0f, 1.0f, 1.0f);
    InterpolationInit(&test_object_b, &test_object_a, &target_position, &target_velocity, &apparent_position);

    target_position = 1.0f;
    target_velocity = 100.0f;

    for(int i = 0; i < STEPS; i++)
        InterpolationUpdate(&test_object_b, TIME_STEP);

    float delta = target_position-apparent_position;
    float delta_squared = delta*delta;
    assert(delta_squared < EPSILON_SQUARED, "interpolation does not converge");
}

int main()
{
    testConverges();
    return exit_code;
}