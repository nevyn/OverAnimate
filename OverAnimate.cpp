#include "OverAnimate.h"

Animation::
Animation(TimeInterval duration, bool repeats) :
    beginTime(0),
    duration(duration),
    scheduled(0),
    enabled(1),
    repeats(repeats)
{

}

void
Animation::
animate(
    float fraction
) {

}

#pragma mark -

BoundFunctionAnimation::
BoundFunctionAnimation(
    AnimationFunction func,
    int boundArgument,
    TimeInterval duration,
    bool repeats
) :
    Animation(duration, repeats),
    function(func),
    boundArgument(boundArgument)
{

}

void
BoundFunctionAnimation::
animate(
    float fraction
) {
    this->function(this, boundArgument, fraction);
}

#pragma mark -

AnimationSystem::
AnimationSystem() :
    _lowestAvailableIndex(0),
    _elapsedTime(0)
{
    memset(_animations, 0, kMaxAnimationCount*sizeof(Animation*));
}

TimeInterval
AnimationSystem::
now()  {
    return _elapsedTime;
}

int
AnimationSystem::
addAnimation(
    Animation *animation
) {
    if(animation->scheduled) {
        return -1;
    }
    if(_lowestAvailableIndex == kMaxAnimationCount-1) {
        // full, don't add any more
        return -1;
    }
    
    int idx = _lowestAvailableIndex;
    _lowestAvailableIndex++;
    _animations[idx] = animation;
    animation->scheduled = true;
    return idx;
}


void
AnimationSystem::
removeAnimation(
    int idx
) {
    if(idx < 0) return;

    Animation *animation = _animations[idx];
    if(idx >= _lowestAvailableIndex || !animation) return;

    animation->scheduled = false;
    _animations[idx] = NULL;
    
    // move the rest down
    for(int j = idx; j < _lowestAvailableIndex-1; j++) {
        _animations[j] = _animations[j+1];
    }
    _lowestAvailableIndex--;
}

void
AnimationSystem::
removeAnimation(
    Animation *toRemove
) {
    if(!toRemove || !toRemove->scheduled) {
        return;
    }
    for(int i = 0; i < _lowestAvailableIndex; i++) {
        Animation *animation = _animations[i];
        if(animation == toRemove) {
            removeAnimation(i);
            break;
        }
    }
}

void
AnimationSystem::
playElapsedTime(
    float deltaAdded
) {
    _elapsedTime += deltaAdded;
    
    for(int i = 0; i < _lowestAvailableIndex; i++) {
        Animation *animation = _animations[i];
        if(!animation->enabled) {
            continue;
        }
        if(_elapsedTime > animation->beginTime) {
            float fraction = (_elapsedTime - animation->beginTime) / animation->duration;
            if(fraction >= 1.0) {
                if(animation->repeats) {
                    animation->beginTime = now();
                } else {
                    removeAnimation(animation);
                    i--;
                    continue;
                }
            }
            animation->animate(fraction);
        }
    }
}

