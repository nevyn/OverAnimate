#ifndef OVERANIMATE__H
#define OVERANIMATE__H
#include <string.h>
class AnimationSystem;

/// Time in seconds.
typedef double TimeInterval;

/*!
 * @class Animation
 * An abstract animation, which has a start time and duration.
 * Subclass and implement animate.
 */
class Animation
{
    friend class AnimationSystem;
public:
    /// When, in the time space of the system, should this animation start?
    TimeInterval beginTime;
    /// And for how many seconds should the animation be?
    TimeInterval duration;
    /// Is the animation part of a system right now? It is true from the time you say
    /// addAnimation, and becomes false after the system time is over `beginTime+duration`.
    bool scheduled;

    /// if not enabled, animation will not be processed
    bool enabled;
    
    /// Does the animation repeat? If so, beginTime is set to now() instead of removing it upon end.
    bool repeats;
    
    Animation(TimeInterval duration = 1.0, bool repeats = false);
protected:
    // Override this. After the system's time is over `beginTime`, but under
    // `beginTime+duration`, this function will be called every "frame" with the
    // fraction of completion as argument.
    virtual void animate(float fraction) = 0;
};

typedef void(*AnimationFunction)(Animation*, int, float);
/*! @class BoundFunctionAnimation
 *  Simple helper animation that takes a function and an int,
 *  and during animation, calls the function with the int and the elapsed time.
 *  Example use case: the int is the pin number to animate :)
 */
class BoundFunctionAnimation : public Animation
{
public:
    AnimationFunction function;
    int boundArgument;
    BoundFunctionAnimation(AnimationFunction func, int boundArgument, TimeInterval duration = 1.0, bool repeats = false);
protected:
    void animate(float fraction);
};

/// Since Arduino has no memory allocation, this sytem only supports a fixed number of animations.
static const int kMaxAnimationCount = 64;
/*! @class AnimationSystem
 *  A group of animations, running in a time space. Time starts at 0, and moves forward
 *  at each invocation of playElapsedTime.
 */
class AnimationSystem
{
public:
    AnimationSystem();
    
    /// What's the sum of all deltas added? Animations added at now() will start playing immediately.
    float now();
    
    /// Schedule an animation for playback. Its `scheduled` flag will be set, and then un-set after it has
    /// finished playing. Returns the index of the animation in the current runlist, or -1 if
    /// we couldn't add the animation.
    int addAnimation(Animation *animation);
    
    // removes an animation based on its index in the runlist.
    void removeAnimation(int i);
    // removes an animation, which stops it from running
    void removeAnimation(Animation *animation);
    
    /// Time has elapsed; add it to the internal clock and perform animations
    /// accordingly. If animations have ended, set their `scheduled` to false.
    /// Please call this method often with small deltas.
    void playElapsedTime(float deltaAdded);
    
private:
    Animation* _animations[kMaxAnimationCount];
    TimeInterval _elapsedTime;
    int _lowestAvailableIndex;
};

#endif
