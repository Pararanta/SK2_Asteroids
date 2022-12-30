#ifndef H_ASTEROIDS_UTILS_INTERPOLATE
#define H_ASTEROIDS_UTILS_INTERPOLATE
    
    typedef float vec2[2];

    typedef struct SecondOrderSystem
    {
        // using pointers may be faster
        float target_position;
        float target_velocity;

        float apparent_position;
        float apparent_velocity;

        float frequency;
        float damping;
        float response;

        float k1;
        float k2;
        float k3;
    }
    SecondOrderSystem;

    typedef struct Interpolation
    {
        int * enabled;
        SecondOrderSystem * system;

        float * in_position;
        float * in_velocity;

        float * out_position;
    } Interpolation;

    void SOSInit(SecondOrderSystem * system, float position, float velocity);
    void SOSSetParameters(SecondOrderSystem * system, float frequency, float damping, float response);
    void SOSSolve(SecondOrderSystem * system, float time_step);
    void InterpolationInit(Interpolation * interpolation, SecondOrderSystem * system, float * in_position, float * in_velocity, float * out_position);
    void InterpolationUpdate(Interpolation * interpolation, float time_step);
#endif