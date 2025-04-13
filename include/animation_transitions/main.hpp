#include <iostream>

int main(int, char **);

enum AnimationType
{
    Slide = 0,
    Bounce,
    Zoom,
    Fade,
    Orbit,
    Pulse,
    Jitter,
    AnimationType_Count
};

struct SlideParams
{
    float speed = 1.0f, amplitude = 100.0f;
};
struct BounceParams
{
    float speed = 1.0f, height = 80.0f;
};
struct ZoomParams
{
    float speed = 1.0f, minScale = 0.7f, maxScale = 1.5f;
};
struct FadeParams
{
    float speed = 1.0f, minAlpha = 0.2f, maxAlpha = 1.0f;
};
struct OrbitParams
{
    float speed = 1.0f, radius = 100.0f;
};
struct PulseParams
{
    float speed = 1.0f, minScale = 0.7f, maxScale = 1.5f;
};
struct JitterParams
{
    float intensity = 4.0f, frequency = 30.0f;
};

struct AnimationParameters
{
    SlideParams slide;
    BounceParams bounce;
    ZoomParams zoom;
    FadeParams fade;
    OrbitParams orbit;
    PulseParams pulse;
    JitterParams jitter;
};

inline float easeInOut(float t)
{
    return t * t * (3.0f - 2.0f * t);
}